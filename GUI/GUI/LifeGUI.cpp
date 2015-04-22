#include "LifeGUI.h"
#include "Image.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "Font\FontManager.h"

//---Constructor
CLifeGUI::CLifeGUI( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
							 const Vect2f position_percent, std::string lit, uint32 textHeightOffset, 
							 uint32 textWidthOffset, bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, IMAGE, lit, 
		textHeightOffset, textWidthOffset, isVisible,isActive)
, m_back(new CImage(windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent))
, m_heart(new CImage(windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent))
, m_sActiveTexture( "default" )
, m_bAnimated(false)
, m_bLoop(false)
, m_fTimePerImage(0.f)
, m_fCounter(0.f)
, m_eFlip(NONE_FLIP)
, m_life(0)
{}

void CLifeGUI::UpdateGUI(CInputManager* input, float elapsedTime)
{
	Update(input, elapsedTime);
	m_back->Update(input, elapsedTime);
	m_back->SetPosition(Vect2i(5,150));
	m_heart->Update(input, elapsedTime);
	m_heart->SetPosition(Vect2i(5,150));

}

void CLifeGUI::RenderGUI(CGraphicsManager* render, CFontManager* fm)
{
	Render(render, fm);
	m_back->Render(render, fm);
	m_heart->Render(render, fm);
}

void CLifeGUI::SetBackgroundTexture(CTexture* back, std::string path)
{
	m_back->SetTexture(back, path);
}

void CLifeGUI::SetHeartTexture(CTexture* heart, std::string path)
{
	m_heart->SetTexture(heart, path);
}

void CLifeGUI::SetHeartVisible(bool visible)
{
	m_heart->SetVisible(visible);
}

void CLifeGUI::SetBackgroundVisible(bool visible)
{
	m_back->SetVisible(visible);
}

/*
void CLifeGUI::Update(CInputManager* intputManager, float elapsedTime)
{
	std::string buffer_prev = m_sBuffer;

	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
	{
		Vect2i mousePosition;
		intputManager->GetPosition(IDV_MOUSE, mousePosition);
		CGuiElement::CalculatePosMouse(mousePosition);
		if( CGuiElement::IsInside() )
		{
			if (intputManager->IsUpDown(IDV_MOUSE, MOUSE_BUTTON_LEFT))
			{
				CGuiElement::m_bFocus = true;
			}
		}
	}

	if (CGuiElement::m_bFocus)
	{
		m_bShift = (intputManager->IsDown(IDV_KEYBOARD, KEY_RSHIFT) || intputManager->IsDown(IDV_KEYBOARD, KEY_LSHIFT) );
		
		if( intputManager->IsUpDown(IDV_KEYBOARD, KEY_LEFT) )
		{
			
			if (m_uCursorPos > 0) 
			{
				m_uCursorPos--;
			}
		}

		if( intputManager->IsUpDown(IDV_KEYBOARD, KEY_RIGHT) )
		{
			m_uCursorPos++;
			if (m_uCursorPos > m_sBuffer.size())
			{
				m_uCursorPos = (uint8)m_sBuffer.size();
			}
		}

		if( intputManager->IsUpDown(IDV_KEYBOARD, KEY_BACK) )
		{
			Remove1character();
		}
		else
		{
			ProcessNewEntries(intputManager);
		}

		if( intputManager->IsUpDown(IDV_KEYBOARD, KEY_RETURN) )
		{
			m_bReturnPress = true;
			m_uCursorPos = (uint8)m_sBuffer.size();
		}

		if( intputManager->IsDown(IDV_KEYBOARD, KEY_BACK) )
		{
			m_BackTime1 += elapsedTime;
			if (m_BackTime1 > 0.7f)
			{
				m_BackTime2 += elapsedTime;
				if (m_BackTime2 > 0.05f)
				{
					Remove1character();
					m_BackTime2 = 0.f;
				}
			}		
		}
		else
		{
			m_BackTime1 = 0.f;
			m_BackTime2 = 0.f;
		}

		if (intputManager->IsUpDown(IDV_KEYBOARD, KEY_RSHIFT))
		{
			if (m_uCursorPos < m_sBuffer.size())
			{
				m_sBuffer.erase( m_sBuffer.begin()+m_uCursorPos );
			}
		}

	
		
		m_fTimeCount += elapsedTime;
		if( m_fTimeCount > m_fTime )
		{
			if( !m_sFocusObject.compare(" ") )
				m_sFocusObject = "_";
			else
				m_sFocusObject = " ";

			m_fTimeCount = 0.f;
		}
	}// End if (CGuiElement::m_bFocus)

	std::string temp = m_sBuffer+m_sFocusObject;
	CFontManager* fm = CCORE->GetFontManager();
	uint32 sizeX = fm->SizeX(temp.c_str(), m_uFontID) + (uint32)(2*m_fWidthMargin);
	if (sizeX > CGuiElement::m_uWidth)
	{
		m_sBuffer = buffer_prev;
	}

	if (ACT2IN->DoAction("ConsoleEnter")){
		IsReturnPress();
	}

	if (ACT2IN->DoAction("KeyMoveBack") && m_buffers.size() > 0){
		if (m_bufferPos < m_buffers.size()-1){
			m_bufferPos += 1;
			m_sBuffer = m_buffers[m_bufferPos];
			m_uCursorPos = m_sBuffer.size();
		}else{
			m_sBuffer = "";
			m_uCursorPos = 0;
		}
	}
	if (ACT2IN->DoAction("KeyMoveForward") && m_buffers.size() > 0){
		m_sBuffer = m_buffers[m_bufferPos];
		if (m_bufferPos > 0)
			m_bufferPos -= 1;
		m_uCursorPos = m_sBuffer.size();
	}

	if (ACT2IN->DoAction("ConsoleStart")){
		m_uCursorPos = 0;
	}
	if (ACT2IN->DoAction("ConsoleEnd")){
		m_uCursorPos = m_sBuffer.size();
	}
}

*/