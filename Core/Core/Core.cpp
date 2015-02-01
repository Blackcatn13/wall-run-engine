#include "Core.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
#include "SoundManager.h"
#include "Utils/Defines.h"
#include "InputManager.h"
#include "Language/LanguageManager.h"
#include "Font/FontManager.h"
#include "ActionToInput.h"
#include "Renderable\RenderableObjectsManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Texture\TextureManager.h"
#include "AnimatedModels\AnimatedModelManager.h"
#include "ScriptManager.h"
#include "Lights\LightManager.h"
#include "Cinematics\CinematicController.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Camera\CameraController.h"

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
    m_GraphicsManager->Init(handler, m_Config.FullScreen, m_Config.Screen_Width, m_Config.Screen_Heigth);
    m_SoundManager = new CSoundManager();
    m_EffectManager = new CEffectManager();
    m_EffectManager->Load(m_Config.EffectPath);
    //m_RenderableObjectTechniqueManager = new CRenderableObjectTechniqueManager();
    //Load?
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
    m_TextureManager = new CTextureManager();
    m_StaticMeshManager = new CStaticMeshManager();
    m_StaticMeshManager->Load(m_Config.MeshesPath);
    m_ScriptManager = new CScriptManager();
    m_ScriptManager->Initialize();
    m_RenderableManager = new CRenderableObjectsManager();
    m_AnimatedModelManager = new CAnimatedModelManager();
    m_RenderableManager->Load(m_Config.RenderablePath);
    m_RenderableManager->Load(m_Config.LuaPath);
    m_LightManager = new CLightManager();
    m_LightManager->Load(m_Config.LightsPath);
    m_CinematicManager = new CCinematicController();
    m_CinematicManager->Load(m_Config.CinematicPath);
    m_CameraController = new CCameraController();
    m_CameraController->Load(m_Config.CameraPath);
    /*m_RenderableManager->Load(m_Config.RenderablePath);
    m_ScriptManager = new CScriptManager();
    m_ScriptManager->Initialize();
    m_LightManager = new CLightManager();*/
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
    CHECKED_DELETE(m_StaticMeshManager);
    CHECKED_DELETE(m_AnimatedModelManager);
    CHECKED_DELETE(m_RenderableManager);
    CHECKED_DELETE(m_LightManager);
    CHECKED_DELETE(m_TextureManager);
    CHECKED_DELETE(m_ScriptManager);
    CHECKED_DELETE(m_CameraController);
    CHECKED_DELETE(m_CinematicManager);
    CHECKED_DELETE(m_EffectManager);
    //CHECKED_DELETE(m_RenderableObjectTechniqueManager);
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
    m_LightManager->Render(GRAPHM);
    m_SoundManager->Render();
}

void CCore::Update(float dt)
{
    m_GraphicsManager->Update();
    m_SoundManager->Update();
    m_InputManager->Update();
}
