#pragma once
#ifndef INC_LIFE_H
#define INC_LIFE_H

#include "GuiElement.h"
#include "Utils\Defines.h"
#include "GraphicsManager.h"

class CImage;
class CTexture;
class CInputManager;
class CFontManager;

class CLifeGUI: public CGuiElement
{
public:
    CLifeGUI( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
              const Vect2f position_percent, std::string lit = "", uint32 textHeightOffset = 0, uint32 textWidthOffset = 0,
              bool isVisible = true, bool isActive = true);

    ~CLifeGUI();

    void UpdateGUI(CInputManager* input, float elapsedTime);
    void RenderGUI(CGraphicsManager* render, CFontManager* fm);
    void SetBackgroundTexture(CTexture* back, std::string path);
    void SetHeartTexture(CTexture* heart, std::string path);
    void SetHeartVisible(bool visible);
    void SetBackgroundVisible(bool visible);

private:
    uint32		m_life;
    CImage*		m_heart;
    CImage*		m_back;
    std::string			m_sActiveTexture;
    bool				m_bAnimated;
    bool				m_bLoop;
    float				m_fTimePerImage;
    float				m_fCounter;
    ETypeFlip			m_eFlip;
};

#endif //INC_LIFE_H