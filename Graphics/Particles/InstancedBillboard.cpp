#include "Particles\InstancedBillboard.h"
#include "Particles\Billboard.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "Camera\CameraController.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "XML\XMLTreeNode.h"
#include "Texture\Texture.h"
#include "Math\Color.h"
#include "Utils\Defines.h"
#include "RenderableVertex\RenderableVertexs.h"
#include "RenderableVertex\InstancingVertexs.h"
#include "RenderableVertex\VertexTypes.h"
#include "Renderable\RenderableObjectTechniqueManager.h"


CInstancedBillboard::CInstancedBillboard(CXMLTreeNode &node)
  : m_numBillboards(node.GetIntProperty("billboards")) {
  int numChild = node.GetNumChildren();
  m_vertex_list = new TBILLBOARD_VERTEX_INSTANCE[m_numBillboards];
  int j = 0;
  for (int i = 0; i < numChild; ++i) {
    CXMLTreeNode nodeChild = node.getNextChild();
    std::string Name = nodeChild.GetName();
    if (Name == "Texture") {
      std::string TextureName = nodeChild.GetPszISOProperty("name", "");
      if (TEXTM->ExisteResource(TextureName)) {
        m_Textures.push_back(TEXTM->GetResource(TextureName));
      } else {
        CTexture *texture = new CTexture();
        texture->Load(TextureName);
        TEXTM->AddResource(TextureName, texture);
        m_Textures.push_back(texture);
      }
    } else if (Name == "Instance") {
      Vect3f position = nodeChild.GetVect3fProperty("position", v3fZERO);
      Vect2f size = nodeChild.GetVect2fProperty("size", v2fONE);
      m_vertex_list[j].x = position.x;
      m_vertex_list[j].y = position.y;
      m_vertex_list[j].z = position.z;
      m_vertex_list[j].sizex = size.x;
      m_vertex_list[j].sizey = size.y;
      //m_vertex_list[j].visible = 1;
      m_vertex_list[j].tick = ((rand() % 100 + 1) / 10);
      j++;
    }
  }

  const uint32 lIdxCount = 6;
  const uint32 lVtxCount = 4;

  TBILLBOARD_INSTANCED_VERTEX vertexs[lVtxCount] = {
    {  -1.f, 0.0f, -1.f, 0, 1 },    // vertex 0
    {  -1.f, 0.0f,  1.f, 0, 0 },    // vertex 1
    {   1.f, 0.0f,  1.f, 1, 0 },    // vertex 2
    {   1.f, 0.0f, -1.f, 1, 1 }     // vertex 3
  };

  unsigned short int lIdx[lIdxCount] = { 0, 1, 2, 2, 3, 0};

  m_RV = new CInstancingVertexs<TBILLBOARD_VERTEX_INSTANCE, TBILLBOARD_INSTANCED_VERTEX>(GRAPHM, &vertexs, &lIdx, lVtxCount, lIdxCount);
  ((CInstancingVertexs<TBILLBOARD_VERTEX_INSTANCE, TBILLBOARD_INSTANCED_VERTEX> *)m_RV)->SetInstanceNumber(m_numBillboards);
  ((CInstancingVertexs<TBILLBOARD_VERTEX_INSTANCE, TBILLBOARD_INSTANCED_VERTEX> *)m_RV)->CreateInstanceBuffer(GRAPHM);
}

CInstancedBillboard::~CInstancedBillboard() {
  /*for (std::vector<CTexture *>::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it) {
    CHECKED_DELETE((*it));
  }*/
  /* if (m_Texture != NULL) {
     uint32 width = m_Texture->GetWidth();
     if (width < 5000)
       CHECKED_DELETE(m_Texture);
   }*/
  if (m_RV != NULL)
    CHECKED_DELETE(m_RV);
  delete[] m_vertex_list;
}

void CInstancedBillboard::Render(CGraphicsManager *GM) {
  Mat44f t = m44fIDENTITY;
  GM->SetTransform(t);
  t.Translate(v3fZERO);
  GM->SetTransform(t);

  ((CInstancingVertexs<TBILLBOARD_VERTEX_INSTANCE, TBILLBOARD_INSTANCED_VERTEX> *)m_RV)->AddInstancinguffer(GM, m_vertex_list);
  CEffectTechnique *l_EffectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(m_RV->GetVertexType()))->GetEffectTechnique();
  for (size_t i = 0; i < m_Textures.size(); ++i)
    m_Textures[i]->Activate(i);
  m_RV->Render(GM, l_EffectTechnique);
  t = m44fIDENTITY;
  GM->SetTransform(t);
  for (size_t i = 0; i < m_Textures.size(); ++i)
    m_Textures[i]->Deactivate(i);
}


void CInstancedBillboard::Update(float ElapsedTime) {
  for (int i = 0; i < m_numBillboards; ++i) {
    m_vertex_list[i].tick += 1.f * ElapsedTime;
    if (m_vertex_list[i].tick >= 1000.f)
      m_vertex_list[i].tick = 0.f;
  }
}