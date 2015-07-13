#include "Button.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "Texture\Texture.h"
#include "GraphicsManager.h"
#include "Utils\Logger.h"
#include "Core\ScriptManager.h"
#include "Core\Core.h"
#include "ButtonList.h"


//---Constructor
CButtonList::CButtonList (	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
                            const Vect2f position_percent, std::string lit, uint32 textHeightOffset,
                            uint32 textWidthOffset, bool isVisible, bool isActive)
  : CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, BUTTON, lit, textHeightOffset, textWidthOffset, isVisible, isActive)
{}


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
    CGuiElement::Update(intputManager, elapsedTime);

    Vect2i mousePosition;
    if (intputManager->HasGamePad())
      intputManager->GetPosition(IDV_GAMEPAD1, mousePosition);
    else
      intputManager->GetPosition(IDV_MOUSE, mousePosition);
    CGuiElement::CalculatePosMouse(mousePosition);
  }
}
