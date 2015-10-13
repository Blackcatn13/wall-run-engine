#include "Button.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "Texture\Texture.h"
#include "GraphicsManager.h"
#include "Utils\Logger.h"
#include "Core\ScriptManager.h"
#include "Core\Core.h"
#include "ButtonList.h"
#include "XML\XMLTreeNode.h"


//---Constructor
CButtonList::CButtonList (	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
                            const Vect2f position_percent, uint32 columns, uint32 rows, std::string lit, uint32 textHeightOffset,
                            uint32 textWidthOffset, bool isVisible, bool isActive)
  : CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, BUTTON_LIST, lit, textHeightOffset, textWidthOffset, isVisible, isActive)
  , m_currentIdX(0)
  , m_currentIdY(0)
{}

CButtonList::CButtonList(CXMLTreeNode &node, uint32 windowsHeight, uint32 windowsWidth)
  : CGuiElement(node, windowsHeight, windowsWidth, BUTTON_LIST)
  , m_currentIdX(0)
  , m_currentIdY(0) {
  Vect2i pos = Vect2i(-1, -1);
  CXMLTreeNode m = node["Buttons"];
  if (m.Exists()) {
    int childs = m.GetNumChildren();
    m_buttons = std::vector<std::vector<CButton *>>(childs);
    for (int i = 0; i < childs; ++i) {
      CXMLTreeNode n = m.getNextChild();
      int childs1 = n.GetNumChildren();
      for (int j = 0; j < childs1; ++j) {
        CXMLTreeNode node1 = n.getNextChild();
        std::string name = node1.GetName();
        CButton *b;
        if (name == "Button") {
          b = new CButton(node1, windowsHeight, windowsWidth);
        } else if (name == "ButtonRef") {
          b = (CButton *)m_Children.find(node1.GetPszISOProperty("name", "", false))->second;
        }
        m_buttons[i].push_back(b);
        if (b->IsVisible() && pos == Vect2i(-1, -1)) {
          pos = b->GetPosition();
          m_currentIdX = i;
          m_currentIdX = j;
        }
        AddChild(b);
      }
    }
  }
  pos += v2iONE;
  m_lastMousePosition = pos;
  if (m_bIsVisible && m_bIsActive)
    INPUTM->setFakePosition(m_lastMousePosition);
}

CButtonList::~CButtonList() {
  for (auto it = m_Children.begin(); it != m_Children.end(); ++it) {
    delete(it->second);
  }
}

//---------------Interfaz de GuiElement----------------------
void CButtonList::Render	(CGraphicsManager *renderManager, CFontManager *fm) {
  if ( CGuiElement::m_bIsVisible) {
    //Primero renderizamos todos los hijos que pudiera tener el Button:
    CGuiElement::Render(renderManager, fm);
    //Finalmente renderizamos el texto:
    CGuiElement::RenderText(renderManager, fm);

  }//END if( CGuiElement::m_bIsVisible )
}

void CButtonList::Update(CInputManager *intputManager, float elapsedTime) {
  if ( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive ) {
    //Primero actualizamos todos los hijos que pudiera tener el button:
    if (intputManager->HasGamePad()) {
      bool actionDone = false;
      Vect2i mousePos;
      intputManager->GetPosition(IDV_MOUSE, mousePos);
      if (mousePos != m_lastMousePosition) {
        intputManager->setFakePosition(m_lastMousePosition);
      }
      if (ACT2IN->DoAction("MenuDown")) {
        do {
          m_currentIdY++;
          if (m_currentIdY == m_buttons[m_currentIdX].size())
            m_currentIdY = 0;
        } while (!m_buttons[m_currentIdX][m_currentIdY]->IsVisible());
        actionDone = true;
      } else if (ACT2IN->DoAction("MenuUp")) {
        do {
          m_currentIdY--;
          if (m_currentIdY == -1)
            m_currentIdY = m_buttons[m_currentIdX].size() - 1;
        } while (!m_buttons[m_currentIdX][m_currentIdY]->IsVisible());
        actionDone = true;
      } else if (ACT2IN->DoAction("MenuRight")) {
        do {
          m_currentIdX++;
          if (m_currentIdX == m_buttons.size())
            m_currentIdX = 0;
        } while (!m_buttons[m_currentIdX][m_currentIdY]->IsVisible());
        actionDone = true;
      } else if (ACT2IN->DoAction("MenuLeft")) {
        do {
          m_currentIdX--;
          if (m_currentIdX == -1)
            m_currentIdX = m_buttons.size() - 1;
        } while (!m_buttons[m_currentIdX][m_currentIdY]->IsVisible());
        actionDone = true;
      }
      if (actionDone) {
        Vect2i pos = m_buttons[m_currentIdX][m_currentIdY]->GetPosition();
        pos += v2iONE;
        m_lastMousePosition = pos;
        intputManager->setFakePosition(m_lastMousePosition);
      }
      intputManager->setUseFakePosition(true);
      CGuiElement::Update(intputManager, elapsedTime);
      intputManager->setUseFakePosition(false);
    } else {
      CGuiElement::Update(intputManager, elapsedTime);
    }
  }
}

void CButtonList::OnClickedChild(const std::string &name) {
}
