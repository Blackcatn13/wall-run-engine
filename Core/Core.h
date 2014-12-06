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
};


#endif
