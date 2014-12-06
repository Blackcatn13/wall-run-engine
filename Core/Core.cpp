#include "Core.h"
#include "GraphicsManager.h"
#include "SoundManager.h"
#include "Utils/Defines.h"
#include "InputManager.h"
#include "LanguageManager.h"
#include "FontManager.h"
#include "ActionToInput.h"

CCore* CCore::m_Instance = 0;

CCore::CCore()
{
}

CCore::~CCore()
{
    DeInit();
}

void CCore::Init(HWND handler)
{
    m_GraphicsManager = new CGraphicsManager();
    m_SoundManager = new CSoundManager();
    m_GraphicsManager->Init(handler, m_Config.FullScreen, m_Config.Screen_Width, m_Config.Screen_Heigth);
    m_InputManager = new CInputManager();
    m_InputManager->Init(handler, Vect2i(m_Config.Screen_Width, m_Config.Screen_Heigth), m_Config.Mouse_Exclusive);
    m_LanguageManager = new CLanguageManager();
    m_FontManager = new CFontManager();
    m_FontManager->Init(m_GraphicsManager);
    m_FontManager->LoadTTFs(m_Config.FontsPath);
    for (uint32 i = 0; i < m_Config.LanguagesPath.size(); ++i)
        m_LanguageManager->SetXmlFile(m_Config.LanguagesPath[i]);
    m_LanguageManager->LoadXMLs();
    m_LanguageManager->SetCurrentLanguage(m_Config.CurrentLanguage);
    m_ActionToInput = new CActionToInput(m_InputManager);
    m_ActionToInput->LoadXML(m_Config.ActionsPath);
}

void CCore::DeInit()
{
    CHECKED_DELETE(m_GraphicsManager);
    CHECKED_DELETE(m_SoundManager);
	m_InputManager->Done();
	CHECKED_DELETE(m_InputManager);
	CHECKED_DELETE(m_LanguageManager);
	CHECKED_DELETE(m_FontManager);
	CHECKED_DELETE(m_ActionToInput);
}

CCore* CCore::GetInstance()
{
    if (m_Instance == 0) {
        m_Instance = new CCore();
    }
    return m_Instance;
}

void CCore::Render()
{
    //m_GraphicsManager->Render();
    m_SoundManager->Render();
}

void CCore::Update(float dt)
{
    m_GraphicsManager->Update();
    m_SoundManager->Update();
    m_InputManager->Update();
}
