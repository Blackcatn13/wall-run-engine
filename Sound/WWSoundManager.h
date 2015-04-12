
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
#include <map>
#include "AkDefaultIOHookBlocking.h"
#include "Math\Vector3.h"
#include <vector>

struct EventInfo {
  std::string Event;
  AkGameObjectID GameObjectID;
  AkPlayingID PlayindID;
};

class CWWSoundManager {
 public:
  //----Init and End protocols------------------------------------
  CWWSoundManager();
  virtual ~CWWSoundManager() {Done();}
  bool	Init		();
  void	Done		();
  CAkDefaultIOHookBlocking *m_lowLevelIO;
  void Render();
  void Load(std::string file);
  void SetListenerPosition(Vect3f pos);
  void PlayEvent(std::string eventName, std::string GameObject);
 private:
  std::string m_fileName;
  std::map<std::string, AkGameObjectID> m_GameObjects;
  AkGameObjectID m_LastId;
  std::vector<EventInfo *> m_events;
};
#endif // INC_SOUND_MANAGER_H_
