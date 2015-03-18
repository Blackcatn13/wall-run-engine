#ifndef CPARTICLE_EMITTER_H
#define CPARTICLE_EMITTER_H

#include <string>
#include "Object\Object3D.h"
#include "Utils\Defines.h"
#include <vector>
#include "Math\Color.h"
#include "Math\Vector3.h"



class CXMLTreeNode;
class CGraphicsManager;


class CParcitleEmitter : public CObject3D
{
private:
   Vect3f			m_vPos;
   Vect3f			m_vSpawnDir1;
   Vect3f			m_vSpawnDir2;
   float			m_CurrentTime;
   float			m_TimeNextParticle;

   float			m_MinSpeed;
   float			m_MaxSpeed;
   int				m_MinParticles;
   int				m_MaxParticles;
   float			m_MinEmissionTime;
   float			m_MaxEmissionTime;
   float			m_MinAge;
   float			m_MaxAge;
   float			m_MinSize;
   float			m_MaxSize;
   CColor			m_Color1;
   CColor			m_Color2;
   std::string		m_sTexture;

 

 //  std::vector<CParticle*>		m_Particles;


   /*
float		m_fNumNewPartsExcess
LPDIRECT3DDEVICE9			m_pd3dDevice
LPDIRECT3DVERTEXBUFFER9		m_vbParticles
CRecyclingArray<CParticle, NUMPARTICLES>	m_Particles
   */

public:
    CParcitleEmitter(CXMLTreeNode  &node);
    ~CParcitleEmitter();
    void Render(CGraphicsManager *RM);
    void Update(float dt);
};

#endif
