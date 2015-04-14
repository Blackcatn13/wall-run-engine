#include "ProgressBar.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "Font/FontManager.h"
#include "Core/ScriptManager.h"
#include "Core/Core.h"


CProgressBar::CProgressBar(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
														const Vect2f position_percent, std::string lit, uint32 textHeightOffset, uint32 textWidthOffset, 
														bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent,position_percent, PROGRESS_BAR, lit, textHeightOffset, textWidthOffset, isVisible,isActive)
, m_fProgress( 0.f )
, m_sText("Loading")
, m_fCountTime( 0.f )
, m_fTimeToUpdate( 0.5f )
, m_sLuaCode_OnComplete("")
, m_pBackgroundTexture(NULL)
, m_pProgressTexture(NULL)
, m_BackGroundColor(colBLACK)
, m_ProgressColor(colRED)
, m_uFontID(0)
, m_TextColor(colBLACK)
{}

//---------------Interfaz de CGuiElement----------------------
void CProgressBar::Render	(CGraphicsManager *renderManager, CFontManager* fm)
{
	if( CGuiElement::m_bIsVisible)
	{
		if (m_pBackgroundTexture)
		{
			renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_pBackgroundTexture);
		}
		else
		{
			renderManager->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, UPPER_LEFT, m_BackGroundColor);
		}
		
		
		Vect2i posProgress(	(uint32)(CGuiElement::m_Position.x + (uint32)CGuiElement::m_uWidth*0.05f),
												(uint32)(CGuiElement::m_Position.y + (uint32)CGuiElement::m_uHeight*0.2f));
		
		uint32 h = (uint32)(CGuiElement::m_uHeight*0.6f);
		uint32 w = (uint32)(CGuiElement::m_uWidth*0.9f*(m_fProgress*0.01f));
		if (m_pProgressTexture)
		{
			renderManager->DrawQuad2D(posProgress,w,h, UPPER_LEFT, m_pProgressTexture);
		}
		else
		{
			renderManager->DrawQuad2D(posProgress,w,h, UPPER_LEFT, m_ProgressColor);
		}
		Vect2i pos;
		pos.x = (uint32)(CGuiElement::m_Position.x+0.4f*m_uWidth);
		pos.y = (uint32)(CGuiElement::m_Position.y+ 0.2f*m_uHeight);
		fm->DrawText(pos.x, pos.y, m_TextColor, m_uFontID, m_sText.c_str());

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText(renderManager, fm);	
	}
}

void CProgressBar::Update(CInputManager* intputManager, float elapsedTime)
{
	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
	{
		m_fCountTime += elapsedTime;
		if( m_fCountTime > m_fTimeToUpdate )
		{
			m_fCountTime = 0.f;

			if( m_sText.compare("Loading") == 0 )
				m_sText = "Loading.";
			else if( m_sText.compare("Loading.") == 0 )
				m_sText = "Loading..";
			else if( m_sText.compare("Loading..") == 0 )
				m_sText = "Loading...";		
			else if( m_sText.compare("Loading...") == 0 )
				m_sText = "Loading";		
		}
	}
}


//---------------Interfaz de ProgressBar---------------------------
void CProgressBar::SetTextures (CTexture* background, CTexture* progress)
{
	m_pBackgroundTexture	= background;
	m_pProgressTexture		= progress;
}

void CProgressBar::SetColors (const CColor& background, const CColor& progress)
{
	m_BackGroundColor = background;
	m_ProgressColor		= progress;
}

void CProgressBar::SetFont (uint32 fontID, const CColor& textColor)
{
	m_uFontID = fontID;
	m_TextColor = textColor;
}

void CProgressBar::SetProgress (float progress)
{
	m_fProgress = progress;
	if( m_fProgress >= 100 )
	{
		m_fProgress = 0;
		OnComplete();
	}

}

void CProgressBar::OnComplete( void )
{
	if (m_sLuaCode_OnComplete.compare(""))
	{
		//Lanzar acción en Lua:
		CScriptManager* scriptManager = CCORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnComplete);
	}
}

