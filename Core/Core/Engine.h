#ifndef CENGINE_H
#define CENGINE_H

#include <string>
#include <Windows.h>
#include "EngineDefs.h"
#include "Core_Utils/Timer.h"

class CCore;
class CProcess;

class CEngine
{
private:
    CProcess		*m_Process;
    CCore			*m_Core;
    std::string		m_ConfFile;
    CONFIG_INFO     m_Conf_info;
    CTimer			m_Timer;
    //CLogRender		*m_LogRender;
public:
    CEngine(void): m_Timer(50) {}
    ~CEngine(void);
    void Update();
    void Render();
    void Init(CProcess *p, std::string ConfFile, HWND handler);
    void DeInit();
    void ParseConfFile();
    void RenderScene();
};


#endif
