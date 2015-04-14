#include "CheckButton.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "Utils/Logger.h"
#include "Core/ScriptManager.h"
#include "Core/Core.h"


//---Constructor
CCheckButton::CCheckButton(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
														const Vect2f position_percent, bool isOn, std::string lit, uint32 textHeightOffset,
														uint32 textWidthOffset, bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, CHECKBUTTON, lit, textHeightOffset, textWidthOffset, isVisible,isActive)
, m_pOnTexture(NULL)
, m_pOffTexture(NULL)
, m_pDeactivatedTexture(NULL)
, m_sLuaCode_OnCheckOn("")
, m_sLuaCode_OnCheckOff("")
, m_sLuaCode_OnOver("")
, m_OnColor(colGREEN)
, m_OffColor(colRED)
, m_DeactivatedColor(colYELLOW)
{
	if (isOn)
		m_eState = CBS_ON;
	else
		m_eState = CBS_OFF;
}

//---------------Interfaz de CGuiElement----------------------
void CCheckButton::Render (CGraphicsManager *renderManager, CFontManager* fm)
{
	if( CGuiElement::m_bIsVisible)
	{
		//Primero renderizamos todos los hijos que pudiera tener el checkBbutton:
		CGuiElement::Render(renderManager, fm);

		if( CGuiElement::m_bIsActive )
		{
			if( m_eState == CBS_ON )
			{
				if (m_pOnTexture)
				{
					renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pOnTexture);
				}
				else
				{
					renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_OnColor);
				}
			}
			else
			{
				if (m_pOnTexture)
				{
					renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pOffTexture);
				}
				else
				{
					renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_OffColor);
				}
			}
		}
		else
		{
			if (m_pDeactivatedTexture)
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pDeactivatedTexture);
			}
			else
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_DeactivatedColor);
			}
		}

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText(renderManager, fm);	

	} //END if( CGuiElement::m_bIsVisible )
}

void CCheckButton::Update (CInputManager* intputManager, float elapsedTime)
{

	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
	{
		//Primero actualizamos todos los hijos que pudiera tener el checkButton:
		CGuiElement::Update(intputManager, elapsedTime);

		Vect2i mousePosition;
		intputManager->GetPosition(IDV_MOUSE, mousePosition);
		CGuiElement::CalculatePosMouse(mousePosition);
		
		if( CGuiElement::IsInside() )
		{
			if( CGuiElement::IsOver() )
			{
				OnOverButton();
			}

			if (intputManager->IsUpDown(IDV_MOUSE,MOUSE_BUTTON_LEFT))
			{
				if (m_eState == CBS_ON)
				{
					m_eState = CBS_OFF;
					OnCheckOff();
				}
				else
				{
					m_eState = CBS_ON;
					OnCheckOn();
				}
			}
		}
	}
}

//---------------Interfaz de CheckButton---------------------------
void CCheckButton::SetTextures (CTexture* on, CTexture* off, CTexture* deactivated)
{
	m_pOnTexture					= on;
	m_pOffTexture					= off;
	m_pDeactivatedTexture	= deactivated;
}

void CCheckButton::SetColors (const CColor& on, const CColor& off, const CColor& deactivated, float alpha)
{
	m_OnColor						= on;
	m_OffColor					= off;
	m_DeactivatedColor	= deactivated;

	m_OnColor.SetAlpha(alpha);
	m_OnColor.SetAlpha(alpha);
	m_DeactivatedColor.SetAlpha(alpha);
}

void CCheckButton::SetOnCheckOnAction (const std::string& inAction)
{
	m_sLuaCode_OnCheckOn = inAction;	
}

void CCheckButton::SetOnCheckOffAction (const std::string& inAction)
{
	m_sLuaCode_OnCheckOff = inAction;
}

void CCheckButton::SetOnOverAction (const std::string& inAction)
{
	m_sLuaCode_OnOver = inAction;
}

void CCheckButton::OnCheckOn ()
{
	if( CGuiElement::m_pParent != NULL )
		m_pParent->OnClickedChild(CGuiElement::m_sName);

	if (m_sLuaCode_OnCheckOn.compare(""))
	{
		//Lanzar acción en Lua:
		CScriptManager* scriptManager = CCORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnCheckOn);
	}
}

void CCheckButton::OnCheckOff ()
{
	if( CGuiElement::m_pParent != NULL )
		m_pParent->OnClickedChild(CGuiElement::m_sName);

	if (m_sLuaCode_OnCheckOff.compare(""))
	{
		//Lanzar acción en Lua:
		CScriptManager* scriptManager = CCORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnCheckOff);
	}
}

void CCheckButton::OnOverButton ()
{
	if (m_sLuaCode_OnOver.compare(""))
	{
		//Lanzar acción en Lua:
		CScriptManager* scriptManager = CCORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnOver);
	}
}
