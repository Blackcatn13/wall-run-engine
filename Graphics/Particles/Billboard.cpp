#include "Billboard.h"
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
#include "RenderableVertex\IndexedVertexs.h"
#include "RenderableVertex\VertexTypes.h"
#include "Renderable\RenderableObjectTechniqueManager.h"

CBillboard::CBillboard(CXMLTreeNode &node)
  : m_sizes(node.GetVect2fProperty("size", v2fONE))
  , m_position(node.GetVect3fProperty("position", v3fZERO	, false))
  , m_Color1(CColor((node.GetVect4fProperty("color", v4fZERO	, false)))) {
  int numChild = node.GetNumChildren();
  for (int i = 0; i < numChild; ++i) {
    CXMLTreeNode nodeChild = node(i);
    std::string textureName = nodeChild.GetPszISOProperty("name", "");
    if (TEXTM->ExisteResource(textureName)) {
      m_Textures.push_back(TEXTM->GetResource(textureName));
    } else {
      CTexture *texture = new CTexture();
      texture->Load(textureName);
      TEXTM->AddResource(textureName, texture);
      m_Textures.push_back(texture);
    }
  }

  const uint32 lIdxCount = 6;
  const uint32 lVtxCount = 4;

  TBILLBOARD_VERTEX vertexs[lVtxCount] = {
    {  -1.f, 0.0f, -1.f, 0, 0, m_sizes.x, m_sizes.y },    // vertex 0
    {  -1.f, 0.0f,  1.f, 0, 1, m_sizes.x, m_sizes.y },    // vertex 1
    {   1.f, 0.0f,  1.f, 1, 1, m_sizes.x, m_sizes.y },    // vertex 2
    {   1.f, 0.0f, -1.f, 1, 0, m_sizes.x, m_sizes.y }     // vertex 3
  };

  unsigned short int lIdx[lIdxCount] = { 0, 1, 2, 2, 3, 0};

  m_RV = new CIndexedVertexs<TBILLBOARD_VERTEX>(GRAPHM, &vertexs, &lIdx, lVtxCount, lIdxCount);
}

CBillboard::CBillboard(float size)
  : m_size(size)
  , m_position(v3fZERO)
  , m_RV(NULL) {
}

CBillboard::CBillboard(float size, Vect3f pos)
  : m_size(size)
  , m_position(pos)
  , m_RV(NULL) {
}

CBillboard::~CBillboard() {
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
}

void CBillboard::Render(CGraphicsManager *GM) {
  Mat44f t = m44fIDENTITY;
  GM->SetTransform(t);
  t.Translate(m_position);
  GM->SetTransform(t);

  CEffectTechnique *l_EffectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(m_RV->GetVertexType()))->GetEffectTechnique();
  for (size_t i = 0; i < m_Textures.size(); ++i)
    m_Textures[i]->Activate(i);
  m_RV->Render(GM, l_EffectTechnique);
  t = m44fIDENTITY;
  GM->SetTransform(t);
}


void CBillboard::Update(float ElapsedTime) {
}