//----------------------------------------------------------------------------------
// CWindows class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el pintado y update de una ventana de la GUI. Esta ventana es simplemente una agrupacion
// de GuiElements que se lee al inicio de la aplicacion de un fichero .xml
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_GUI_MANAGER_H
#define INC_GUI_MANAGER_H

#include <string>
#include <map>
#include "Math/MathUtils.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "TextBox.h"
#include "Console.h"
//#include "Core/ScriptRegister.h"

//---Forward Declarations---
class CPointerMouse;
class CGuiElement;
class CWindows;
class CRenderManager;
class CFontManager;
class CScriptManager;
//--------------------------

//----------Declaracion de nuevos tipos------------------------------------
typedef enum EtypeTransitionEffect {TE_SHADOW = 0};
struct TransitionEffect {
    TransitionEffect::TransitionEffect(): m_bDoEffect(false), m_fTransitionTime(0.f), m_eType(TE_SHADOW), m_sWindowsName(""), m_fTimeCounter(0.f), m_bActiveWindows(false) {}
    bool									m_bDoEffect;
    float									m_fTransitionTime;
    float									m_fTimeCounter;
    std::string						m_sWindowsName;
    bool									m_bActiveWindows;
    EtypeTransitionEffect	m_eType;

};
//---------------------------------------------------------------------------

class CGUIManager   //:public CScriptRegister
{

public:
    // Init and End protocols
    CGUIManager(const Vect2i &resolution);
    virtual ~CGUIManager()
    {
        Done();
    }


    bool								Init											(const std::string &initGuiXML);
    void								Done											();
    bool								IsOk											() const
    {
        return m_bIsOk;
    }
    void								Render										(CGraphicsManager *renderManager, CFontManager *fontManager);
    void								RenderPointerMouse				(CGraphicsManager *renderManager, CFontManager *fontManager);
    void								Update										(float elapsedTime);
    void								SetScreenResolution				(const Vect2i &resolution);
    bool								LoadGuiFiles							(const std::string &pathWindows);
    bool								ReloadGuiFiles						();
    void								SetMessageBox							(const std::string &text);
    bool								IsVisibleMessage					() const
    {
        assert(m_TextBox);
        return m_TextBox->IsVisible();
    }
    void								ActiveWindows							(const std::string &inNameWindow);
    void                PushWindows               (const std::string &inNameWindow);
    void                PopWindows                ();
    void								ActiveWindowsWithEffect		(const std::string &inNameWindow, EtypeTransitionEffect type, float transitionTime );
    void								SetVisiblePointerMouse		(bool flag)
    {
        m_bVisiblePointerMouse = flag;
    }

    void								SetConsole();


    //----CScriptRegister interface--------------------------------------------
    virtual void				RegisterFunctions					(CScriptManager *scriptManager);

    //-------Funciones para modificar los GuiElements--------------------------
    void								SetActiveGuiElement				(const std::string &inNameGuiElement, bool flag);
    void								SetVisibleGuiElement			(const std::string &inNameGuiElement, bool flag);

    bool								GetProgressBarValue				(const std::string &inNameGuiElement, float &outValue);
    bool								SetProgressBarValue				(const std::string &inNameGuiElement, float inValue);

    bool								NextBlockInRadioBox				(const std::string &inNameRadioBox);
    bool								PrevBlockInRadioBox				(const std::string &inNameRadioBox);

    std::string					GetButtonCheckInRadioBox	(const std::string &inNameRadioBox);
    void								SetButtonCheckInRadioBox	(const std::string &inNameRadioBox, const std::string &button);

    void								SetStateCheckButton 			(const std::string &inCheckButtonName, bool state);
    bool								GetStateCheckButton 			(const std::string &inCheckButtonName);

    void								SetStateSlider						(const std::string &inSliderName, float amount);
    float								GetStateSlider						(const std::string &inSliderName);

    void								SetLiteralInStaticText		(const std::string &inStaticText, const std::string &lit);

    void								SetEditableTextBox				(const std::string &inEditableTextName, const std::string &text);
    std::string					GetEditableTextBox				(const std::string &inEditableTextName);

    void								SetImage									(const std::string &inImageName, const std::string &activeImage);
    std::string					GetImage									(const std::string &inImageName);
    void								PlayImage									(const std::string &inImageName, float timePerImage, bool loop);
    //-----------------------------------------------------------------------------------------------------------//

    bool							GetIsPaused();
    void							SetIsPaused(bool input);

    void							ActivatePause(bool input);

    bool							GetIsDisplayedConsole();
    void							SetIsDisplayedConsole(bool input);
    bool							GetIsDisplayedHeart();
    void							SetIsDisplayedHeart(bool input);
    bool							GetIsDisplayedPixelite();
    void							SetIsDisplayedPixelite(bool input);
    void							SetVisibilityMessage(bool input);
    bool							GetIsDisplayedPolyMessage();
    void							SetIsDisplayedPolyMessage(bool input);
    bool							GetFirstPolyMessageDisplayed();
    void							SetFirstPolyMessageDisplayed(bool input);
    bool							GetIsDisplayedUnlock();
    void							SetIsDisplayedUnlock(bool input);
    float							GetPositionXPercentElement(std::string name);
    float							GetPositionYPercentElement(std::string name);
    void							SetPositionPercentElement(std::string name, float posx, float posy);
    float							GetGUICountHeart();
    void							SetGUICountHeart(float value);
    float							GetGUICountPixelite();
    void							SetGUICountPixelite(float value);
    int							GetGUIPixeliteNumber();
    void							SetGUIPixeliteNumber(int input);
    bool							GetIsGameOver();
    void							SetIsGameOver(bool input);
    float							GetElapsedTime();
    float							GetScreenWidth();
    float							GetScreenHeight();
    void							SetGUIElementSizePercent(std::string _name, int _sizeX, int _sizeY);

    std::string					GetCurrentWindow()
    {
        return m_sCurrentWindows;
    }

private:
    void								Release										();
    void								RenderTransitionEffect		(CGraphicsManager *renderManager);
    bool								UpdateTransitionEffect		(float elapsedTime);

private:
    Vect2i															m_ScreenResolution;
    bool																m_bIsOk;
    std::map<std::string, CWindows *>		m_WindowsMap;
    std::map<std::string, CGuiElement *>	m_ElementsMap;
    std::string													m_sCurrentWindows;
    std::vector<std::string>            m_PrevWindows;
    bool																m_bVisiblePointerMouse;
    bool																m_bRenderError;
    bool																m_bUpdateError;
    CPointerMouse											*m_PointerMouse;
    CTextBox														*m_TextBox;
    CConsole														*m_Console;
    bool																m_bLoadedGuiFiles;
    std::string													m_sLastLoadpathGUI_XML;
    TransitionEffect										m_sTransitionEffect;
    bool																m_bFirstUpdate;

    bool																m_isPaused;

    bool																m_isDisplayedConsole;
    bool																m_isDisplayedHeart;
    bool																m_isDisplayedPixelite;
    bool																m_isDisplayedPolyMessage;
    bool																m_firstPolyMessageSeen;
    bool																m_isDisplayedUnlock;
    float																m_GUICountHeart;
    float																m_GUICountPixelite;
    int																m_GUINumPixelites;
    bool																m_isGameOver;
    float																m_dt;
};

#endif //INC_GUI_MANAGER_H
