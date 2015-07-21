#include "AnimatedCoreModel.h"
#include "AnimatedModelManager.h"
#include "XML\XMLTreeNode.h"
#include "Texture\TextureManager.h"
#include "Core\Core.h"
#include "Utils\Defines.h"
#include "RenderableVertex\RenderableVertexs.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "RenderableVertex\VertexTypes.h"
#include "cal3d\cal3d.h"
#include "cal3d\coremodel.h"
#include "cal3d\hardwaremodel.h"
#include "Core_Utils/MemLeaks.h"


CAnimatedCoreModel::CAnimatedCoreModel()
  : m_CalCoreModel(NULL)
  , m_CalHardwareModel(NULL)
  , m_RenderableVertexs(NULL)
  , m_NumFaces(0)
  , m_NumVtxs(0) {
}


CAnimatedCoreModel::~CAnimatedCoreModel() {
  CHECKED_DELETE(m_CalCoreModel);
  CHECKED_DELETE(m_RenderableVertexs);
  CHECKED_DELETE(m_CalHardwareModel);
}


bool CAnimatedCoreModel::LoadMesh(const std::string &Filename) {
  if (m_CalCoreModel->loadCoreMesh(Filename) == 0)
    return false;
  return true;
}
bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename) {
  if (m_CalCoreModel->loadCoreSkeleton(Filename) == 0)
    return false;
  return true;
}
bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename) {
  int aux = m_CalCoreModel->loadCoreAnimation(Filename, Name);
  if ( aux == -1)
    return false;
  return true;
}


void CAnimatedCoreModel::Load(const std::string &Path) {
  m_Path = Path;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile((Path + std::string("\\actor.xml")).c_str())) {
    printf("ERROR loading the file.");
  }
  CXMLTreeNode  m = newFile["animated_model"];
  if (m.Exists()) {
    std::string l_Name = m.GetPszISOProperty("name", "");
    m_CalCoreModel = new CalCoreModel(l_Name);
    int count = m.GetNumChildren();
    for (int i = 0; i < count; ++i) {
      CXMLTreeNode nodeChild = m.getNextChild();
      std::string l_Element(nodeChild.GetName());
      if ( l_Element == "skeleton") {
        std::string Filename = nodeChild.GetPszISOProperty("file", "");
        LoadSkeleton(Path + "\\" + Filename);
      } else if (l_Element == "animation") {
        std::string name = nodeChild.GetPszISOProperty("name", "");
        std::string Filename = nodeChild.GetPszISOProperty("file", "");
        LoadAnimation(name, Path + "\\" + Filename);
      } else if (l_Element == "mesh") {
        std::string Filename = nodeChild.GetPszISOProperty("file", "");
        LoadMesh(Path + "\\" + Filename);
      } else if (l_Element == "texture") {
        std::string Filename = Path + std::string(nodeChild.GetPszISOProperty("file", ""));
        m_TextureFilenameList.push_back(Filename);
        TEXTM->GetResource(Filename);
      } else if (l_Element == "normal_texture") {
        std::string Filename = Path + std::string(nodeChild.GetPszISOProperty("file", ""));
        m_NormalTextureFilenameList.push_back(Filename);
      }
    }
  }
  LoadVertexBuffer();
}

const std::string &CAnimatedCoreModel::GetTextureName( size_t id ) {
  assert(id <= m_TextureFilenameList.size());
  return m_TextureFilenameList[id];
}

int CAnimatedCoreModel::GetAnimationId(const std::string &AnimationName) const {
  return m_CalCoreModel->getCoreAnimationId(AnimationName);
}

/*
 else if(strKey == "skeleton")
    {
      // load core skeleton
      std::cout << "Loading skeleton '" << strData << "'..." << std::endl;
      if(!m_calCoreModel->loadCoreSkeleton(strData))
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "animation")
    {
      // load core animation
      std::cout << "Loading animation '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreAnimation(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "mesh")
    {
      // load core mesh
      std::cout << "Loading mesh '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreMesh(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
	*/

void CalcTangentsAndBinormalsACM(void *VtxsData, unsigned short *IdxsData, size_t VtxCount,
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

bool CAnimatedCoreModel::LoadVertexBuffer() {
  m_NumVtxs = 0;
  m_NumFaces = 0;
  for (int i = 0; i < m_CalCoreModel->getCoreMeshCount(); ++i) {
    CalCoreMesh *l_CalCoreMesh = m_CalCoreModel->getCoreMesh(i);
    for (int j = 0; j < l_CalCoreMesh->getCoreSubmeshCount(); ++j) {
      CalCoreSubmesh *l_CalCoreSubmesh = l_CalCoreMesh->getCoreSubmesh(j);
      m_NumFaces += l_CalCoreSubmesh->getFaceCount();
      m_NumVtxs += l_CalCoreSubmesh->getVertexCount();
    }
  }
  m_CalHardwareModel = new CalHardwareModel(m_CalCoreModel);
  CAL3D_HW_VERTEX *l_Vtxs = new CAL3D_HW_VERTEX[m_NumFaces * 3];
  //Cogemos el doble de vértices necesarios porque al crear el model de hardware
  //puede necesitar más vértices que el modelo por software
  unsigned short *l_Idxs = new unsigned short[m_NumFaces * 3];
  m_CalHardwareModel->setVertexBuffer((char *) l_Vtxs,	sizeof(CAL3D_HW_VERTEX));
  m_CalHardwareModel->setWeightBuffer(((char *)l_Vtxs) + 12,	sizeof(CAL3D_HW_VERTEX));
  m_CalHardwareModel->setMatrixIndexBuffer(((char *)l_Vtxs) + 28,	sizeof(CAL3D_HW_VERTEX));
  m_CalHardwareModel->setNormalBuffer(((char *)l_Vtxs) + 44,	sizeof(CAL3D_HW_VERTEX));
  m_CalHardwareModel->setTextureCoordNum(1);
  m_CalHardwareModel->setTextureCoordBuffer(0, ((char *)l_Vtxs) + 92,	sizeof(CAL3D_HW_VERTEX));
  m_CalHardwareModel->setIndexBuffer((CalIndex *)l_Idxs);
  m_CalHardwareModel->load( 0, 0, MAXBONES);
  m_NumVtxs = m_CalHardwareModel->getTotalVertexCount();
  //En caso de utilizar NormalMap
  CalcTangentsAndBinormalsACM(l_Vtxs, l_Idxs, m_NumVtxs, m_NumFaces * 3, sizeof(CAL3D_HW_VERTEX), 0, 44, 60, 76, 92);
  m_RenderableVertexs = new CIndexedVertexs<CAL3D_HW_VERTEX>(GRAPHM, l_Vtxs, l_Idxs, m_NumVtxs, m_NumFaces * 3);
  delete []l_Vtxs;
  delete []l_Idxs;
  return true;
}