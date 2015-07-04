#include "cal3d\cal3d.h"*/
#include "AnimatedInstanceModel.h"
#include "AnimatedCoreModel.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "Texture\TextureManager.h"
#include "Utils\Defines.h"
#include "RenderableVertex\VertexTypes.h"
#include "Effects\Effect.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectTechnique.h"
#include "AnimatedModels\AnimatedModelManager.h"
#include "RenderableVertex\RenderableVertexs.h"
#include "Core_Utils/MemLeaks.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include <list>
#include <vector>

struct VERTEX {
  D3DXVECTOR3    pos;
  D3DXVECTOR3	normal;
  FLOAT tu, tv;
};

#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

//#include "Utils\XMLTreeNode"


#define CORE_MODEL m_AnimatedCoreModel->GetCoreModel()
//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

CAnimatedInstanceModel::CAnimatedInstanceModel() :
  m_CalModel(NULL),
  m_AnimatedCoreModel(NULL),
  m_EffectTechnique(NULL),
  m_NumFaces(0),
  m_NumVtxs(0),
  m_oldYPosition(0),
  m_oldPosition(0) {
}


CAnimatedInstanceModel::~CAnimatedInstanceModel() {
  Destroy();
}

void CAnimatedInstanceModel::Destroy() {
  CHECKED_DELETE(m_CalModel);
  //CHECKED_DELETE(m_AnimatedCoreModel);
  m_TextureList.clear();
}

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel, CGraphicsManager *RM) {
  m_AnimatedCoreModel = AnimatedCoreModel;
  CalCoreModel *l_CoreModel = CORE_MODEL;
  m_CalModel = new CalModel(l_CoreModel);
  for (int i = 0; i < l_CoreModel->getCoreMeshCount(); ++i)
    m_CalModel->attachMesh(i);
//	LoadVertexBuffer(RM);
  LoadTextures();
  //BlendCycle(0, 1.0f, 0.0f);
  m_CalModel->update(0.0f);
  m_oldPosition = m_CalModel->getSkeleton()->getCoreSkeleton()->getCoreBone(0)->getTranslation().y;
}


void CAnimatedInstanceModel::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget, bool AutoLock) {
  m_CalModel->getMixer()->executeAction(Id, DelayIn, DelayOut, WeightTarget, AutoLock);
}

void CAnimatedInstanceModel::BlendCycle(int Id, float Weight, float DelayIn) {
  m_CalModel->getMixer()->blendCycle(Id, Weight, DelayIn);
}

void CAnimatedInstanceModel::ClearCycle(int Id, float DelayOut) {
  m_CalModel->getMixer()->clearCycle(Id, DelayOut);
}

void CAnimatedInstanceModel::RemoveAction(int Id) {
  m_CalModel->getMixer()->removeAction(Id);
}

void CAnimatedInstanceModel::Update(float ElapsedTime) {
  //TODO => Revisar el update (onIdle?)
  m_CalModel->update(ElapsedTime);
}

void CAnimatedInstanceModel::Render(CGraphicsManager *RM) {
  //TODO renderObject?
  RM->SetTransform(getTransform());
  RenderModelByHardware(RM);
}

void CAnimatedInstanceModel::LoadTextures() {
  std::vector<std::string> textures = m_AnimatedCoreModel->GetTexturesNames();
  std::vector<std::string> normalTextures = m_AnimatedCoreModel->GetNormalTexturesNames();
  for (size_t i = 0; i < textures.size(); ++i) {
    m_TextureList.push_back(TEXTM->GetResource(textures[i]));
  }
  for (size_t i = 0; i < normalTextures.size(); ++i) {
    m_NormalTextureList.push_back(TEXTM->GetResource(normalTextures[i]));
  }
  m_hasNormalTexture = m_NormalTextureList.size() != 0;
}


void CAnimatedInstanceModel::RenderModelBySoftware(CGraphicsManager *RM) {
  //TODO mirar que hace el renderModel y apañarlo aquí
  // get the renderer of the model
  CalRenderer *pCalRenderer;
  pCalRenderer = m_CalModel->getRenderer();
  // begin the rendering loop
  if (!pCalRenderer->beginRendering()) return;
  //nuevo
  TTEXTURE_VERTEX *pVertices;
  CalIndex *meshFaces;
  m_NumVtxs = 0;
  m_NumFaces = 0;
  DWORD dwVBLockFlags = D3DLOCK_NOOVERWRITE;
  DWORD dwIBLockFlags = D3DLOCK_NOOVERWRITE;
  //RM->GetDevice()->SetStreamSource( 0, m_pVB, 0,  sizeof(VERTEX) ); //<= Poner lo nuestro
  //RM->GetDevice()->SetFVF(D3DFVF_VERTEX);//<= Poner lo nuestro
  RM->GetDevice()->SetStreamSource( 0, m_pVB, 0,  sizeof(VERTEX) ); //<= Poner lo nuestro
  RM->GetDevice()->SetFVF(D3DFVF_VERTEX);//<= Poner lo nuestro
  RM->GetDevice()->SetIndices(m_pIB);
  //para cada submesah:
  // get the number of meshes
  int meshCount;
  meshCount = pCalRenderer->getMeshCount();
  // render all meshes of the model
  int meshId;
  for (meshId = 0; meshId < meshCount; meshId++) {
    // get the number of submeshes
    int submeshCount;
    submeshCount = pCalRenderer->getSubmeshCount(meshId);
    // render all submeshes of the mesh
    int submeshId;
    for (submeshId = 0; submeshId < submeshCount; submeshId++) {
      // select mesh and submesh for further data access
      if (pCalRenderer->selectMeshSubmesh(meshId, submeshId)) {
        m_pVB->Lock(m_NumVtxs * sizeof(VERTEX), pCalRenderer->getVertexCount()*sizeof(VERTEX), (void **)&pVertices, dwVBLockFlags);
        int vertexCount = pCalRenderer->getVerticesNormalsAndTexCoords(&pVertices->x);
        m_pVB->Unlock();
        m_pIB->Lock(m_NumFaces * 3 * sizeof(CalIndex), pCalRenderer->getFaceCount() * 3 * sizeof(CalIndex), (void **)&meshFaces, dwIBLockFlags);
        int faceCount = pCalRenderer->getFaces(meshFaces);
        m_pIB->Unlock();
        //GRAPHM->GetDevice()->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
        m_TextureList[meshId]->Activate(0);
        //GRAPHM->GetDevice()->SetTexture(0,(LPDIRECT3DTEXTURE9)m_TextureList[meshId]);
        GRAPHM->GetDevice()->DrawIndexedPrimitive(
          D3DPT_TRIANGLELIST,
          m_NumVtxs,
          0,
          vertexCount,
          m_NumFaces * 3,
          faceCount
        );
        m_NumVtxs += vertexCount;
        m_NumFaces += faceCount;
        dwIBLockFlags = D3DLOCK_NOOVERWRITE;
        dwVBLockFlags = D3DLOCK_NOOVERWRITE;
      }
    }
  }
  pCalRenderer->endRendering();
}

void CAnimatedInstanceModel::RenderModelByHardware(CGraphicsManager *RM) {
  //CEffectManager &l_EffectManager = CCORE->GetEffectManager();
  unsigned short l_Id = CAL3D_HW_VERTEX::GetVertexType();
  CEffectTechnique *l_EffectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(l_Id))->GetEffectTechnique();
  if (l_EffectTechnique == NULL)
    l_EffectTechnique = m_EffectTechnique;
  if (l_EffectTechnique == NULL)
    return;
  EFFECTM->SetWorldMatrix(getTransform());
  CEffect *m_Effect = l_EffectTechnique->GetEffect();
  if (m_Effect == NULL)
    return;
  LPD3DXEFFECT l_Effect = m_Effect->GetD3DEffect();
  if (l_Effect) {
    l_EffectTechnique->BeginRender();
    CalHardwareModel *l_CalHardwareModel = m_AnimatedCoreModel->GetCalHardwareModel();
    D3DXMATRIX transformation[MAXBONES];
    for (int hardwareMeshId = 0; hardwareMeshId < l_CalHardwareModel->getHardwareMeshCount(); hardwareMeshId++) {
      int textureID = (hardwareMeshId < m_TextureList.size()) ? hardwareMeshId : 0;
      m_TextureList[textureID]->Activate(0);
      if (m_hasNormalTexture)
        m_NormalTextureList[textureID]->Activate(1);
      l_CalHardwareModel->selectHardwareMesh(hardwareMeshId);
      for (int boneId = 0; boneId < l_CalHardwareModel->getBoneCount(); boneId++) {
        D3DXMatrixRotationQuaternion(&transformation[boneId], (CONST D3DXQUATERNION *)&l_CalHardwareModel->getRotationBoneSpace(boneId,
                                     m_CalModel->getSkeleton()));
        CalVector translationBoneSpace = l_CalHardwareModel->getTranslationBoneSpace(boneId, m_CalModel->getSkeleton());
        transformation[boneId]._14 = translationBoneSpace.x;
        transformation[boneId]._24 = translationBoneSpace.y;
        transformation[boneId]._34 = translationBoneSpace.z;
      }
      float l_Matrix[MAXBONES * 3 * 4];
      for (int b = 0; b < l_CalHardwareModel->getBoneCount(); ++b) {
        memcpy(&l_Matrix[b * 3 * 4], &transformation[b], sizeof(float) * 3 * 4);
      }
      l_Effect->SetFloatArray(m_Effect->GetBonesParameter(),	(float *)l_Matrix, (l_CalHardwareModel->getBoneCount()) * 3 * 4);
      //m_TextureList[l_CalHardwareModel->getVectorHardwareMesh()[hardwareMeshId].meshId]->Activate(0);
      //m_NormalTextureList[0]->Activate(1);
      m_AnimatedCoreModel->GetRenderableVertexs()->Render(GRAPHM, l_EffectTechnique, l_CalHardwareModel->getBaseVertexIndex(), 0,
          l_CalHardwareModel->getVertexCount(), l_CalHardwareModel->getStartIndex(), l_CalHardwareModel->getFaceCount() * 3);
    }
  }
}

Vect3f CAnimatedInstanceModel::GetAnimationBonePosition() {
  CalVector CBone = m_CalModel->getSkeleton()->getBone(0)->getTranslation();
  return Vect3f(CBone.x, CBone.y, CBone.z);
}

Vect3f CAnimatedInstanceModel::GetBonePosition() {
  CalVector CBone = m_CalModel->getSkeleton()->getCoreSkeleton()->getCoreBone(0)->getTranslation();
  return Vect3f(CBone.x, CBone.y, CBone.z);
}

Vect3f CAnimatedInstanceModel::GetBoneMovement() {
  CalVector AnimatedBone = m_CalModel->getSkeleton()->getBone(0)->getTranslation();
  float position = AnimatedBone.y - m_oldYPosition;
  m_oldYPosition = AnimatedBone.y;
  if (position < 0.001 && position > -0.001) position = 0;
  return Vect3f(0, position, 0);
}

Vect3f CAnimatedInstanceModel::GetBonesMovement() {
  CalVector AnimatedBone = m_CalModel->getSkeleton()->getBone(0)->getTranslation();
  Vect3f position = Vect3f(AnimatedBone.x, AnimatedBone.y, AnimatedBone.z) - m_oldPosition;
  m_oldPosition = Vect3f(AnimatedBone.x, AnimatedBone.y, AnimatedBone.z);
  if (position.x < 0.001 && position.x > -0.001) position.x = 0;
  if (position.y < 0.001 && position.y > -0.001) position.y = 0;
  if (position.z < 0.001 && position.z > -0.001) position.z = 0;
  return position;
}

void CAnimatedInstanceModel::UpdateSkeleton(float dt) {
  m_CalModel->getMixer()->updateAnimation(dt);
  m_CalModel->getMixer()->updateSkeleton();
}

void CAnimatedInstanceModel::RestartBonesOldPosition() {
  m_oldPosition = Vect3f(0);
}

bool CAnimatedInstanceModel::IsCycleAnimationActive() const {
  std::list< CalAnimationAction *> anim = m_CalModel->getMixer()->getAnimationActionList();
  if (anim.size() > 0)
    return (anim.front()->getState() != CalAnimation::STATE_STOPPED);
  return false;
}

bool CAnimatedInstanceModel::IsActionAnimationActive() const {
  return true;
}