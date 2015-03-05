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
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Camera\CameraController.h"
#include "PhysicsManager.h"
#include "RenderableCommands\SceneRendererCommandManager.h"
#include "TriggerManager\TriggerManager.h"
#include "PlayerController.h"
#include "Core_Utils/MemLeaks.h"
#include "Core_Utils\LogRender.h"


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
    //m_RenderableManager = new CRenderableObjectsManager();
    m_AnimatedModelManager = new CAnimatedModelManager();
    m_AnimatedModelManager->Load(m_Config.AnimatedMeshPath);
    //Cargamos Technique pools
    m_RenderableObjectTechniqueManager = new CRenderableObjectTechniqueManager();
    m_RenderableObjectTechniqueManager->Load(m_Config.PoolRenderableObjects);
    //Cargando Layers
    m_RenderableLayersManager = new CRenderableObjectsLayersManager();
    m_RenderableLayersManager->Load(m_Config.RenderablePath);
    m_RenderableLayersManager->Load(m_Config.LuaPath);
    //m_RenderableLayersManager->Load(m_Config.LuaPath);
    //como cargar LuaPath?
    //m_RenderableLayersManager->Load(m_Config.LuaPath);
    //m_RenderableManager->Load(m_Config.RenderablePath);
    //m_RenderableManager->Load(m_Config.LuaPath);
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
    m_PhysicsManager = new CPhysicsManager();
    m_PhysicsManager->Init();
    m_LogRender = new CLogRender();
    m_SceneRendererCommandManager = new CSceneRendererCommandManager();
    m_SceneRendererCommandManager->Load(m_Config.SceneRenderCommandsPath);
    m_TriggerManager = new CTriggerManager();  
	m_ScriptManager->Load(m_Config.LuaPath);
    m_PlayerController = new CPlayerController();
    //m_TriggerManager->LoadTriggers("./Data/triggers.xml");
  
    m_LuaLoadLevelFunc = m_Config.LuaLevelObjectsFunc;
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
    //CHECKED_DELETE(m_RenderableManager);
    CHECKED_DELETE(m_RenderableLayersManager);
    CHECKED_DELETE(m_LightManager);
    CHECKED_DELETE(m_TextureManager);
    CHECKED_DELETE(m_ScriptManager);
    CHECKED_DELETE(m_CameraController);
    CHECKED_DELETE(m_CinematicManager);
    CHECKED_DELETE(m_EffectManager);
    CHECKED_DELETE(m_TriggerManager);
    CHECKED_DELETE(m_PlayerController);
    CHECKED_DELETE(m_PhysicsManager);
    //CHECKED_DELETE(m_Process);
    CHECKED_DELETE(m_RenderableObjectTechniqueManager);
    CHECKED_DELETE(m_SceneRendererCommandManager);
    CHECKED_DELETE(m_LogRender);
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
    // m_LightManager->Render(m_GraphicsManager);
    m_PhysicsManager->DebugRender(m_GraphicsManager);
    m_SoundManager->Render();
}

void CCore::Update(float dt)
{
    m_GraphicsManager->Update();
    m_PhysicsManager->Update(dt);
    m_SoundManager->Update();
    m_InputManager->Update();
    m_LogRender->Update(dt);
    //m_PlayerController->Move(dt);
}


void CCore::Trace(const std::string & msg )
{
	std::cout << msg << std::endl;
}