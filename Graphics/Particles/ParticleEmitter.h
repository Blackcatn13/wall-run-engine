#ifndef CPARTICLE_EMITTER_H
#define CPARTICLE_EMITTER_H

#include <string>
#include "Object\Object3D.h"
#include "Utils\Defines.h"
#include <vector>
#include "Math\Color.h"
#include "Math\Vector3.h"
//#include "Particles/RecyclingArray.h"
#include "RenderableVertex\VertexTypes.h"
#include "Utils\Named.h"

class CXMLTreeNode;
class CGraphicsManager;
class CParticle;
class CTexture;
class CRenderableVertexs;

enum EmitterType {EMITTER_ESF, EMITTER_PLANE, EMITTER_ESF1};

class CParticleEmitter : public CObject3D, public CNamed {
 public:
  CParticleEmitter();
  CParticleEmitter(CXMLTreeNode  &node);
  ~CParticleEmitter();
  void Render(CGraphicsManager *RM);
  void Update(float dt);
  GET_SET(EmitterType, Type);
  GET_SET(Vect3f, vPos);
  GET_SET(Vect3f, vSpawnDir1);
  GET_SET(Vect3f, vSpawnDir2);
  GET_SET(float, MinSpeed);
  GET_SET(float, MaxSpeed);
  GET_SET(int, MinParticles);
  GET_SET(int, MaxParticles);
  GET_SET(float, MinEmissionTime);
  GET_SET(float, MaxEmissionTime);
  GET_SET(float, MinAge);
  GET_SET(float, MaxAge);
  GET_SET(float, MinSize);
  GET_SET(float, MaxSize);
  GET_SET(CColor, Color1);
  GET_SET(CColor, Color2);
  GET_SET(std::string, sTexture);
  GET_SET(float, Gravity);
  GET_SET(float, MaxVelocidadOndulacion);
  GET_SET(float, MinVelocidadOndulacion);
  GET_SET(Vect3f, vOndulacion1);
  GET_SET(Vect3f, vOndulacion2);
  GET_SET(bool, visible);
  bool reloadTexture();
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
  float 			                m_MaxSize;
  CColor			                m_Color1;
  CColor			                m_Color2;
  std::string		              m_sTexture;
  //CRecyclingArray<CParticle> *m_Particles;
  float								        m_Gravity;
  float								        m_MaxVelocidadOndulacion;
  float								        m_MinVelocidadOndulacion;
  Vect3f							        m_vOndulacion1;
  Vect3f							        m_vOndulacion2;

  CTexture							     *m_Texture;
  CRenderableVertexs         *m_RV;
  TPARTICLE_VERTEX_INSTANCE  *m_vertex_list;
  unsigned short             *m_index_list;
  bool                        m_visible;

  // POOL of particles
  uint32  m_FreeElements;
  uint32  m_UsedElements;
  CParticle *m_RecyclingArray;
  bool *m_RecyclingArrayStatus;

  void InitPool();
  void DeleteOldParticles(float age);
  void UpdateParticles(float dt);
  CParticle *NewParticle();
};

#endif
