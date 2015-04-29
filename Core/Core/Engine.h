#ifndef CENGINE_H
#define CENGINE_H

#include <string>
#include <Windows.h>
#include "EngineDefs.h"
#include "Core_Utils/Timer.h"

class CCore;
class CProcess;

class CEngine {
 private:
  CProcess		  *m_Process;
  CCore			    *m_Core;
  std::string		m_ConfFile;
  CONFIG_INFO   m_Conf_info;
  CTimer			  m_Timer;
  float         m_speed;
 public:
  CEngine(void);
  ~CEngine(void);
  void Update();
  void Render();
  void Init(CProcess *p, HWND handler);
  void DeInit();
  void ParseConfFile(std::string file);
  void RenderScene();
  CONFIG_INFO getConfig() {
    return m_Conf_info;
  }
};


#endif
