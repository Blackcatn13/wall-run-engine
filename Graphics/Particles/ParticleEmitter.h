#ifndef CPARTICLE_EMITTER_H
#define CPARTICLE_EMITTER_H

#include <string>
#include "Object\Object3D.h"
#include "Utils\Defines.h"
#include <vector>
#include "Math\Color.h"
#include "Math\Vector3.h"
#include "Particles/RecyclingArray.h"

class CXMLTreeNode;
class CGraphicsManager;
class CParticle;

enum EmitterType {EMITTER_ESF, EMITTER_PLANE};

class CParticleEmitter : public CObject3D {
 public:
  CParticleEmitter(CXMLTreeNode  &node);
  ~CParticleEmitter();
  void Render(CGraphicsManager *RM);
  void Update(float dt);
 private:
  void PopulateParticle(CParticle *p);
  EmitterType                  m_Type;
  Vect3f		                  m_vPos;
  Vect3f		                  m_vSpawnDir1;
  Vect3f		                  m_vSpawnDir2;
  float			                  m_CurrentTime;
  float			                  m_TimeNextParticle;
  float                       m_ExcedentTime;

  float			                  m_MinSpeed;
  float			                  m_MaxSpeed;
  int			                    m_MinParticles;
  int			                    m_MaxParticles;
  float			                  m_MinEmissionTime;
  float			                  m_MaxEmissionTime;
  float			                  m_MinAge;
  float			                  m_MaxAge;
  float			                  m_MinSize;
  float			                  m_MaxSize;
  CColor			                m_Color1;
  CColor			                m_Color2;
  std::string		              m_sTexture;
  CRecyclingArray<CParticle> *m_Particles;
  float								m_Gravity;
  float								m_MaxVelocidadOndulacion;
  float								m_MinVelocidadOndulacion;
  Vect3f							m_vOndulacion1;
  Vect3f							m_vOndulacion2;
};

#endif
