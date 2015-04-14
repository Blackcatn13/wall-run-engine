#include "Slider.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "Utils/Logger.h"
#include "Core/ScriptManager.h"
#include "Core/Core.h"


CSlider::CSlider(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
									const Vect2f position_percent,  float buttonWidthPercent, float buttonHeightPercent,
									float value, std::string lit, uint32 textHeightOffset, uint32 textWidthOffset, bool isVisible, bool isActive)

: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, SLIDER, lit, textHeightOffset, textWidthOffset, isVisible, isActive)
, m_Button(	windowsHeight, windowsWidth, buttonHeightPercent, buttonWidthPercent,  position_percent, "", 0, 0, isVisible, isActive )
, m_fValue( value )
, m_bStart_to_Move( false )
, m_pBackGroundTexture(NULL)
{
	Vect2f position;
	position.x = position_percent.x + witdh_percent*value - buttonWidthPercent*0.5f;
	position.y = position_percent.y + height_precent*0.5f - buttonHeightPercent*0.5f;
	m_Button.SetPositionPercent(position);
}



//---------------Interfaz de GuiElement----------------------
void CSlider::Render	(CGraphicsManager *renderManager, CFontManager* fm)
{
	if( CGuiElement::m_bIsVisible)
	{
		//Primero renderizamos todos los hijos que pudiera tener el Button:
		CGuiElement::Render(renderManager, fm);

		//Despues renderizamos el fondo del slider
		if (m_pBackGroundTexture)
		{
			renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pBackGroundTexture);
		}
		else
		{
			renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_BackGroundColor);
		}

		//Pintamos el boton del slider
		m_Button.Render(renderManager, fm);

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText(renderManager, fm);	
	}
}

void CSlider::Update(CInputManager* intputManager, float elapsedTime)
{
	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
	{
		//Primero actualizamos todos los hijos que pudiera tener el checkButton:
		CGuiElement::Update(intputManager, elapsedTime);
		
		m_Button.Update( intputManager, elapsedTime );
		Vect2i mousePosition;
		intputManager->GetPosition(IDV_MOUSE, mousePosition);
		CGuiElement::CalculatePosMouse(mousePosition);
		if( m_Button.IsClicking() ||( m_bStart_to_Move && intputManager->IsDown(IDV_MOUSE, MOUSE_BUTTON_LEFT)) )
		{
			CGuiElement::m_bFocus = true;

			if( !m_bStart_to_Move )
			{
				m_bStart_to_Move = true;
				m_fPreviousPosMouseX = mousePosition.x;
			}
			else
			{
				m_fPrevValue = m_fValue;

				//Mover el botón del slider por el carril
				Vect2i	buttonPos			= m_Button.GetPosition();
				uint32	buttonHeight	= m_Button.GetHeight();
				uint32	buttonWidth		= m_Button.GetWidth();

				int32 incPosX = mousePosition.x - m_fPreviousPosMouseX;
				m_fPreviousPosMouseX = mousePosition.x;

				if( buttonPos.x+ buttonWidth*0.5 + incPosX > m_Position.x + m_uWidth)
				{
					buttonPos.x = m_Position.x - (uint32)(buttonWidth*0.5) + m_uWidth;
					m_fValue = 1.f;
					m_bStart_to_Move = false;
				}
				else if( buttonPos.x + buttonWidth*0.5 + incPosX < m_Position.x )
				{
					buttonPos.x = m_Position.x - (uint32)(buttonWidth*0.5);
					m_fValue = 0.f;
					m_bStart_to_Move = false;
				}
				else
				{
					buttonPos.x += incPosX;
					m_fValue = float(( buttonPos.x + buttonWidth - m_Position.x ) / (float)m_uWidth);
					if (m_fValue > 1.0f)
						m_fValue = 1.0f;
				}
				m_Button.SetPosition( buttonPos );

				if (m_fPrevValue != m_fValue)
				{
					OnChangeValue();
				}
			}
		}
		else //else ( m_Button.IsClicking() )
		{
			m_bStart_to_Move = false;		
		}

	}//END if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
}


//---------------Interfaz de Slider---------------------------
void CSlider::SetValue( float value )
{
	if( value >= 0.f && value <= 1.f )
	{
		Vect2i	buttonPos;
		uint32	buttonHeight	= m_Button.GetHeight();
		uint32	buttonWidth		= m_Button.GetWidth();
		buttonPos.x =  uint32(m_Position.x + m_uWidth*value - buttonWidth*0.5f); //Se desplaza por una linea que ocupa 60%
		buttonPos.y =  uint32(m_Position.y + m_uHeight*0.5f - buttonHeight*0.5f);
		m_Button.SetPosition( buttonPos );
		m_fValue = value;
	}
}
void CSlider::SetOnClickedAction (std::string & inAction )
{
	m_Button.SetOnClickedAction(inAction);
}

void CSlider::SetOnOverAction	(std::string & inAction )
{
	m_Button.SetOnOverAction(inAction);
}

void CSlider::SetOnChangeValueAction (std::string & inAction)
{
	m_sLuaCode_OnChangeValue = inAction;
}

void CSlider::SetButtonTextures	(CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated)
{
	m_Button.SetTextures(normal, over, clicked, deactivated);
}

void CSlider::SetBackGroundTexture (CTexture* background)
{
	m_pBackGroundTexture = background;
}

void CSlider::SetButtonColors	(const CColor& normal, const CColor& over, const CColor& clicked, const CColor& deactivated, float alpha)
{
	m_Button.SetColors(normal, over, clicked, deactivated, alpha);
}

void CSlider::SetBackGroundColor (const CColor& background, float alpha)
{
	m_BackGroundColor	= background;
	m_BackGroundColor.SetAlpha(alpha);
}

void CSlider::OnChangeValue( void )
{
	if (m_sLuaCode_OnChangeValue.compare(""))
	{
		//Lanzar acción en Lua:
		CScriptManager* scriptManager = CCORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnChangeValue);
	}
}