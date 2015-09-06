#include "Core.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
//#include "SoundManager.h"
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
//#include "Core_Utils/MemLeaks.h"
#include "Core_Utils\LogRender.h"
#include "AI\WPManager.h"
#include "Enemies\EnemyManager.h"
#include "Particles\ParticleManager.h"
#include "Particles\BillboardManager.h"
#include "AI\FSMManager.h"
//#include "SoundManager.h"
#include "WWSoundManager.h"
#include "AI\PuzzleManager.h"
#include "GUI\GUIManager.h"
#include "Collectibles\CollectibleManager.h"
#include "SceneElements\PolyPlatform.h"
#ifdef _DEBUG
#include "Core_Utils\Timer.h"
#endif

CCore *CCore::m_Instance = 0;

CCore::CCore():
  m_ElapsedTime(0.0f) {
}

CCore::~CCore() {
  DeInit();
}

void CCore::Init(HWND handler) {
#ifdef _DEBUG
  std::cout << "Creating Graphics Manager";
  CTimer t = CTimer(1);
  CTimer t1 = CTimer(1);
  t.Update();
  t1.Update();
#endif
  m_GraphicsManager = new CGraphicsManager();
  m_GraphicsManager->Init(handler, m_Config.FullScreen, m_Config.Screen_Width, m_Config.Screen_Heigth);
  //m_SoundManager = new CSoundManager();
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Effect Manager";
#endif
  m_EffectManager = new CEffectManager();
  m_EffectManager->Load(m_Config.EffectPath);
  //m_RenderableObjectTechniqueManager = new CRenderableObjectTechniqueManager();
  //Load?
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Renderable Object Technique Manager";
#endif
  m_RenderableObjectTechniqueManager = new CRenderableObjectTechniqueManager();
  m_RenderableObjectTechniqueManager->Load(m_Config.PoolRenderableObjects);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Input Manager";
#endif
  m_InputManager = new CInputManager();
  m_InputManager->Init(handler, Vect2i(m_Config.Screen_Width, m_Config.Screen_Heigth), m_Config.Mouse_Exclusive);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Font Manager";
#endif
  m_FontManager = new CFontManager();
  m_FontManager->Init(m_GraphicsManager);
  m_FontManager->LoadTTFs(m_Config.FontsPath);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Language Manager";
#endif
  m_LanguageManager = new CLanguageManager();
  for (uint32 i = 0; i < m_Config.LanguagesPath.size(); ++i)
    m_LanguageManager->SetXmlFile(m_Config.LanguagesPath[i]);
  m_LanguageManager->LoadXMLs();
  m_LanguageManager->SetCurrentLanguage(m_Config.CurrentLanguage);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Action To Input";
#endif
  m_ActionToInput = new CActionToInput(m_InputManager);
  m_ActionToInput->LoadXML(m_Config.ActionsPath);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Texture Manager";
#endif
  m_TextureManager = new CTextureManager();
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Static Mesh Manager";
#endif
  m_StaticMeshManager = new CStaticMeshManager();
  m_StaticMeshManager->Load(m_Config.MeshesPath);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Script Manager";
#endif
  m_ScriptManager = new CScriptManager();
  m_ScriptManager->Initialize();
  //m_RenderableManager = new CRenderableObjectsManager();
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Animated Model Manager";
#endif
  m_AnimatedModelManager = new CAnimatedModelManager();
  m_AnimatedModelManager->Load(m_Config.AnimatedMeshPath);
  //Cargamos Technique pools
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Physics Manager";
#endif
  m_PhysicsManager = new CPhysicsManager();
  m_PhysicsManager->Init();
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating FSM Manager";
#endif
  m_FSMManager = new CFSMManager();
  m_FSMManager->Load(m_Config.FSMPath);
  //Cargando Layers
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Renderable Layers Manager";
#endif
  m_ScenePhisicsFile = m_Config.ScenePhisics;
  m_RenderableLayersManager = new CRenderableObjectsLayersManager();
  m_RenderableLayersManager->Load(m_Config.RenderablePath);
  m_RenderableLayersManager->Load(m_Config.LuaPath);
  //m_RenderableLayersManager->Load(m_Config.LuaPath);
  //como cargar LuaPath?
  //m_RenderableLayersManager->Load(m_Config.LuaPath);
  //m_RenderableManager->Load(m_Config.RenderablePath);
  //m_RenderableManager->Load(m_Config.LuaPath);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Light Manager";
#endif
  m_LightManager = new CLightManager();
  m_LightManager->Load(m_Config.LightsPath);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Camera Controller Manager";
#endif
  m_CameraController = new CCameraController();
  m_CameraController->Load(m_Config.CameraPath);
  /*m_RenderableManager->Load(m_Config.RenderablePath);
  m_ScriptManager = new CScriptManager();
  m_ScriptManager->Initialize();
  m_LightManager = new CLightManager();*/
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Cinematic Manager";
#endif
  m_CinematicManager = new CCinematicController();
  m_CinematicManager->Load(m_Config.CinematicPath);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Logger Render";
#endif
  m_LogRender = new CLogRender();
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Player Controller";
#endif
  m_PlayerController = new CPlayerController();
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Scene Render Command Manager";
#endif
  m_SceneRendererCommandManager = new CSceneRendererCommandManager();
  m_SceneRendererCommandManager->Load(m_Config.SceneRenderCommandsPath);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Trigger Manager";
#endif
  m_TriggerManager = new CTriggerManager();
  m_TriggerManager->LoadTriggers(m_Config.TriggersPath);
// m_WPManager = new CWPManager();
  //m_WPManager->Load("data//AI//Waypoints3.xml");
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Enemy Manager";
#endif
  m_EnemyManager = CEnemyManager::GetInstance();
  if (m_EnemyManager->Init(m_Config.EnemiesPath))
    m_EnemyManager->InitEnemies("enemies");
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Puzzle Manager";
#endif
  m_PuzzleManager  = new CPuzzleManager();
  m_PuzzleManager->Load(m_Config.PuzzlesPath);
  m_LuaLoadLevelFunc = m_Config.LuaLevelObjectsFunc;
  m_LevelPhisicsFile = m_Config.LevelPhisics;
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Particle Manager";
#endif
  m_ParticleManager = new CParticleManager();
  m_ParticleManager->Load(m_Config.ParticlesPath);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Billboard Manager";
#endif
  m_BillboardManager = new CBillboardManager();
  m_BillboardManager->Load(m_Config.BillboardPath);
  //m_SoundManager = new CSoundManager();
  //m_SoundManager->Init();
  //m_SoundManager->LoadSounds("./Data/sounds.xml");
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating WW Sound Manager";
#endif
  m_WWSoundManager = new CWWSoundManager();
  m_WWSoundManager->Init();
  m_WWSoundManager->Load(m_Config.SoundPath);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating GUI Manager";
#endif
  m_GuiManager = new CGUIManager(Vect2i(m_Config.Screen_Width, m_Config.Screen_Heigth));
  if (m_GuiManager->Init(m_Config.GUIPath))
    m_GuiManager->LoadGuiFiles(m_Config.GUIFolder);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Scprit Manager";
#endif
  m_ScriptManager->Load(m_Config.LuaPath);
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  std::cout << "Creating Collectible Manager";
#endif
  m_CollectibleManager = new CCollectibleManager();
  if (m_CollectibleManager->Load(m_Config.CollectiblesPath))
    m_CollectibleManager->InitCollectibles(m_CollectibleManager->GetCollectiblesLayerName());
#ifdef _DEBUG
  t.Update();
  std::cout << " ... " << t.GetElapsedTime() << " s" << std::endl;
  t1.Update();
  std::cout << "Total loading time " << t1.GetElapsedTime() << " s" << std::endl;
#endif
}

void CCore::DeInit() {
  CHECKED_DELETE(m_GraphicsManager);
  //CHECKED_DELETE(m_SoundManager);
  m_InputManager->Done();
  CHECKED_DELETE(m_InputManager);
  CHECKED_DELETE(m_LanguageManager);
  CHECKED_DELETE(m_FontManager);
  CHECKED_DELETE(m_ActionToInput);
  CHECKED_DELETE(m_AnimatedModelManager);
  //CHECKED_DELETE(m_RenderableManager);
  CHECKED_DELETE(m_RenderableLayersManager);
  CHECKED_DELETE(m_LightManager);
  CHECKED_DELETE(m_ScriptManager);
  CHECKED_DELETE(m_CameraController);
  CHECKED_DELETE(m_CinematicManager);
  CHECKED_DELETE(m_EffectManager);
  CHECKED_DELETE(m_TriggerManager);
  CHECKED_DELETE(m_FSMManager);
  CHECKED_DELETE(m_PlayerController);
  //CHECKED_DELETE(m_Process);
  CHECKED_DELETE(m_RenderableObjectTechniqueManager);
  CHECKED_DELETE(m_SceneRendererCommandManager);
  CHECKED_DELETE(m_TextureManager);
  CHECKED_DELETE(m_LogRender);
// CHECKED_DELETE(m_WPManager);
  CHECKED_DELETE(m_EnemyManager);
  CHECKED_DELETE(m_BillboardManager);
  CHECKED_DELETE(m_ParticleManager);
  CHECKED_DELETE(m_WWSoundManager);
  CHECKED_DELETE(m_PuzzleManager);
  CHECKED_DELETE(m_GuiManager);
  CHECKED_DELETE(m_PhysicsManager);
  CHECKED_DELETE(m_CollectibleManager);
  CHECKED_DELETE(m_StaticMeshManager);
  // m_PlatformsMap->Destroy();
}

CCore *CCore::GetInstance() {
  if (m_Instance == 0) {
    m_Instance = new CCore();
  }
  return m_Instance;
}

void CCore::Render() {
  //m_GraphicsManager->Render();
  // m_LightManager->Render(m_GraphicsManager);
  if (!m_GuiManager->GetIsPaused()) {
    m_PhysicsManager->DebugRender(m_GraphicsManager);
  }
  m_WWSoundManager->Render();
}

void CCore::Update(float dt) {
  m_ElapsedTime = dt;
//  m_ScriptManager->RunCode("update_pause()");
  if (!m_GuiManager->GetIsPaused()) {
    m_ActionToInput->Update();
    m_GraphicsManager->Update();
    m_PhysicsManager->Update(dt);
    m_InputManager->Update();
    m_LogRender->Update(dt);
    m_TriggerManager->Update(dt);
    m_EnemyManager->Update(dt);
    m_ParticleManager->Update(dt);
    m_BillboardManager->Update(dt);
    m_PuzzleManager->Update(dt);
    m_WWSoundManager->Render();
    m_GuiManager->Update(dt);
    m_CollectibleManager->Update(dt);
    m_CinematicManager->Update(dt);
    m_CameraController->UpdateCinematicCameras(dt);
  } else {
    m_ActionToInput->Update();
    m_InputManager->Update();
    m_WWSoundManager->Render();
    m_GuiManager->Update(dt);
  }
}


void CCore::Trace(const std::string &msg ) {
  std::cout << msg << std::endl;
}

void CCore::SetLightsToPlatforms(std::string layer) {
  CRenderableObjectsManager *l_Rendm = RENDLM->GetRenderableObjectsManagerByStr(layer);
  for (int i = 0; i < l_Rendm->GetSize(); ++i) {
    CPolyPlatform *l_Poly = (CPolyPlatform *)l_Rendm->GetResourcesVector()[i];
    CLight *l_Light = LIGHTM->GetResource(l_Poly->getLightName());
    l_Poly->SetLight(l_Light);
    l_Poly->setLightOriginalPosition(l_Light->GetPosition());
  }
}

//#ifdef _PARTICLEVIEWER
void CCore::CopyToClipboard(std::string toCopy) {
  const char *output = toCopy.c_str();
  const size_t len = strlen(output) + 1;
  HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
  memcpy(GlobalLock(hMem), output, len);
  GlobalUnlock(hMem);
  OpenClipboard(0);
  EmptyClipboard();
  SetClipboardData(CF_TEXT, hMem);
  CloseClipboard();
}
//#endif