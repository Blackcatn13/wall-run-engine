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
  , m_Gravity(node.GetFloatProperty("Gravity", 0.0f, false))
  , m_MaxVelocidadOndulacion(node.GetFloatProperty("max_speed_ondulacion", 0.0f, false))
  , m_MinVelocidadOndulacion(node.GetFloatProperty("min_speed_ondulacion", 0.0f, false))
  , m_vOndulacion1(node.GetVect3fProperty("ondulacion_vec1", v3fZERO, false))
  , m_vOndulacion2(node.GetVect3fProperty("ondulacion_vec2", v3fZERO, false))
  , m_sTexture(node.GetPszISOProperty("texture", "", false)) {
  std::string type = node.GetPszISOProperty("type", "PLANE", false);
  if (type == "ESF")
    m_Type = EMITTER_ESF;
  else if (type == "PLANE")
    m_Type = EMITTER_PLANE;
  m_Particles = new CRecyclingArray<CParticle>(m_MaxParticles);
  m_TimeNextParticle = mathUtils::RandomFloatRange(m_MinEmissionTime, m_MaxEmissionTime);

  m_Texture = new CTexture();
  m_Texture->Load(m_sTexture);
  /*m_vertex_list = (TTEXTURE_NORMAL_VERTEX *)malloc(m_MaxSize * 4 * sizeof(TTEXTURE_NORMAL_VERTEX));
  m_index_list = (unsigned short *)malloc(m_MaxSize * 6 * sizeof(unsigned short));*/
  const uint32 lIdxCount = 6;
  const uint32 lVtxCount = 4;

  TPARTICLE_VERTEX vertexs[lVtxCount] = {
    {  -0.5f, 0.0f, -0.5f, 0, 0 },    // vertex 0
    {  -0.5f, 0.0f,  0.5f, 0, 1 },    // vertex 1
    {   0.5f, 0.0f,  0.5f, 1, 1 },    // vertex 2
    {   0.5f, 0.0f, -0.5f, 1, 0 }     // vertex 3
  };

  unsigned short int lIdx[lIdxCount] = { 0, 1, 2,  2, 3, 0 };

  m_RV = new CInstancingVertexs<TPARTICLE_VERTEX>(GRAPHM, &vertexs, &lIdx, lVtxCount, lIdxCount);
  ((CInstancingVertexs<TPARTICLE_VERTEX> *)m_RV)->SetInstanceNumber(m_MaxParticles);
  //m_vertex_list = (TPARTICLE_VERTEX_INSTANCE *)malloc(m_MaxSize * sizeof(TPARTICLE_VERTEX_INSTANCE));
  const uint32 size_instancing = 200;
  m_vertex_list = new TPARTICLE_VERTEX_INSTANCE[size_instancing];
}

CParticleEmitter::~CParticleEmitter() {
  m_Particles->DeleteAllElements();
  for (size_t i = 0; i < m_Particles->GetNumFreeElements(); ++i)
    m_Particles->Free(i);

  CHECKED_DELETE(m_Particles);
  if (m_Texture != NULL) {
    uint32 width = m_Texture->GetWidth();
    if (width < 5000)
      CHECKED_DELETE(m_Texture);
  }
  delete[] m_vertex_list;
  /*free(m_vertex_list);
  free(m_index_list);*/
}

void CParticleEmitter::Render(CGraphicsManager *RM) {
  Mat44f t = m44fIDENTITY;
  RM->SetTransform(t);
  t.Translate(m_Position);
  RM->SetTransform(t);

  /*int numOfParticles = m_Particles->GetNumUsedElements();
  int vertex_num = 0;
  int index_num = 0;
  CCamera *actCam = CAMCONTM->getActiveCamera();
  Vect3f up_cam = actCam->GetVecUp().Normalize();*/

  for (int i = 0; i < m_MaxParticles; i++) {
    if (!m_Particles->IsFree(i)) {
      //m_Particles->GetAt(i)->Render(RM);
      CParticle *p = m_Particles->GetAt(i);
      m_vertex_list[i].x = p->getPosition().x;
      m_vertex_list[i].y = p->getPosition().y;
      m_vertex_list[i].z = p->getPosition().z;
      m_vertex_list[i].size = p->getSize();
      m_vertex_list[i].visible = 1;
    } else {
      m_vertex_list[i].visible = 0;
      /*Vect3f up =  up_cam * p->getSize() / 2;
      Vect3f direction = actCam->GetDirection().Normalize();
      Vect3f right = (up_cam ^ direction) * p->getSize() / 2;
      Vect3f ul = p->getPosition() + up - right;
      Vect3f ur = p->getPosition() + up + right;
      Vect3f dl = p->getPosition() - up - right;
      Vect3f dr = p->getPosition() - up + right;
      Vect3f v2 = ur - ul;
      Vect3f v1 = dl - ul;
      Vect3f normal = Vect3f(v1.y * v2.z - v1.z * v2.y, v1.x * v2.z - v1.z * v1.x, v1.x * v2.y - v1.y * v2.x);

      m_index_list[index_num++] = vertex_num;
      m_index_list[index_num++] = vertex_num + 1;
      m_index_list[index_num++] = vertex_num + 2;
      m_index_list[index_num++] = vertex_num + 1;
      m_index_list[index_num++] = vertex_num + 3;
      m_index_list[index_num++] = vertex_num + 2;


      m_vertex_list[vertex_num].x = ul.x;
      m_vertex_list[vertex_num].y = ul.y;
      m_vertex_list[vertex_num].z = ul.z;
      m_vertex_list[vertex_num].tu = 0.f;
      m_vertex_list[vertex_num].tv = 0.f;
      m_vertex_list[vertex_num].nx = normal.x;
      m_vertex_list[vertex_num].ny = normal.y;
      m_vertex_list[vertex_num].nz = normal.z;
      vertex_num++;

      m_vertex_list[vertex_num].x = ur.x;
      m_vertex_list[vertex_num].y = ur.y;
      m_vertex_list[vertex_num].z = ur.z;
      m_vertex_list[vertex_num].tu = 0.f;
      m_vertex_list[vertex_num].tv = 1.f;
      m_vertex_list[vertex_num].nx = normal.x;
      m_vertex_list[vertex_num].ny = normal.y;
      m_vertex_list[vertex_num].nz = normal.z;
      vertex_num++;

      m_vertex_list[vertex_num].x = dl.x;
      m_vertex_list[vertex_num].y = dl.y;
      m_vertex_list[vertex_num].z = dl.z;
      m_vertex_list[vertex_num].tu = 1.f;
      m_vertex_list[vertex_num].tv = 0.f;
      m_vertex_list[vertex_num].nx = normal.x;
      m_vertex_list[vertex_num].ny = normal.y;
      m_vertex_list[vertex_num].nz = normal.z;
      vertex_num++;


      m_vertex_list[vertex_num].x = dr.x;
      m_vertex_list[vertex_num].y = dr.y;
      m_vertex_list[vertex_num].z = dr.z;
      m_vertex_list[vertex_num].tu = 1.f;
      m_vertex_list[vertex_num].tv = 1.f;
      m_vertex_list[vertex_num].nx = normal.x;
      m_vertex_list[vertex_num].ny = normal.y;
      m_vertex_list[vertex_num].nz = normal.z;
      vertex_num++;*/

    }
  }
  ((CInstancingVertexs<TPARTICLE_VERTEX> *)m_RV)->AddInstancinguffer(RM, m_vertex_list);
  CEffectTechnique *l_EffectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(m_RV->GetVertexType()))->GetEffectTechnique();
  m_Texture->Activate(0);
  m_RV->Render(RM, l_EffectTechnique);
  //free(m_vertex_list);
  /*CIndexedVertexs<TTEXTURE_NORMAL_VERTEX> m_RV = CIndexedVertexs<TTEXTURE_NORMAL_VERTEX>(GRAPHM, m_vertex_list, m_index_list, numOfParticles * 4, numOfParticles * 6);
  CEffectTechnique *l_EffectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(m_RV.GetVertexType()))->GetEffectTechnique();
  m_Texture->Activate(0);
  m_RV.Render(RM, l_EffectTechnique);*/
  /*CRenderableVertexs *m_RV = new CIndexedVertexs<TTEXTURE_NORMAL_VERTEX>(GRAPHM, m_vertex_list, m_index_list, numOfParticles * 4, numOfParticles * 6);
  CEffectTechnique *l_EffectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(m_RV->GetVertexType()))->GetEffectTechnique();
  m_Texture->Activate(0);
  m_RV->Render(RM, l_EffectTechnique);*/
  t = m44fIDENTITY;
  RM->SetTransform(t);
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
  CColor col1 = CColor(mathUtils::RandomFloatRange(r1, r2), mathUtils::RandomFloatRange(g1, g2), mathUtils::RandomFloatRange(b1, b2), 0.0f);
  p->setSpeed(speed);
  p->setSize(size);
  p->setAge(age);
  p->setPosition(m_vPos);
  p->setColor1(col1);
  p->setGravity(m_Gravity);
  p->setVelocidadOndulacion(mathUtils::RandomFloatRange(m_MinVelocidadOndulacion, m_MaxVelocidadOndulacion));

  /* l_Texture->Create(m_sTexture, size, size, 1, CTexture::TUsageType::RENDERTARGET, CTexture::TPoolType::DEFAULT, l_Texture->GetFormatTypeFromString("R8G8B8"));
   float height = l_Texture->GetHeight();*/
  p->setTexture(m_Texture);
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
    case EMITTER_PLANE:
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
  p->setDirection1(dirFinal);
}