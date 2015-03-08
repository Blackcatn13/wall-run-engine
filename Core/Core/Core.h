#ifndef CCORE_H
#define CCORE_H

#include <Windows.h>
#include "Utils\Defines.h"
#include "EngineDefs.h"
#include "Camera\CameraController.h"
#include "Utils\MapManager.h"

class CLanguageManager;
class CGraphicsManager;
class CSoundManager;
class CInputManager;
class CFontManager;
class CActionToInput;
class CStaticMeshManager;
class CRenderableObjectsManager;
class CAnimatedModelManager;
class CTextureManager;
class CScriptManager;
class CLightManager;
class CCinematicController;
class CCameraController;
class CEffectManager;
class CRenderableObjectTechniqueManager;
class CPhysicsManager;
class CRenderableObjectsLayersManager;
class CSceneRendererCommandManager;
class CProcess;
class CTriggerManager;
class CPlayerController;
class CLogRender;
class CStaticPlatform;

#define CCORE			CCore::GetInstance()
#define GRAPHM			CCore::GetInstance()->GetGraphicsManager()
#define SOUNDM			CCore::GetInstance()->GetSoundManager()
#define INPUTM			CCore::GetInstance()->GetInputManager()
#define LANGM			CCore::GetInstance()->GetLanguageManager()
#define FONTM			CCore::GetInstance()->GetFontManager()
#define ACT2IN			CCore::GetInstance()->GetActionToInput()
#define SMESHM			CCore::GetInstance()->GetStaticMeshManager()
#define RENDM			CCore::GetInstance()->GetRenderableManager()
#define TEXTM			CCore::GetInstance()->GetTextureManager()
#define ANIMAN			CCore::GetInstance()->GetAnimatedModelManager()
#define SCRIPTM			CCore::GetInstance()->GetScriptManager()
#define LIGHTM			CCore::GetInstance()->GetLightManager()
#define EFFECTM			CCore::GetInstance()->GetEffectManager()
#define RENDTECHM		CCore::GetInstance()->GetRenderableObjectTechniqueManager()
#define CAMCONTM		CCore::GetInstance()->GetCameraController()
#define PHYSXM			CCore::GetInstance()->GetPhysicsManager()
#define RENDLM			CCore::GetInstance()->GetRenderableObjectsLayersManager()
#define SCENRENDCOMM	CCore::GetInstance()->GetSceneRendererCommandManager()
#define TRIGGM			CCore::GetInstance()->GetTriggerManager()
#define PLAYC			CCore::GetInstance()->GetPlayerController()
#define LOGRNDR         CCore::GetInstance()->GetLogRender()


class CCore
{
protected:
    static CCore* m_Instance;
    CCore();
private:
    CGraphicsManager*		            m_GraphicsManager;
    CSoundManager*			            m_SoundManager;
    CInputManager*                      m_InputManager;
    CLanguageManager*		            m_LanguageManager;
    HWND					            m_Handler;
    CONFIG_INFO				            m_Config;
    CFontManager*			            m_FontManager;
    CActionToInput*                     m_ActionToInput;
    CStaticMeshManager*                 m_StaticMeshManager;
    CRenderableObjectsManager*          m_RenderableManager;
    CAnimatedModelManager*              m_AnimatedModelManager;
    CTextureManager*                    m_TextureManager;
    CScriptManager*			            m_ScriptManager;
    CLightManager*			            m_LightManager;
    CCinematicController*	            m_CinematicManager;
    CCameraController*		            m_CameraController;
    CEffectManager*			            m_EffectManager;
    CRenderableObjectTechniqueManager*  m_RenderableObjectTechniqueManager;
    CPhysicsManager*					m_PhysicsManager;
    CRenderableObjectsLayersManager*	m_RenderableLayersManager;
    CSceneRendererCommandManager*		m_SceneRendererCommandManager;
    CProcess*							m_Process;
    CTriggerManager*					m_TriggerManager;
    CPlayerController*					m_PlayerController;
    CLogRender*                         m_LogRender;
    std::string							m_LuaLoadLevelFunc;
    std::string							m_LuaCameras;
    // CMapManager<CStaticPlatform>	*		m_PlatformsMap;
public:
    ~CCore();
    void Update ( float dt );
    void Trace(const std::string & msg );
    void Render();
    void Init ( HWND handler );
    void DeInit();
    static CCore* GetInstance();
    CGraphicsManager* GetGraphicsManager()
    {
        return m_GraphicsManager;
    }
    CSoundManager* GetSoundManager()
    {
        return m_SoundManager;
    }
    CInputManager* GetInputManager()
    {
        return m_InputManager;
    }
    CLanguageManager* GetLanguageManager()
    {
        return m_LanguageManager;
    }
    CFontManager* GetFontManager()
    {
        return m_FontManager;
    }
    GET_SET ( HWND, Handler );
    GET_SET_REF ( CONFIG_INFO, Config );
    CActionToInput* GetActionToInput()
    {
        return m_ActionToInput;
    }
    CStaticMeshManager* GetStaticMeshManager()
    {
        return m_StaticMeshManager;
    }
    CRenderableObjectsManager* GetRenderableManager()
    {
        return m_RenderableManager;
    }
    CAnimatedModelManager* GetAnimatedModelManager()
    {
        return m_AnimatedModelManager;
    }
    CTextureManager* GetTextureManager()
    {
        return m_TextureManager;
    }
    CScriptManager * GetScriptManager()
    {
        return m_ScriptManager;
    }
    CLightManager * GetLightManager()
    {
        return m_LightManager;
    }

    CCinematicController * GetCinematicController()
    {
        return m_CinematicManager;
    }
    CPhysicsManager * GetPhysicsManager()
    {
        return m_PhysicsManager;
    }
    CCameraController * GetCameraController()
    {
        return m_CameraController;
    }
    CEffectManager * GetEffectManager()
    {
        return m_EffectManager;
    }
    CRenderableObjectTechniqueManager * GetRenderableObjectTechniqueManager()
    {
        return m_RenderableObjectTechniqueManager;
    }
    CRenderableObjectsLayersManager * GetRenderableObjectsLayersManager()
    {
        return m_RenderableLayersManager;
    }
    void SetCameraController(CCameraController * cameraController)
    {
        m_CameraController = cameraController;
    }
    CSceneRendererCommandManager * GetSceneRendererCommandManager()
    {
        return m_SceneRendererCommandManager;
    }

    CProcess * GetProcess()
    {
        return m_Process;
    }

    void SetProcess(CProcess * process)
    {
        m_Process = process;
    }
    CTriggerManager * GetTriggerManager()
    {
        return m_TriggerManager;
    }
    CPlayerController * GetPlayerController()
    {
        return m_PlayerController;
    }

    CLogRender* GetLogRender()
    {
        return m_LogRender;
    }

    GET_SET(std::string, LuaLoadLevelFunc)

    /*  CMapManager<CStaticPlatform>	*GetPlatformsMap()
      {
          return m_PlatformsMap;
      }*/
};


#endif
