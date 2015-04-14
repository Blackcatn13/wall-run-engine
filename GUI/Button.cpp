#include "Button.h"
#include "InputManager.h"
#include "Texture\Texture.h"
#include "GraphicsManager.h"
#include "Utils\Logger.h"
#include "Core\ScriptManager.h"
#include "Core\Core.h"


//---Constructor
CButton::CButton (	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
										const Vect2f position_percent, std::string lit, uint32 textHeightOffset,
										uint32 textWidthOffset, bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent,position_percent, BUTTON, lit, textHeightOffset, textWidthOffset, isVisible,isActive)
, m_eState(BS_NORMAL)
, m_sLuaCode_OnClicked("")
, m_sLuaCode_OnOver("")
, m_pNormalTexture(NULL)
, m_pOverTexture(NULL)
, m_pClickedTexture(NULL)
, m_pDeactivatedTexture(NULL)
, m_NormalColor(colBLUE)
, m_OverColor(colGREEN)
, m_ClickedColor(colRED)
, m_DeactivatedColor(colYELLOW)
{}


//---------------Interfaz de GuiElement----------------------
void CButton::Render	(CGraphicsManager *renderManager, CFontManager* fm)
{
	if( CGuiElement::m_bIsVisible)
	{
		//Primero renderizamos todos los hijos que pudiera tener el Button:
		CGuiElement::Render(renderManager, fm);

		switch(m_eState)
		{
		case BS_NORMAL:
			if (m_pNormalTexture)
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pNormalTexture);
			}
			else
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_NormalColor);
			}
			break;
		case BS_OVER:
			if (m_pOverTexture)
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pOverTexture);
			}
			else
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_OverColor);
			}
			break;
		case BS_CLICKED:
			if (m_pClickedTexture)
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pClickedTexture);
			}
			else
			{
				renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_ClickedColor);
			}
			break;
		default:
			LOGGER->AddNewLog(ELL_ERROR,"CButton::Render Tipo de estado desconocido en el boton %s",CGuiElement::m_sName.c_str());
			break;
		}

		if( !CGuiElement::m_bIsActive )
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

	}//END if( CGuiElement::m_bIsVisible )
}

void CButton::Update(CInputManager* intputManager, float elapsedTime)
{
	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
	{
		//Primero actualizamos todos los hijos que pudiera tener el button:
		CGuiElement::Update(intputManager, elapsedTime);

		Vect2i mousePosition;
		intputManager->GetPosition(IDV_MOUSE, mousePosition);
		CGuiElement::CalculatePosMouse(mousePosition);
		if( CGuiElement::IsInside() )
		{
			if(m_eState != BS_CLICKED)
			{
				m_eState = BS_OVER;
			}
			if( CGuiElement::IsOver() )
			{
				OnOverButton();
			}
			
			if (intputManager->IsUpDown(IDV_MOUSE,MOUSE_BUTTON_LEFT) ||
				 (intputManager->IsDown(IDV_MOUSE,MOUSE_BUTTON_LEFT) && m_eState == BS_CLICKED ) )
			{
				m_eState = BS_CLICKED;
			}

			if (intputManager->IsDownUp(IDV_MOUSE,MOUSE_BUTTON_LEFT) && m_eState == BS_CLICKED )
			{
				m_eState = BS_OVER;
				//Si hemos pasado de apretar el botón a no apretarlo entonces lanzar evento
				OnClickedButton();
			}
		}
		else
		{
			m_eState = BS_NORMAL;
		}
	}
}

//---------------Interfaz de Button---------------------------
void CButton::SetTextures (CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated)
{
	m_pNormalTexture			= normal;
	m_pOverTexture				= over;
	m_pClickedTexture			= clicked;
	m_pDeactivatedTexture	= deactivated;
}

void CButton::SetColors (const CColor& normal, const CColor& over, const CColor& clicked, const CColor& deactivated, float alpha)
{
	m_NormalColor				= normal;
	m_OverColor					= over;
	m_ClickedColor			= clicked;
	m_DeactivatedColor	= deactivated;

	m_NormalColor.SetAlpha(alpha);
	m_OverColor.SetAlpha(alpha);
	m_ClickedColor.SetAlpha(alpha);
	m_DeactivatedColor.SetAlpha(alpha);
}

void CButton::SetOnClickedAction( std::string & inAction )
{
	m_sLuaCode_OnClicked = inAction;
}

void CButton::SetOnOverAction( std::string & inAction )
{
	m_sLuaCode_OnOver = inAction;
}

void CButton::OnClickedButton( void )
{
	if( CGuiElement::m_pParent != NULL )
		m_pParent->OnClickedChild(CGuiElement::m_sName);

	if (m_sLuaCode_OnClicked.compare(""))
	{
		//Lanzar acción en Lua:
		CScriptManager* scriptManager = CCORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnClicked);
	}
}

void CButton::OnOverButton( void )
{
	if (m_sLuaCode_OnOver.compare(""))
	{
		//Lanzar acción en Lua:
		CScriptManager* scriptManager = CCORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnOver);
	}
}


