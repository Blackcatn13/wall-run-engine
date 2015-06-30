#include <string>

#include "Engine.h"
#include "Process.h"
#include "Core.h"
#include "Utils/Defines.h"
#include "XML/XMLTreeNode.h"
#include "GraphicsManager.h"
#include "Language/LanguageManager.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "Utils/Logger.h"
#include "Core_Utils/LogRender.h"
#include "Texture\TextureManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Effects\EffectManager.h"
#include "Lights\LightManager.h"
#include "ScriptManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "RenderableCommands\SceneRendererCommandManager.h"
#include "Core_Utils/MemLeaks.h"
#include "AI\PuzzleManager.h"


#define APPLICATION_NAME	"VIDEOGAME"

CEngine::CEngine() :
  m_Timer(50),
  m_Process(NULL),
  m_Core(NULL),
  m_Conf_info(),
  m_speed(1)
{}

CEngine::~CEngine(void) {
  DeInit();
}

void CEngine::Init(CProcess *p, HWND handler) {
  m_Process = p;
  m_Core = CCORE;
  m_Process->setexitGame(false);
  m_Core->SetProcess(m_Process);
  m_Core->setConfig (m_Conf_info);
  m_Core->setHandler (handler);
  m_Core->Init (handler);
  //m_LogRender = new CLogRender();
  //LOGGER->SetCapacity(500);
  //m_LogRender->SetLinePerPage(30);
  //m_Core->GetLanguageManager()->SetXmlFile(".\\Data\\fonts.xml");
  //m_Core->GetLanguageManager()->LoadXMLs();
}

void CEngine::DeInit() {
  CHECKED_DELETE (m_Process);
  CHECKED_DELETE (m_Core);
  //CHECKED_DELETE (m_LogRender);
  CLogger::DeInit();
}

void CEngine::Update() {
  m_Timer.Update();
  float dt = m_Timer.GetElapsedTime() * m_speed;
  m_Core->Update (dt);
  m_Process->Update (dt);
  if (ACT2IN->DoAction("LoadLanguages"))
    LANGM->LoadXMLs();
  if (ACT2IN->DoAction("Logger"))
    m_Process->SetPrintInfo(!m_Process->getPrintInfo());
  if (ACT2IN->DoAction("LoggerPageUP"))
    LOGRNDR->PageUp();
  if (ACT2IN->DoAction("LoggerPageDown"))
    LOGRNDR->PageDown();
  if (ACT2IN->DoAction("LoggerUP") || ACT2IN->DoAction("LoggerUPCont"))
    LOGRNDR->PrevLine();
  if (ACT2IN->DoAction("LoggerDown") || ACT2IN->DoAction("LoggerDownCont"))
    LOGRNDR->NextLine();
  if (ACT2IN->DoAction("ReloadActions"))
    ACT2IN->ReloadXML();
  if (ACT2IN->DoAction("Save2File"))
    LOGGER->SaveLogsInFile();
  if (ACT2IN->DoAction("LoggerOpen"))
    LOGRNDR->SetVisible(!LOGRNDR->GetVisible());
  if (ACT2IN->DoAction("ReloadTextures"))
    TEXTM->Reload();
  if (ACT2IN->DoAction("ReloadMeshes"))
    RENDLM->Reload();
  if (ACT2IN->DoAction("ReloadEffects"))
    EFFECTM->Reload();
  if (ACT2IN->DoAction("ReloadLights"))
    LIGHTM->Reload();
  if (ACT2IN->DoAction("ReloadScripts"))
    SCRIPTM->Reload();
  if (ACT2IN->DoAction("ReloadRenderCommands"))
    SCENRENDCOMM->Reload();
  if (ACT2IN->DoAction("RenderDebugInfo"))
    m_Process->SetPrintInfo(!m_Process->getPrintInfo());
  if (ACT2IN->DoAction("ReloadPuzzles"))
    PUZZLEM->Reload();
  if (ACT2IN->DoAction("AddSlowMotion"))
    m_speed -= 0.1f;
  if (ACT2IN->DoAction("AddFastMotion"))
    m_speed += 0.1f;
  if (ACT2IN->DoAction("ResetMotion"))
    m_speed = 1;

  //m_LogRender->Update(m_Timer.GetElapsedTime());
}

void CEngine::Render() {
  //CGraphicsManager* gm = m_Core->GetGraphicsManager();
  //CCamera* camera = m_Process->GetCamera();
  // gm->BeginRendering();
  //  gm->SetupMatrices(m_Process->GetCamera());
  //gm->ClearSceneCommand(true, true, true);
  //gm->BeginRenderCommand();
  //  RenderScene();
  m_Process->Render();
  /*gm->DisableZBuffering();
  gm->EnableAlphaBlend();*/
// m_Process->RenderDebugInfo(true, m_Timer.GetElapsedTime());
  /* m_LogRender->Render(gm, FONTM);
   gm->DisableAlphaBlend();
   gm->EnableZBuffering();*/
  // gm->EndRendering();
}

void CEngine::RenderScene() {
  m_Core->Render();
  m_Process->Render();
}

void CEngine::ParseConfFile(std::string file) {
  m_ConfFile = file;
  CXMLTreeNode File;
  if (!File.LoadFile(m_ConfFile.c_str())) {
    // log error
  } else {
    CXMLTreeNode n = File["Config"];
    if (n.Exists()) {
      int count = n.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode nodeChild = n(i);
        if (!nodeChild.IsComment()) {
          std::string name = nodeChild.GetName();
          if (name == "ScreenResolution") {
            m_Conf_info.Screen_Width = nodeChild.GetIntProperty("width");
            m_Conf_info.Screen_Heigth = nodeChild.GetIntProperty("height");
          } else if (name == "WindowsPosition") {
            m_Conf_info.Win_posX = nodeChild.GetIntProperty("posX");
            m_Conf_info.Win_posY = nodeChild.GetIntProperty("posY");
          } else if (name == "Rendermode") {
            m_Conf_info.FullScreen = nodeChild.GetBoolProperty("fullscreenMode", false, false);
          } else if (name == "Mouse") {
            m_Conf_info.Mouse_Exclusive = nodeChild.GetBoolProperty("exclusiveModeinMouse", false, false);
            m_Conf_info.Mouse_Draw = nodeChild.GetBoolProperty("drawPointerMouse", false, false);
          } else if (name == "Languages") {
            m_Conf_info.LanguagesPath = std::vector<std::string>();
            m_Conf_info.CurrentLanguage = nodeChild.GetPszISOProperty("current", "catalan", false);
            int languages = n (i).GetNumChildren();
            for (int j = 0; j < languages; ++j)
              m_Conf_info.LanguagesPath.push_back(nodeChild(j).GetPszISOProperty("XMLfile", "./Data/catalan.xml", false));
          } else if (name == "Fonts") {
            m_Conf_info.FontsPath = nodeChild.GetPszISOProperty("fontsXML", "./Data/fonts/fonts.xml", false);
          } else if (name == "Actions") {
            m_Conf_info.ActionsPath = nodeChild.GetPszISOProperty("actionsXML", "./Data/Actions.xml", false);
          } else if (name == "Meshes") {
            m_Conf_info.MeshesPath = nodeChild.GetPszISOProperty("meshesXML", "", false);
          } else if (name == "Renderable") {
            m_Conf_info.RenderablePath = nodeChild.GetPszISOProperty("renderXML", "", false);
          } else if (name == "Lights") {
            m_Conf_info.LightsPath = nodeChild.GetPszISOProperty("lightsXML", "", false);
          } else if (name == "Cameras") {
            m_Conf_info.CameraPath = nodeChild.GetPszISOProperty("CamerasLvl", "", false);
          } else if (name == "Cinematic") {
            m_Conf_info.CinematicPath = nodeChild.GetPszISOProperty("CinematicLvl", "", false);
          } else if (name == "Lua") {
            m_Conf_info.LuaPath = nodeChild.GetPszISOProperty("LuaFile", "", false);
          } else if (name == "Effect") {
            m_Conf_info.EffectPath = nodeChild.GetPszISOProperty("EffectXML", "", false);
          } else if (name == "RendererCommands") {
            m_Conf_info.SceneRenderCommandsPath = nodeChild.GetPszISOProperty("rendererCommandsXML", "", false);
          } else if (name == "PoolRenderableObjects") {
            m_Conf_info.PoolRenderableObjects = nodeChild.GetPszISOProperty("poolRenderableObjects", "", false);
          } else if (name == "AnimatedModels") {
            m_Conf_info.AnimatedMeshPath = nodeChild.GetPszISOProperty("modelsXML", "", false);
          } else if (name == "LoadLevel") {
            m_Conf_info.LuaLevelObjectsFunc = nodeChild.GetPszISOProperty("LuaInitLevelFunc", "init_level()", false);
            m_Conf_info.LuaLevelCamFunc = nodeChild.GetPszISOProperty("LuaInitCamerasFunc", "", false);
          } else if (name == "LevelPhisicsFile") {
            m_Conf_info.LevelPhisics = nodeChild.GetPszISOProperty("PhisicsFile", "", false);
          } else if (name == "Triggers") {
            m_Conf_info.TriggersPath = nodeChild.GetPszISOProperty("TriggersFile", "", false);
          } else if (name == "Puzzles") {
            m_Conf_info.PuzzlesPath = nodeChild.GetPszISOProperty("PuzzlesFile", "", false);
          } else if (name == "Collectibles") {
            m_Conf_info.CollectiblesPath = nodeChild.GetPszISOProperty("CollectiblesFile", "", false);
          } else if (name == "Enemies") {
            m_Conf_info.EnemiesPath = nodeChild.GetPszISOProperty("enemiesXML", "", false);
          } else if (name == "Sound") {
            m_Conf_info.SoundPath = nodeChild.GetPszISOProperty("soundXML", "" , false);
          } else if (name == "GUI") {
            m_Conf_info.GUIPath = nodeChild.GetPszISOProperty("initGuiXML", "", false);
            m_Conf_info.GUIFolder = nodeChild.GetPszISOProperty("dataFolder", "", false);
          } else if (name == "Particles") {
            m_Conf_info.ParticlesPath = nodeChild.GetPszISOProperty("particlesXML", "", false);
          } else if (name == "FSM") {
            m_Conf_info.FSMPath = nodeChild.GetPszISOProperty("FsmXML", "", false);
          } else if (name == "Billboard") {
            m_Conf_info.BillboardPath = nodeChild.GetPszISOProperty("billboardXML", "");
          }
        }
      }
    }
    //<ScreenResolution width="800" height="600"/>
    //  <!--ScreenResolution width="1280" height="1024"/-->
    //  <WindowsPosition posX="100" posY="100"/>
    //  <Rendermode fullscreenMode="false"/>
    //  <!--Rendermode fullscreenMode="true"/-->
  }
}
