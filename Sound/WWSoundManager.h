
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

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>

#include "AkSoundEngineExports.h"
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
};
#endif // INC_SOUND_MANAGER_H_
