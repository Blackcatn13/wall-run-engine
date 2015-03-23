#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include <string>
#include "Utils\TemplatedVectorMapManager.h"

class CParticleEmitter;
class CGraphicsManager;

class CParticleManager : public CTemplatedVectorMapManager<CParticleEmitter> {
 private:
  std::string m_fileName;
  void Load();

 public:
  CParticleManager();
  ~CParticleManager();
  void Load(std::string file);
  void Reload();
  void Update(float ElapsedTime);
  void Render(CGraphicsManager *RM);
};

#endif