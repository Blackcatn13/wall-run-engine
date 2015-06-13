#ifndef CPARTICLE_PROCESS_H
#define CPARTICLE_PROCESS_H

#include "Core/Process.h"
#include "Math/Matrix44.h"
#include "Renderable\RenderableObject.h"

class CCameraController;
class CObject3D;
class CFPSCamera;
class CThPSCamera;

class CParticleProcess : public CProcess {
 private:

 public:
  CParticleProcess(void);
  ~CParticleProcess(void);
  void Update(float dt);
  void Render();
  void Init();
  void DeInit();
  Vect2i RenderDebugInfo(bool render/*, float dt*/);
};

#endif
