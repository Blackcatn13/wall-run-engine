#include "StaticMesh.h"
#include "Texture/Texture.h"
#include "RenderableVertex/RenderableVertexs.h"
#include "GraphicsManager.h"
#include "RenderableVertex/VertexTypes.h"
#include "RenderableVertex/IndexedVertexs.h"
#include "Core/Core.h"
#include <cstdio>
#include "Utils/Logger.h"
#include "Texture\TextureManager.h"
#include "Texture\CubeTexture.h"
#include "Effects\EffectManager.h"
//#include "Effects\EffectManager.h"
#include "Renderable\RenderableObjectTechniqueManager.h"

#include "Core_Utils/MemLeaks.h"

CStaticMesh::CStaticMesh() : m_NumVertexs(0),
  m_NumFaces(0), m_deleted(false) {
}

CStaticMesh::~CStaticMesh() {
  if (!m_deleted) {
    Destroy();
    m_deleted = true;
  }
}

void CStaticMesh::Destroy() {
  for (std::vector<CRenderableVertexs *>::iterator it = m_RVs.begin(); it != m_RVs.end(); ++it) {
    CHECKED_DELETE(*it);
  }
  m_RVs.clear();
  for (std::vector<std::vector<CTexture *>>::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it) {
    for (std::vector<CTexture *>::iterator it1 = it->begin(); it1 != it->end(); ++it1) {
      //CHECKED_DELETE(*it1);
    }
    it->clear();
  }
  m_Textures.clear();
}

void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount,
                              size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride,
                              size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride) {
  D3DXVECTOR3 *tan1 = new D3DXVECTOR3[VtxCount * 2];
  D3DXVECTOR3 *tan2 = tan1 + VtxCount;
  ZeroMemory(tan1, VtxCount * sizeof(D3DXVECTOR3) * 2);
  unsigned char *l_VtxAddress = (unsigned char *)VtxsData;
  for (size_t b = 0; b < IdxCount; b += 3) {
    unsigned short i1 = IdxsData[b];
    unsigned short i2 = IdxsData[b + 1];
    unsigned short i3 = IdxsData[b + 2];
    D3DXVECTOR3 *v1 = (D3DXVECTOR3 *)
                      &l_VtxAddress[i1 * VertexStride + GeometryStride];
    D3DXVECTOR3 *v2 = (D3DXVECTOR3 *)
                      &l_VtxAddress[i2 * VertexStride + GeometryStride];
    D3DXVECTOR3 *v3 = (D3DXVECTOR3 *)
                      &l_VtxAddress[i3 * VertexStride + GeometryStride];
    D3DXVECTOR2 *w1 = (D3DXVECTOR2 *)
                      &l_VtxAddress[i1 * VertexStride + TextureCoordsStride];
    D3DXVECTOR2 *w2 = (D3DXVECTOR2 *)
                      &l_VtxAddress[i2 * VertexStride + TextureCoordsStride];
    D3DXVECTOR2 *w3 = (D3DXVECTOR2 *)
                      &l_VtxAddress[i3 * VertexStride + TextureCoordsStride];
    float x1 = v2->x - v1->x;
    float x2 = v3->x - v1->x;
    float y1 = v2->y - v1->y;
    float y2 = v3->y - v1->y;
    float z1 = v2->z - v1->z;
    float z2 = v3->z - v1->z;
    float s1 = w2->x - w1->x;
    float s2 = w3->x - w1->x;
    float t1 = w2->y - w1->y;
    float t2 = w3->y - w1->y;
    float r = 1.0F / (s1 * t2 - s2 * t1);
    D3DXVECTOR3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                     (t2 * z1 - t1 * z2) * r);
    D3DXVECTOR3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                     (s1 * z2 - s2 * z1) * r);
    assert(i1 < VtxCount);
    assert(i2 < VtxCount);
    assert(i3 < VtxCount);
    tan1[i1] += sdir;
    tan1[i2] += sdir;
    tan1[i3] += sdir;
    tan2[i1] += tdir;
    tan2[i2] += tdir;
    tan2[i3] += tdir;
  }
  for (size_t b = 0; b < VtxCount; ++b) {
    D3DXVECTOR3 *l_NormalVtx = (D3DXVECTOR3 *)
                               &l_VtxAddress[b * VertexStride + NormalStride];
    D3DXVECTOR3 *l_TangentVtx = (D3DXVECTOR3 *)
                                &l_VtxAddress[b * VertexStride + TangentStride];
    D3DXVECTOR4 *l_TangentVtx4 = (D3DXVECTOR4 *)
                                 &l_VtxAddress[b * VertexStride + TangentStride];
    D3DXVECTOR3 *l_BiNormalVtx = (D3DXVECTOR3 *)
                                 &l_VtxAddress[b * VertexStride + BiNormalStride];
    const D3DXVECTOR3 &t = tan1[b];
    // Gram-Schmidt orthogonalize
    D3DXVECTOR3 l_VAl = t - (*l_NormalVtx) * D3DXVec3Dot(l_NormalVtx, &t);
    D3DXVec3Normalize(l_TangentVtx, &l_VAl);
    //tangent[a] = (t - n * Dot(n, t)).Normalize();
    // Calculate handedness
    D3DXVECTOR3 l_Cross;
    D3DXVec3Cross(&l_Cross, l_NormalVtx, l_TangentVtx);
    l_TangentVtx4->w = (D3DXVec3Dot(&l_Cross, &tan2[b]) < 0.0f ) ? -1.0f : 1.0f;
    //tangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
    D3DXVec3Cross(l_BiNormalVtx, l_NormalVtx, l_TangentVtx);
  }
  delete[] tan1;
}

bool CStaticMesh::Load (const std::string &FileName) {
  m_RVs = std::vector<CRenderableVertexs *>();
  m_Textures = std::vector<std::vector<CTexture *>>();
  m_FileName = FileName;
  FILE *f;
  f = fopen(FileName.c_str(), "rb");
  if (f != NULL) {
    unsigned short header;
    fread(&header, sizeof header, 1, f);
    if (header == 0x55ff) {
      unsigned short n_mat;
      fread(&n_mat, sizeof n_mat, 1, f);
      std::vector<unsigned short> vertex_type;
      for (int i = 0; i < n_mat; ++i) {
        unsigned short aux;
        fread(&aux, sizeof aux, 1, f);
        vertex_type.push_back(aux);
        unsigned short n_text;
        fread(&n_text, sizeof n_text, 1, f);
        m_Textures.push_back(std::vector<CTexture *>());
        for (int j = 0; j < n_text; ++j) {
          unsigned short aux_size;
          fread(&aux_size, sizeof aux_size, 1, f);
          char *buff = (char *)malloc(aux_size * sizeof(char) + 1);
          size_t len = fread(buff, sizeof(char), aux_size + 1, f);
          std::string aux_name(buff);
          delete(buff);
          CTexture *aux_text;
          if (TEXTM->ExisteResource(aux_name)) {
            aux_text = TEXTM->GetResource(aux_name);
          } else {
            if (vertex_type[i] == TTEXTURE_CUBE_NORMAL_VERTEX::GetVertexType() && ((j + 1) == n_text)) {
              aux_text = new CCubeTexture();
              aux_text->Load(aux_name);
            } else {
              aux_text = new CTexture();
              aux_text->Load(aux_name);
            }
            TEXTM->AddResource(aux_name, aux_text);
          }
          m_Textures[i].push_back(aux_text);
        }
      }
      for (int i = 0; i < n_mat; ++i) {
        unsigned short n_vtxs;
        fread(&n_vtxs, sizeof n_vtxs, 1, f);
        int type_size;
        // Getting vertex type size
        if (vertex_type[i] == TTEXTURE_NORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TTEXTURE_NORMAL_VERTEX);
        if (vertex_type[i] == TCOLORED_VERTEX::GetVertexType())
          type_size = sizeof(TCOLORED_VERTEX);
        if (vertex_type[i] == TCOLOR_TEXTURE2_VERTEX::GetVertexType())
          type_size = sizeof(TCOLOR_TEXTURE2_VERTEX);
        if (vertex_type[i] == TCOLOR_TEXTURE_NORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TCOLOR_TEXTURE_NORMAL_VERTEX);
        if (vertex_type[i] == TCOLOR_NORMAL_DIFFUSE_TEXTURE2_VERTEX::GetVertexType())
          type_size = sizeof(TCOLOR_NORMAL_DIFFUSE_TEXTURE2_VERTEX);
        if (vertex_type[i] == TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX);
        if (vertex_type[i] == TTEXTURE_VERTEX::GetVertexType())
          type_size = sizeof(TTEXTURE_VERTEX);
        if (vertex_type[i] == TCOLOR_TEXTURE_VERTEX::GetVertexType())
          type_size = sizeof(TCOLOR_TEXTURE_VERTEX);
        if (vertex_type[i] == TTEXTURE2_VERTEX::GetVertexType())
          type_size = sizeof(TTEXTURE2_VERTEX);
        if (vertex_type[i] == TTEXTURE2_NORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TTEXTURE2_NORMAL_VERTEX);
        if (vertex_type[i] == TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX);
        if (vertex_type[i] == TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX);
        if (vertex_type[i] == TCOLORED_TEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TCOLORED_TEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX);
        if (vertex_type[i] == TTEXTURE2_RNORMALM_TANGET_BINORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TTEXTURE2_RNORMALM_TANGET_BINORMAL_VERTEX);
        if (vertex_type[i] == TCOLORED_NORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TCOLORED_NORMAL_VERTEX);
        if (vertex_type[i] == TTEXTURE_CUBE_NORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TTEXTURE_CUBE_NORMAL_VERTEX);
        if (vertex_type[i] == TTEXTURE_POLY_VERTEX::GetVertexType())
          type_size = sizeof(TTEXTURE_POLY_VERTEX);
        if (vertex_type[i] == TTEXTURE_POLY_NORMAL_VERTEX::GetVertexType())
          type_size = sizeof(TTEXTURE_POLY_NORMAL_VERTEX);
        // Getting vertex list (vb) and index list (ib)
        void *vertex_list = (void *)malloc(n_vtxs * type_size);
        fread(vertex_list, n_vtxs * type_size, 1, f);
        unsigned short n_idxs;
        fread(&n_idxs, sizeof n_idxs, 1, f);
        void *index_list = (void *)malloc(n_idxs * sizeof(unsigned short));
        fread(index_list , n_idxs * sizeof(unsigned short), 1, f);
        // Calculate Tangent and binormal
        if (vertex_type[i] == TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexType()) {
          CalcTangentsAndBinormals(vertex_list, (unsigned short *)index_list, n_vtxs, n_idxs, type_size, 0, 12, 28, 44, 60);
        }
        if (vertex_type[i] == TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexType()) {
          CalcTangentsAndBinormals(vertex_list, (unsigned short *)index_list, n_vtxs, n_idxs, type_size, 0, 12, 28, 44, 64);
        }
        if (vertex_type[i] == TTEXTURE2_RNORMALM_TANGET_BINORMAL_VERTEX::GetVertexType()) {
          CalcTangentsAndBinormals(vertex_list, (unsigned short *)index_list, n_vtxs, n_idxs, type_size, 0, 12, 28, 44, 60);
        }
        // Creating Renderable vertex
        CRenderableVertexs *l_rv = NULL;
        if (vertex_type[i] == TTEXTURE_NORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TTEXTURE_NORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TCOLORED_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TCOLORED_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TCOLOR_TEXTURE2_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TCOLOR_TEXTURE2_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TCOLOR_TEXTURE_NORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TCOLOR_TEXTURE_NORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TCOLOR_NORMAL_DIFFUSE_TEXTURE2_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TCOLOR_NORMAL_DIFFUSE_TEXTURE2_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TTEXTURE_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TTEXTURE_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TCOLOR_TEXTURE_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TCOLOR_TEXTURE_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TTEXTURE2_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TTEXTURE2_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TTEXTURE2_NORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TTEXTURE2_NORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TCOLORED_TEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TCOLORED_TEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TTEXTURE2_RNORMALM_TANGET_BINORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TTEXTURE2_RNORMALM_TANGET_BINORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TCOLORED_NORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TCOLORED_NORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TTEXTURE_CUBE_NORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TTEXTURE_CUBE_NORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TTEXTURE_POLY_NORMAL_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TTEXTURE_POLY_NORMAL_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        if (vertex_type[i] == TTEXTURE_POLY_VERTEX::GetVertexType())
          l_rv = new CIndexedVertexs<TTEXTURE_POLY_VERTEX>(GRAPHM, vertex_list, index_list, n_vtxs, n_idxs);
        m_RVs.push_back(l_rv);
        delete(vertex_list);
        delete(index_list);
      }
      /*BoundingBox y BoundingSphere, 10 floats (6 BBox + 4 BShpere)
      BoundingBox			BoundingSphere
      0- xmin				6- center xmin
      1- ymin				7- center ymin
      2- zmin				8- center zmin
      3- xmax				9- radius
      4- ymax
      5- zmax
      */
      float *float_list = (float *)malloc(10 * sizeof(float));
      fread(float_list , 10 * sizeof(float), 1, f);
      m_BBox = BoundingBox();
      m_BBox.SetMinPos(Vect3f(float_list[0], float_list[1], float_list[2]));
      m_BBox.SetMaxPos(Vect3f(float_list[3], float_list[4], float_list[5]));
      m_BSphere = BoundingSphere();
      m_BSphere.SetCenterPos(Vect3f(float_list[6], float_list[7], float_list[8]));
      m_BSphere.SetRadius(float_list[9]);
      delete(float_list);
      unsigned short footer;
      fread(&footer, sizeof footer, 1, f);
      if (footer != 0xff55) {
        fclose(f);
        return false;
      } else
        fclose(f);
      return true;
    }
    fclose(f);
    return false;
  }
  return false;
}
bool CStaticMesh::ReLoad () {
  Destroy();
  m_deleted = false;
  return Load(m_FileName);
}
void CStaticMesh::Render (CGraphicsManager *RM) {
  //CEffectTechnique *l_EffectTechnique=EFFECTM->GetEffectTechnique("BasicTechnique");
  //CEffectTechnique *l_EffectTechnique = EFFECTM->GetEffectTechnique("LightsTechnique");
  //CEffectTechnique *l_EffectTechnique = EFFECTM->GetEffectTechnique("NormalMapTechnique");
  //CEffectTechnique *l_EffectTechnique = EFFECTM->GetEffectTechnique("CubeMapTechnique");
  // CEffectTechnique *l_EffectTechnique = EFFECTM->GetEffectTechnique("LightMapTechnique");
  for (int i = 0; i < m_RVs.size(); ++i) {
    // TODO iterate m_textures
    // TODO cambiar a esto
    // TODO modificar esto para que coja el effectTechnique del TechniqueManager a partir del vertexType
    //std::string l_EffectName = EFFECTM->GetTechniqueEffectNameByVertexDefault(m_RVs[i]->GetVertexType());
    //CEffectTechnique *l_EffectTechnique =EFFECTM->GetEffectTechnique(l_EffectName);
    CEffectTechnique *l_EffectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(m_RVs[i]->GetVertexType()))->GetEffectTechnique();
    for (size_t j = 0; j < m_Textures[i].size(); ++j)
      m_Textures[i][j]->Activate(j);
    /*if ((m_RVs[i]->GetVertexType() & VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1)
        m_Textures[i][0]->Activate(0);
    if ((m_RVs[i]->GetVertexType() & VERTEX_TYPE_TANGENT) == VERTEX_TYPE_TANGENT)
        m_Textures[i][1]->Activate(1);
    if ((m_RVs[i]->GetVertexType() & VERTEX_TYPE_CUBE) == VERTEX_TYPE_CUBE)
        m_Textures[i][1]->Activate(2);
    if ((m_RVs[i]->GetVertexType() & VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2)
        m_Textures[i][1]->Activate(3);
    if ((m_RVs[i]->GetVertexType() & VERTEX_TYPE_DIFFUSE) == VERTEX_TYPE_DIFFUSE)
        RM->GetDevice()->SetTexture(0, 0);*/
    m_RVs[i]->Render(RM, l_EffectTechnique);
  }
}


bool CStaticMesh::GetRenderableObjectTechnique() {
  /*
  CRenderableObjectTechniqueManager *l_ROTM= C3DEngine::GetInstance().GetRenderableObjectsTechniqueManager();
  bool l_Ok=true;
  for(size_t i=0;i<m_VertexTypes.size();++i)
  {
  	if(m_RenderableObjectTechniqueName=="")
  		m_RenderableObjectTechniqueName=l_ROTM->GetRenderableObjectTechniqueNameByVertexType(m_VertexTypes[i]);

  	CRenderableObjectTechnique *l_ROT=l_ROTM->GetResource(m_RenderableObjectTechniqueName);
  	m_RenderableObjectTechniques.push_back(l_ROT);
  	if(l_ROT==NULL)
  		Info("Error trying to GetRenderableObjectTechnique '%s' on CStaticMesh", m_RenderableObjectTechniqueName.c_str());
  	l_Ok=l_Ok && l_ROT!=NULL;
  }
  return l_Ok;*/
  return true;
}
