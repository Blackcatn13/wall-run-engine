#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
#include "Utils\MapManager.h"
#include <vector>
#include "Utils\Defines.h"

class CTrigger;

class CPuzzle {
 public:
  CPuzzle();
  CPuzzle(std::string LuaCode, int MinActivatedSwitches );
  ~CPuzzle();
  void Update(float dt);
// GET_SET(std::vector<CTrigger *>, Switches)
  std::vector<CTrigger *> &getSwitches() {
    return m_Switches;
  }

  GET_SET(std::string, SceneElement)
  GET_SET(std::string, LuaCode)
  GET_SET(int, MinActivatedSwitches)
  GET_SET(int, ActivatedSwitches)
  GET_SET(bool, Done)
 private:
  std::vector<CTrigger *> m_Switches;
  std::string m_LuaCode;
  int m_MinActivatedSwitches;
  int m_ActivatedSwitches;
  bool m_Done;
  std::string m_SceneElement;
};

#endif