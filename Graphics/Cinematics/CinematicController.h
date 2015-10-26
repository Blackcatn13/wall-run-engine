#pragma once

#ifndef CINEMATIC_CONTROLLER_H
#define CINEMATIC_CONTROLLER_H

#include "Cinematic.h"
#include "CinematicElement\CinematicElement.h"
#include <vector>
#include <map>
#include <string>

class CCinematicController {
 private:
  std::string m_FileName;
  bool m_executing;
  std::map<std::string, std::vector<CCinematicElement *> > m_cinematics;
  std::vector<CCinematicElement *> m_currentCinematic;
  int m_lastElement;
  void ExecuteAtEnd();
 public:
  CCinematicController();
  ~CCinematicController();
  bool Load(const std::string &FileName);
  bool Reload();
  bool Reload(const std::string &FileName);
  void Update(float ElapsedTime);
  void Execute(const std::string &cinematic);
  void Restart(const std::string &cinematic);
  void RestartAllCinematics();
  bool IsExecuting() {
    return m_executing;
  }
};

#endif