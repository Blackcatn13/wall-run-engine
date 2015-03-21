#include "Particles\ParticleEmitter.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include "Math\Color.h"
#include "Math\Vector4.h"
#include "Math\MathUtils.h"
#include "GraphicsManager.h"
#include "Particle.h"

CParticleEmitter::CParticleEmitter(CXMLTreeNode  &node)
  : m_CurrentTime(.0f)
  , m_ExcedentTime(.0f)
  , m_MinSpeed(node.GetFloatProperty("min_speed", -1.0f, false))
  , m_MaxSpeed(node.GetFloatProperty("max_speed", -1.0f, false))
  , m_MinParticles(node.GetIntProperty("min_particles", -1, false))
  , m_MaxParticles(node.GetIntProperty("max_particles", -1, false))
  , m_MinEmissionTime(node.GetFloatProperty("min_emission_time", -1.0f, false))
  , m_MaxEmissionTime(node.GetFloatProperty("max_emission_time", -1.0f, false))
  , m_MinAge(node.GetFloatProperty("min_age", -1.0f, false))
  , m_MaxAge(node.GetFloatProperty("max_age", -1.0f, false))
  , m_MinSize(node.GetFloatProperty("min_size", -1.0f, false))
  , m_MaxSize(node.GetFloatProperty("max_size", -1.0f, false))
  , m_Color1(node.GetCColorProperty("color_1", colWHITE, false))
  , m_Color2(node.GetCColorProperty("color_2", colWHITE, false))
  , m_vPos(node.GetVect3fProperty("v_Pos", v3fZERO, false))
  , m_vSpawnDir1(node.GetVect3fProperty("spawn_dir1", v3fZERO, false))
  , m_vSpawnDir2(node.GetVect3fProperty("spawn_dir2", v3fZERO, false))
  , m_sTexture(node.GetPszISOProperty("texture", "", false)) {
  std::string type = node.GetPszISOProperty("type", "", false);
  if (type == "ESF")
    m_Type = EMITTER_ESF;
  else if (type == "PLANE")
    m_Type = EMITTER_PLANE;
  m_Particles = new CRecyclingArray<CParticle>(m_MaxParticles);
  m_TimeNextParticle = mathUtils::RandomFloatRange(m_MinEmissionTime, m_MaxEmissionTime);
}

CParticleEmitter::~CParticleEmitter() {}

void CParticleEmitter::Render(CGraphicsManager *RM) {
  for (int i = 0; i < m_MaxParticles; i++) {
    if (!m_Particles->IsFree(i)) {
      m_Particles->GetAt(i)->Render(RM);
    }
  }
}

void CParticleEmitter::Update(float dt) {
  m_CurrentTime = m_CurrentTime + dt;
  m_Particles->ParticleDeleteOld(dt);
  if (m_CurrentTime >= m_TimeNextParticle) {
    int numberOfNewParticles = (int)((m_CurrentTime + m_ExcedentTime) / m_TimeNextParticle);
    m_ExcedentTime = m_CurrentTime - m_TimeNextParticle;
    m_TimeNextParticle = mathUtils::RandomFloatRange(m_MinEmissionTime, m_MaxEmissionTime);
    for (int i = 0; i < numberOfNewParticles; ++i) {
      CParticle *p = m_Particles->New();
      if (p != NULL) {
        PopulateParticle(p);
      }
    }
  }
  if (m_Particles->GetNumUsedElements() > 0) {
    m_Particles->ParticleUpdate(dt);
  }
}

void CParticleEmitter::PopulateParticle(CParticle *p) {
  float speed = mathUtils::RandomFloatRange(m_MinSpeed, m_MaxSpeed);
  float size = mathUtils::RandomFloatRange(m_MinSize, m_MaxSize);
  float age = mathUtils::RandomFloatRange(m_MinAge, m_MaxAge);
  float r1 = m_Color1.GetRed();
  float g1 = m_Color1.GetGreen();
  float b1 = m_Color1.GetBlue();
  float r2 = m_Color2.GetRed();
  float g2 = m_Color2.GetGreen();
  float b2 = m_Color2.GetBlue();
  CColor col1 = CColor(mathUtils::RandomFloatRange(r1, r2), mathUtils::RandomFloatRange(g1, g2), mathUtils::RandomFloatRange(b1, b2));
  p->setSpeed(speed);
  p->setSize(size);
  p->setAge(age);
  p->setPosition(m_vPos);
  p->setColor1(col1);
  p->setDirection1(m_vSpawnDir1);
  switch (m_Type) {
    case EMITTER_ESF:
      break;
    case EMITTER_PLANE:
      break;
  }
}