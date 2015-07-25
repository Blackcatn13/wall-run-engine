#include "Particles\ParticleEmitter.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include "Math\Color.h"
#include "Math\Vector4.h"
#include "Math\MathUtils.h"
#include "GraphicsManager.h"
#include "Particle.h"
#include "Utils\Defines.h"
#include "Core\Core.h"
#include "Texture\Texture.h"
#include "Texture\TextureManager.h"
#include "RenderableVertex\RenderableVertexs.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "RenderableVertex\InstancingVertexs.h"
// #include <omp.h>

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
  , m_MaxSize(node.GetFloatProperty("max_size", 1.0f, false))
  , m_EndSize(node.GetFloatProperty("end_size", -1.0f, false))
  , m_Color1(node.GetCColorProperty("color_1", colWHITE, false))
  , m_Color2(node.GetCColorProperty("color_2", colWHITE, false))
  , m_vPos(node.GetVect3fProperty("v_Pos", v3fZERO, false))
  , m_Rotation(node.GetFloatProperty("rotation", 0.f))
  , m_vSpawnDir1(node.GetVect3fProperty("spawn_dir1", v3fZERO, false))
  , m_vSpawnDir2(node.GetVect3fProperty("spawn_dir2", v3fZERO, false))
  , m_Gravity(node.GetFloatProperty("Gravity", 0.0f, false))
  , m_MaxVelocidadOndulacion(node.GetFloatProperty("max_speed_ondulacion", 0.0f, false))
  , m_MinVelocidadOndulacion(node.GetFloatProperty("min_speed_ondulacion", 0.0f, false))
  , m_vOndulacion1(node.GetVect3fProperty("ondulacion_vec1", v3fZERO, false))
  , m_vOndulacion2(node.GetVect3fProperty("ondulacion_vec2", v3fZERO, false))
  , m_EndAlpha(node.GetFloatProperty("end_alpha", 1.f))
  , m_sTexture(node.GetPszISOProperty("texture", "", false)) {
  m_visible = true;
  m_Name = (node.GetPszISOProperty("name", "", false));
  std::string type = node.GetPszISOProperty("type", "PLANE", false);
  if (type == "ESF")
    m_Type = EMITTER_ESF;
  else if (type == "PLANE")
    m_Type = EMITTER_PLANE;
  else if (type == "ESF1")
    m_Type = EMITTER_ESF1;

  InitPool();
  m_TimeNextParticle = mathUtils::RandomFloatRange(m_MinEmissionTime, m_MaxEmissionTime);

  m_Texture = new CTexture();
  m_Texture->Load(m_sTexture);
  const uint32 lIdxCount = 6;
  const uint32 lVtxCount = 4;

  TPARTICLE_VERTEX vertexs[lVtxCount] = {
    {  -1.f, 15.0f, -1.f, 0, 0 },    // vertex 0
    {  -1.f, 15.0f,  1.f, 0, 1 },    // vertex 1
    {   1.f, 15.0f,  1.f, 1, 1 },    // vertex 2
    {   1.f, 15.0f, -1.f, 1, 0 }     // vertex 3
  };

  unsigned short int lIdx[lIdxCount] = { 0, 1, 2, 2, 3, 0};

  m_RV = new CInstancingVertexs<TPARTICLE_VERTEX_INSTANCE, TPARTICLE_VERTEX>(GRAPHM, &vertexs, &lIdx, lVtxCount, lIdxCount);
  ((CInstancingVertexs<TPARTICLE_VERTEX_INSTANCE, TPARTICLE_VERTEX> *)m_RV)->SetInstanceNumber(m_MaxParticles);
  ((CInstancingVertexs<TPARTICLE_VERTEX_INSTANCE, TPARTICLE_VERTEX> *)m_RV)->CreateInstanceBuffer(GRAPHM);
  m_vertex_list = new TPARTICLE_VERTEX_INSTANCE[m_MaxParticles];
}

CParticleEmitter::CParticleEmitter()
  : m_CurrentTime(.0f)
  , m_ExcedentTime(.0f)
  , m_MinSpeed(1.f)
  , m_MaxSpeed(1.f)
  , m_MinParticles(1)
  , m_MaxParticles(200)
  , m_MinEmissionTime(1)
  , m_MaxEmissionTime(1)
  , m_MinAge(1)
  , m_MaxAge(2)
  , m_MinSize(1)
  , m_MaxSize(1)
  , m_EndSize(1)
  , m_Color1(colWHITE)
  , m_Color2(colWHITE)
  , m_vPos(v3fZERO)
  , m_vSpawnDir1(v3fZERO)
  , m_vSpawnDir2(v3fZERO)
  , m_Gravity(0.f)
  , m_MaxVelocidadOndulacion(0.f)
  , m_MinVelocidadOndulacion(0.f)
  , m_vOndulacion1(v3fZERO)
  , m_Rotation(0.f)
  , m_vOndulacion2(v3fZERO)
  , m_sTexture("") {
  m_visible = true;
  std::string type = "PLANE";
  if (type == "ESF")
    m_Type = EMITTER_ESF;
  else if (type == "PLANE")
    m_Type = EMITTER_PLANE;
  else if (type == "ESF1")
    m_Type = EMITTER_ESF1;

  InitPool();
  m_TimeNextParticle = mathUtils::RandomFloatRange(m_MinEmissionTime, m_MaxEmissionTime);

  m_Texture = new CTexture();
  if (m_sTexture != "")
    m_Texture->Load(m_sTexture);
  const uint32 lIdxCount = 6;
  const uint32 lVtxCount = 4;

  TPARTICLE_VERTEX vertexs[lVtxCount] = {
    {  -1.f, 15.0f, -1.f, 0, 0 },    // vertex 0
    {  -1.f, 15.0f,  1.f, 0, 1 },    // vertex 1
    {   1.f, 15.0f,  1.f, 1, 1 },    // vertex 2
    {   1.f, 15.0f, -1.f, 1, 0 }     // vertex 3
  };

  unsigned short int lIdx[lIdxCount] = { 0, 1, 2, 2, 3, 0};

  m_RV = new CInstancingVertexs<TPARTICLE_VERTEX_INSTANCE, TPARTICLE_VERTEX>(GRAPHM, &vertexs, &lIdx, lVtxCount, lIdxCount);
  ((CInstancingVertexs<TPARTICLE_VERTEX_INSTANCE, TPARTICLE_VERTEX> *)m_RV)->SetInstanceNumber(m_MaxParticles);
  m_vertex_list = new TPARTICLE_VERTEX_INSTANCE[m_MaxParticles];
}

CParticleEmitter::~CParticleEmitter() {
  CHECKED_DELETE_ARRAY(m_RecyclingArray);
  CHECKED_DELETE_ARRAY(m_RecyclingArrayStatus);
  if (m_Texture != NULL) {
    uint32 width = m_Texture->GetWidth();
    if (width < 5000)
      CHECKED_DELETE(m_Texture);
  }
  delete[] m_vertex_list;
  CHECKED_DELETE(m_RV);

}

void CParticleEmitter::Render(CGraphicsManager *RM) {
  Mat44f t = m44fIDENTITY;
  RM->SetTransform(t);
  t.Translate(m_Position);
  RM->SetTransform(t);

  ((CInstancingVertexs<TPARTICLE_VERTEX_INSTANCE, TPARTICLE_VERTEX> *)m_RV)->AddInstancinguffer(RM, m_vertex_list);
  CEffectTechnique *l_EffectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(m_RV->GetVertexType()))->GetEffectTechnique();
  m_Texture->Activate(0);
  m_RV->Render(RM, l_EffectTechnique);
  t = m44fIDENTITY;
  RM->SetTransform(t);
}

void CParticleEmitter::Update(float dt) {
  m_CurrentTime = m_CurrentTime + dt;
  DeleteOldParticles(dt);
  if (m_CurrentTime >= m_TimeNextParticle) {
    int numberOfNewParticles = (int)((m_CurrentTime + m_ExcedentTime) / m_TimeNextParticle);
    m_ExcedentTime = m_CurrentTime - m_TimeNextParticle;
    m_TimeNextParticle = mathUtils::RandomFloatRange(m_MinEmissionTime, m_MaxEmissionTime);
    for (int i = 0; i < numberOfNewParticles; ++i) {
      CParticle *p = NewParticle();
      if (p != NULL) {
        PopulateParticle(p);
      }
    }
  }
  if (m_UsedElements > 0) {
    UpdateParticles(dt);
  }
}

bool CParticleEmitter::reloadTexture() {
  if (m_sTexture != "") {
    CHECKED_DELETE(m_Texture);
    m_Texture = new CTexture();
    m_Texture->Load(m_sTexture);
  }
  return false;
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
  CColor col1 = CColor(mathUtils::RandomFloatRange(r1, r2), mathUtils::RandomFloatRange(g1, g2), mathUtils::RandomFloatRange(b1, b2), 0.0f);
  p->setSpeed(speed);
  p->setSize(size);
  p->setAge(age);
  p->setPosition(m_vPos);
  p->setColor1(col1);
  p->setGravity(m_Gravity);
  p->setEndAlpha(1.f);
  p->setRotation(m_Rotation);
  if (m_EndSize < 0) {
    p->setEndSize(size);
  } else {
    p->setEndSize(m_EndSize);
  }
  p->setVelocidadOndulacion(mathUtils::RandomFloatRange(m_MinVelocidadOndulacion, m_MaxVelocidadOndulacion));

  /* l_Texture->Create(m_sTexture, size, size, 1, CTexture::TUsageType::RENDERTARGET, CTexture::TPoolType::DEFAULT, l_Texture->GetFormatTypeFromString("R8G8B8"));
   float height = l_Texture->GetHeight();*/
  //p->setTexture(m_Texture);
  float o1x = m_vOndulacion1.x;
  float o2x = m_vOndulacion2.x;
  float o1y = m_vOndulacion1.y;
  float o2y = m_vOndulacion2.y;
  float o1z = m_vOndulacion1.z;
  float o2z = m_vOndulacion2.z;

  p->setVectorOndulacion(Vect3f(mathUtils::RandomFloatRange(o1x, o2x), mathUtils::RandomFloatRange(o1y, o2y), mathUtils::RandomFloatRange(o1z, o2z)));
  p->setInicioOndulacion(mathUtils::RandomFloatRange(0, 360));
  Vect3f dirFinal = Vect3f(0, 1.0, 0);
  switch (m_Type) {
    case EMITTER_ESF:
      //Simulamos un generador de particulas esferico indicando un vector posicion central y un vector desviación
      dirFinal = m_vSpawnDir1;
      dirFinal.x += m_vSpawnDir2.x * mathUtils::RandomFloatRange(-1.0, 1.0);
      dirFinal.y += m_vSpawnDir2.y * mathUtils::RandomFloatRange(-1.0, 1.0);
      dirFinal.z += m_vSpawnDir2.z * mathUtils::RandomFloatRange(-1.0, 1.0);
      dirFinal = dirFinal.Normalize();
      break;
    case EMITTER_PLANE: {
      //Cambiamos para que en vez de generar particulas siempre en SpawnDir1 se coja un random entre SpawnDir1, SpawnDir2
      //p->setDirection1(m_vSpawnDir1);
      float d1x = m_vSpawnDir1.x;
      float d2x = m_vSpawnDir2.x;
      float d1y = m_vSpawnDir1.y;
      float d2y = m_vSpawnDir2.y;
      float d1z = m_vSpawnDir1.z;
      float d2z = m_vSpawnDir2.z;

      dirFinal = Vect3f(mathUtils::RandomFloatRange(d1x, d2x), mathUtils::RandomFloatRange(d1y, d2y), mathUtils::RandomFloatRange(d1z, d2z)).Normalize();
      break;
    }
    case EMITTER_ESF1: {
      float a = mathUtils::Deg2Rad<float>(mathUtils::RandomFloatRange(0, 360));
      float b = mathUtils::Deg2Rad<float>(mathUtils::RandomFloatRange(0, 360));
      float y = mathUtils::Deg2Rad<float>(mathUtils::RandomFloatRange(0, 360));
      dirFinal = Vect3f(mathUtils::Cos<float>(a), mathUtils::Cos<float>(b), mathUtils::Cos<float>(y));
      break;
    }
  }
  p->setDirection1(dirFinal);
}


void CParticleEmitter::InitPool() {
  m_RecyclingArray = new CParticle[m_MaxParticles];
  m_RecyclingArrayStatus = new bool[m_MaxParticles];

  for (int i = 0; i < m_MaxParticles; ++i)
    m_RecyclingArrayStatus[i] = true;
  m_FreeElements = m_MaxParticles;
  m_UsedElements = 0;
}

void CParticleEmitter::DeleteOldParticles(float age) {
  //#pragma omp parallel for
  for (int i = 0; i < m_MaxParticles; ++i) {
    if (!m_RecyclingArrayStatus[i]) {
      if ((&(m_RecyclingArray[i]))->takeLife(age) <= 0) {
        m_UsedElements--;
        m_FreeElements++;
        m_RecyclingArrayStatus[i] = true;
        m_vertex_list[i].visible = 0;
      }
    }
  }
}

void CParticleEmitter::UpdateParticles(float dt) {
  //#pragma omp parallel for
  for (int i = 0; i < m_MaxParticles; ++i) {
    if (!m_RecyclingArrayStatus[i]) {
      CParticle *p = &(m_RecyclingArray[i]);
      p->Update(dt);
      Vect3f pos = p->getPosition();
      m_vertex_list[i].x = pos.x;
      m_vertex_list[i].y = pos.y;
      m_vertex_list[i].z = pos.z;
      m_vertex_list[i].size = p->getSize();
      m_vertex_list[i].visible = p->getAlpha();
    }
  }
}

CParticle *CParticleEmitter::NewParticle() {
  bool found = false;
  int index = 0;

  if (m_FreeElements > 0) {
    while (!found && index < m_MaxParticles) {
      found = m_RecyclingArrayStatus[index];
      ++index;
    }
  }

  if (found) {
    index--;
    m_FreeElements--;
    m_UsedElements++;

    m_RecyclingArrayStatus[index] = false;
    m_vertex_list[index].visible = 1;
    return &(m_RecyclingArray[index]);
  } else
    return 0;
}