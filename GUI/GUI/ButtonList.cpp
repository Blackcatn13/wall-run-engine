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
  , m_currentId(0)
{}

CButtonList::CButtonList(CXMLTreeNode &node, uint32 windowsHeight, uint32 windowsWidth)
  : CGuiElement(node, windowsHeight, windowsWidth, BUTTON_LIST)
  , m_currentId(0) {
  CXMLTreeNode m = node["Buttons"];
  if (m.Exists()) {
    int childs = m.GetNumChildren();
    for (int i = 0; i < childs; ++i) {
      CButton *b = new CButton(m.getNextChild(), windowsHeight, windowsWidth);
      m_buttons.push_back(b);
      AddChild(b);
    }
  }
  Vect2i pos = m_buttons[0]->GetPosition();
  pos += v2iONE;
  m_lastMousePosition = pos;
  if (m_bIsVisible && m_bIsActive)
    INPUTM->setFakePosition(m_lastMousePosition);
}

CButtonList::~CButtonList() {
  for (auto it = m_Children.begin(); it != m_Children.end(); ++it) {
    delete(*it);
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
      Vect2i mousePos;
      intputManager->GetPosition(IDV_MOUSE, mousePos);
      if (mousePos != m_lastMousePosition) {
        intputManager->setFakePosition(m_lastMousePosition);
      }
      if (ACT2IN->DoAction("MenuDown")) {
        m_currentId++;
        if (m_currentId == m_buttons.size())
          m_currentId = 0;
        Vect2i pos = m_buttons[m_currentId]->GetPosition();
        pos += v2iONE;
        m_lastMousePosition = pos;
        intputManager->setFakePosition(m_lastMousePosition);
      } else if (ACT2IN->DoAction("MenuUp")) {
        m_currentId--;
        if (m_currentId == -1)
          m_currentId = m_buttons.size() - 1;
        Vect2i pos = m_buttons[m_currentId]->GetPosition();
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
