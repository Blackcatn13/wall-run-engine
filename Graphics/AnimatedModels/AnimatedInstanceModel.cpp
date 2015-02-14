#include "AnimatedInstanceModel.h"
#include "cal3d\cal3d_wrapper.h"
#include "cal3d\mixer.h"
#include "AnimatedCoreModel.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "Texture\TextureManager.h"
#include "Utils\Defines.h"
#include "cal3d\renderer.h"
#include "RenderableVertex\VertexTypes.h"
#include "Effects\Effect.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectTechnique.h"
#include "AnimatedModels\AnimatedModelManager.h"
#include "RenderableVertex\RenderableVertexs.h"

struct VERTEX {
    D3DXVECTOR3    pos;
    D3DXVECTOR3	normal;
    FLOAT tu, tv;
};

#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

//#include "Utils\XMLTreeNode"


#define CORE_MODEL m_AnimatedCoreModel->GetCoreModel()
//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

CAnimatedInstanceModel::CAnimatedInstanceModel()
{
}


CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
    Destroy();
}

void CAnimatedInstanceModel::Destroy()
{
    CHECKED_DELETE(m_CalModel);
    //CHECKED_DELETE(m_AnimatedCoreModel);
    m_TextureList.clear();
}

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel, CGraphicsManager *RM)
{
    m_AnimatedCoreModel = AnimatedCoreModel;
    CalCoreModel *l_CoreModel = CORE_MODEL;
    m_CalModel = new CalModel(l_CoreModel);
    for (int i = 0; i < l_CoreModel->getCoreMeshCount(); ++i)
        m_CalModel->attachMesh(i);
//	LoadVertexBuffer(RM);
    LoadTextures();
    BlendCycle(0, 1.0f, 0.0f);
    m_CalModel->update(0.0f);
}


void CAnimatedInstanceModel::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget, bool AutoLock)
{
    m_CalModel->getMixer()->executeAction(Id, DelayIn, DelayOut);
}

void CAnimatedInstanceModel::BlendCycle(int Id, float Weight, float DelayIn)
{
    m_CalModel->getMixer()->blendCycle(Id, Weight, DelayIn);
}

void CAnimatedInstanceModel::ClearCycle(int Id, float DelayOut)
{
    m_CalModel->getMixer()->clearCycle(Id, DelayOut);
}

void CAnimatedInstanceModel::Update(float ElapsedTime)
{
    //TODO => Revisar el update (onIdle?)
    m_CalModel->update(ElapsedTime);
}

/*bool CAnimatedInstanceModel::LoadVertexBuffer(CGraphicsManager *RM)
{
 // Create vertex buffer =>> Pillar los nuestros
  //if(FAILED(RM->GetDevice()->CreateVertexBuffer(30000*sizeof(VERTEX),
	if(FAILED(RM->GetDevice()->CreateVertexBuffer(30000*sizeof(TTEXTURE_VERTEX),
	  //D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_VERTEX,
	  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, TTEXTURE_VERTEX::GetFVF(),
	  D3DPOOL_DEFAULT , &m_pVB, NULL
	  )))
	  return false;

  // Create index buffer =>> Pillar los nuestros
  if(sizeof(CalIndex)==2)
  {
	  if(FAILED(
         RM->GetDevice()->CreateIndexBuffer(50000*3*sizeof(CalIndex),
			  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,D3DFMT_INDEX16,
			  D3DPOOL_DEFAULT ,&m_pIB, NULL)))
			  return false;
  }
  else
  {
	  if(FAILED(
      RM->GetDevice()->CreateIndexBuffer(50000*3*sizeof(CalIndex),
			  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,D3DFMT_INDEX32,
			  D3DPOOL_DEFAULT ,&m_pIB, NULL)))
			  return false;
  }

  return true;
}*/

void CAnimatedInstanceModel::Render(CGraphicsManager *RM)
{
    //TODO renderObject?
    RM->SetTransform(getTransform());
    RM->DrawAxis(8.0f);
    RenderModelByHardware(RM);
}

void CAnimatedInstanceModel::LoadTextures()
{
    for (int i = 0; i < m_AnimatedCoreModel->GetNumTextures(); ++i) {
        std::string Name = m_AnimatedCoreModel->GetTextureName(i);
        m_TextureList.push_back(TEXTM->GetResource(Name));
    }
}


void CAnimatedInstanceModel::RenderModelBySoftware(CGraphicsManager *RM)
{
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
                m_pVB->Lock(m_NumVtxs * sizeof(VERTEX), pCalRenderer->getVertexCount()*sizeof(VERTEX), (void**)&pVertices, dwVBLockFlags);
                int vertexCount = pCalRenderer->getVerticesNormalsAndTexCoords(&pVertices->x);
                m_pVB->Unlock();
                m_pIB->Lock(m_NumFaces * 3 * sizeof(CalIndex), pCalRenderer->getFaceCount() * 3 * sizeof(CalIndex), (void**)&meshFaces, dwIBLockFlags);
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

void CAnimatedInstanceModel::RenderModelByHardware(CGraphicsManager *RM)
{
    //CEffectManager &l_EffectManager = CCORE->GetEffectManager();
    unsigned short l_Id = CAL3D_HW_VERTEX::GetVertexType();
    CEffectTechnique *l_EffectTechnique = EFFECTM->GetEffectTechnique(EFFECTM->GetTechniqueEffectNameByVertexDefault(CAL3D_HW_VERTEX::GetVertexType()));
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
            l_CalHardwareModel->selectHardwareMesh(hardwareMeshId);
            for (int boneId = 0; boneId < l_CalHardwareModel->getBoneCount(); boneId++) {
                D3DXMatrixRotationQuaternion(&transformation[boneId], (CONST D3DXQUATERNION*)&l_CalHardwareModel->getRotationBoneSpace(boneId,
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
            m_TextureList[l_CalHardwareModel->getVectorHardwareMesh()[hardwareMeshId].meshId]->Activate(0);
            //m_NormalTextureList[0]->Activate(1);
            m_AnimatedCoreModel->GetRenderableVertexs()->Render(GRAPHM, l_EffectTechnique, l_CalHardwareModel->getBaseVertexIndex(), 0,
                    l_CalHardwareModel->getVertexCount(), l_CalHardwareModel->getStartIndex(), l_CalHardwareModel->getFaceCount() * 3);
        }
    }
}