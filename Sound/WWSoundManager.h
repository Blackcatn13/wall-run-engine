
//----------------------------------------------------------------------------------
// CSoundManager class
// Author: Enric Vergara
//
// Description:
// This secures availability of the necessary sounds functions.
// It internally uses the OpenAL library.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_WW_SOUND_MANAGER_H_
#define INC_WW_SOUND_MANAGER_H_

#include <string>

#include "AkDefaultIOHookBlocking.h"

class CWWSoundManager {
 public:
  //----Init and End protocols------------------------------------
  CWWSoundManager() {}
  virtual ~CWWSoundManager() {Done();}
  bool	Init		();
  void	Done		();
  CAkDefaultIOHookBlocking *m_lowLevelIO;
  void Render();
  void Load(std::string file);
 private:
  std::string m_fileName;
};
#endif // INC_SOUND_MANAGER_H_
