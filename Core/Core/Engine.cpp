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

CEngine::~CEngine(void)
{
    DeInit();
}

void CEngine::Init(CProcess *p, std::string ConfFile, HWND handler)
{
    m_Process = p;
    m_Core = CCORE;
	m_Core->SetProcess(m_Process);
    m_ConfFile = ConfFile;
    ParseConfFile();
    m_Core->setConfig (m_Conf_info);
    m_Core->setHandler (handler);
    m_Core->Init (handler);
    m_LogRender = new CLogRender();
    LOGGER->SetCapacity(500);
    m_LogRender->SetLinePerPage(30);
    //m_Core->GetLanguageManager()->SetXmlFile(".\\Data\\fonts.xml");
    //m_Core->GetLanguageManager()->LoadXMLs();
}

void CEngine::DeInit()
{
    CHECKED_DELETE (m_Process);
    CHECKED_DELETE (m_Core);
    CHECKED_DELETE (m_LogRender);
    CLogger::DeInit();
}

void CEngine::Update()
{
    m_Timer.Update();
    m_Core->Update (m_Timer.GetElapsedTime());
    m_Process->Update (m_Timer.GetElapsedTime());
    if (m_Core->GetActionToInput()->DoAction("LoadLanguages"))
        m_Core->GetLanguageManager()->LoadXMLs();
    if (m_Core->GetActionToInput()->DoAction("Logger"))
        m_Process->SetPrintInfo(!m_Process->getPrintInfo());
    if (m_Core->GetActionToInput()->DoAction("ReloadActions"))
        m_Core->GetActionToInput()->ReloadXML();
    if (m_Core->GetActionToInput()->DoAction("Save2File"))
        LOGGER->SaveLogsInFile();
    if (m_Core->GetActionToInput()->DoAction("LoggerOpen"))
        m_LogRender->SetVisible(!m_LogRender->GetVisible());
    if (m_Core->GetActionToInput()->DoAction("ReloadTextures"))
        TEXTM->Reload();
    if (m_Core->GetActionToInput()->DoAction("ReloadMeshes")) 
		RENDLM->Reload();
    if (ACT2IN->DoAction("ReloadEffects"))
        EFFECTM->Reload();
    if (ACT2IN->DoAction("ReloadLights"))
        LIGHTM->Reload();
    if (ACT2IN->DoAction("ReloadScripts"))
        SCRIPTM->Reload();
	if (ACT2IN->DoAction("ReloadRenderCommands"))
		SCENRENDCOMM->Reload();
    m_LogRender->Update(m_Timer.GetElapsedTime());
}

void CEngine::Render()
{
    CGraphicsManager* gm = m_Core->GetGraphicsManager();
    //CCamera* camera = m_Process->GetCamera();
   // gm->BeginRendering();
  //  gm->SetupMatrices(m_Process->GetCamera());

	//gm->ClearSceneCommand(true, true, true);
	//gm->BeginRenderCommand();
  //  RenderScene();
	m_Process->Render();
	gm->DisbaleZBuffering();
    gm->EnableAlphaBlend();
    m_Process->RenderDebugInfo(true, m_Timer.GetElapsedTime());
    m_LogRender->Render(gm, FONTM);
    gm->DisbaleAlphaBlend();
    gm->EnableZBuffering();
   // gm->EndRendering();
}

void CEngine::RenderScene()
{
    m_Core->Render();
    m_Process->Render();
	
	
}

void CEngine::ParseConfFile()
{
    CXMLTreeNode File;
    if (!File.LoadFile(m_ConfFile.c_str())) {
        // log error
    } else {
        CXMLTreeNode n = File["Config"];
        if (n.Exists()) {
            int count = n.GetNumChildren();
            for (int i = 0; i < count; ++i) {
                if (!n(i).IsComment()) {
                    std::string name = n(i).GetName();
                    if (name == "ScreenResolution") {
                        m_Conf_info.Screen_Width = n(i).GetIntProperty("width");
                        m_Conf_info.Screen_Heigth = n(i).GetIntProperty("height");
                    } else if (name == "WindowsPosition") {
                        m_Conf_info.Win_posX = n(i).GetIntProperty("posX");
                        m_Conf_info.Win_posY = n(i).GetIntProperty("posY");
                    } else if (name == "Rendermode") {
                        m_Conf_info.FullScreen = n(i).GetBoolProperty("fullscreenMode");
                    } else if (name == "Mouse") {
                        m_Conf_info.Mouse_Exclusive = n(i).GetBoolProperty("exclusiveModeinMouse");
                        m_Conf_info.Mouse_Draw = n(i).GetBoolProperty("drawPointerMouse");
                    } else if (name == "Languages") {
                        m_Conf_info.LanguagesPath = std::vector<std::string>();
                        m_Conf_info.CurrentLanguage = n(i).GetPszISOProperty("current", "catalan");
                        int languages = n (i).GetNumChildren();
                        for (int j = 0; j < languages; ++j)
                            m_Conf_info.LanguagesPath.push_back(n(i)(j).GetPszISOProperty("XMLfile", "./Data/catalan.xml"));
                    } else if (name == "Fonts") {
                        m_Conf_info.FontsPath = n(i).GetPszISOProperty("fonstXML", "./Data/fonts/fonts.xml");
                    } else if (name == "Actions") {
                        m_Conf_info.ActionsPath = n(i).GetPszISOProperty("actionsXML", "./Data/Actions.xml");
                    } else if (name == "Meshes") {
                        m_Conf_info.MeshesPath = n(i).GetPszISOProperty("meshesXML", "./Data/level1/static_meshes.xml");
                    } else if (name == "Renderable") {
                        m_Conf_info.RenderablePath = n(i).GetPszISOProperty("renderXML", "./Data/level1/renderable_objects.xml");
                    } else if (name == "Lights") {
                        m_Conf_info.LightsPath = n(i).GetPszISOProperty("lightsXML", "./Data/level1/lights.xml");
                    } else if (name == "Cameras") {
                        m_Conf_info.CameraPath = n(i).GetPszISOProperty("CamerasLvl", "");
                    } else if (name == "Cinematic") {
                        m_Conf_info.CinematicPath = n(i).GetPszISOProperty("CinematicLvl", "");
                    } else if (name == "Lua") {
                        m_Conf_info.LuaPath = n(i).GetPszISOProperty("LuaFile", "");
                    } else if (name == "Effect") {
                        m_Conf_info.EffectPath = n(i).GetPszISOProperty("EffectXML", "");
                    }else if (name == "RendererCommands") {
						m_Conf_info.SceneRenderCommandsPath = n(i).GetPszISOProperty("rendererCommandsXML", "");
                    }else if (name == "PoolRenderableObjects") {
						m_Conf_info.PoolRenderableObjects = n(i).GetPszISOProperty("poolRenderableObjects", ".\\Data\\level1\\pool_renderable_objects.xml");
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
