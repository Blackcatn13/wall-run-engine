#ifndef PUZZLE_MANAGER_H
#define PUZZLE_MANAGER_H

#include <string>
#include "Utils\MapManager.h"
class CPuzzle;

class CPuzzleManager : public CMapManager<CPuzzle> {
 public:
  CPuzzleManager();
  ~CPuzzleManager();
  void Load(std::string file);
  void Reload();
  void Update(float dt);
  void ResetPuzzleTriggers(std::string puzzleName);
 private:
  std::string m_fileName;

};

#endif