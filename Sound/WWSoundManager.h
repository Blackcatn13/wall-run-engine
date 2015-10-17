
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
  void Load(const std::string &file);
  void Reload();
  void SetListenerPosition(Vect3f pos, Vect3f direction = Vect3f(1, 0, 0), Vect3f up = Vect3f(0, 1, 0));
  void PlayEvent(const std::string &eventName, const std::string &GameObject);
  void SetSwitch(const std::string &group, const std::string &switch_, const std::string &gameObject);
  void SetTrigger(const std::string &trigger, const std::string &gameObject);
  void RegisterGameObject(const std::string &gameObject);
  void UnregisterGameObject(const std::string &gameObject);
  void SetState(const std::string &group, const std::string &state);
  void StopAllPlayingEvents();
  void SetGameObjectPosition(const std::string &gameObject, Vect3f pos, Vect3f direction = Vect3f(1, 0, 0));
 private:
  std::string m_fileName;
  std::map<std::string, AkGameObjectID> m_GameObjects;
  AkGameObjectID m_LastId;
  std::vector<EventInfo *> m_events;
};
#endif // INC_SOUND_MANAGER_H_
