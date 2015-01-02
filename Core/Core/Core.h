#ifndef CCORE_H
#define CCORE_H

#include <Windows.h>
#include "Utils\Defines.h"
#include "EngineDefs.h"


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

#define CCORE   CCore::GetInstance()
#define GRAPHM  CCore::GetInstance()->GetGraphicsManager()
#define SOUNDM  CCore::GetInstance()->GetSoundManager()
#define INPUTM  CCore::GetInstance()->GetInputManager()
#define LANGM   CCore::GetInstance()->GetLanguageManager()
#define FONTM   CCore::GetInstance()->GetFontManager()
#define ACT2IN  CCore::GetInstance()->GetActionToInput()
#define SMESHM  CCore::GetInstance()->GetStaticMeshManager()
#define RENDM   CCore::GetInstance()->GetRenderableManager()
#define TEXTM   CCore::GetInstance()->GetTextureManager()
#define ANIMAN	CCore::GetInstance()->GetAnimatedModelManager()
#define SCRIPTM CCore::GetInstance()->GetScriptManager()
#define LIGHTM CCore::GetInstance()->GetLightManager()

class CCore
{
protected:
    static CCore* m_Instance;
    CCore();
private:
    CGraphicsManager*		m_GraphicsManager;
    CSoundManager*			m_SoundManager;
    CInputManager*          m_InputManager;
    CLanguageManager*		m_LanguageManager;
    HWND					m_Handler;
    CONFIG_INFO				m_Config;
    CFontManager*			m_FontManager;
    CActionToInput*         m_ActionToInput;
    CStaticMeshManager*     m_StaticMeshManager;
    CRenderableObjectsManager* m_RenderableManager;
	CAnimatedModelManager*  m_AnimatedModelManager;
    CTextureManager*        m_TextureManager;
	CScriptManager*			m_ScriptManager;
	CLightManager*			m_LightManager;
public:
    ~CCore();
    void Update ( float dt );
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
};


#endif
