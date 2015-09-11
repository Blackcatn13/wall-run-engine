#ifndef BILLBOARD_MANAGER_H
#define BILLBOARD_MANAGER_H

#include <string>
#include "Utils\TemplatedVectorMapManager.h"
#include "Billboard.h"

class CParticleEmitter;
class CGraphicsManager;

class CBillboardManager : public CTemplatedVectorMapManager<CBillboard> {
 private:
  std::string m_fileName;
  void Load();

 public:
  CBillboardManager();
  ~CBillboardManager();
  void Load(std::string file);
  void Reload();
  void Update(float ElapsedTime);
  void Render(CGraphicsManager *RM);
  void Render(CGraphicsManager *RM, const std::string &name);
};

#endif