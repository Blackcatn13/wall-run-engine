#include "AnimatedInstanceModel.h"
#include "cal3d\cal3d_wrapper.h"
#include "cal3d\mixer.h"
#include "AnimatedCoreModel.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "Texture\TextureManager.h"
#include "Utils\Defines.h"
#include "cal3d\renderer.h"
//#include "Utils\XMLTreeNode"


#define CORE_MODEL m_AnimatedCoreModel->GetCoreModel()

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
	CHECKED_DELETE(m_AnimatedCoreModel);
	m_TextureList.clear();
	
}

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel, CGraphicsManager *RM)
{

  m_AnimatedCoreModel = AnimatedCoreModel;
  m_CalModel = new CalModel(CORE_MODEL); 
  LoadVertexBuffer(RM);
  LoadTextures();
}


void CAnimatedInstanceModel::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget=1.0f, bool AutoLock=true)
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
	//TODO => Revisar el update
	m_CalModel->update(ElapsedTime);
}

bool CAnimatedInstanceModel::LoadVertexBuffer(CGraphicsManager *RM)
{
 // Create vertex buffer =>> Pillar los nuestros
  if(FAILED(RM->GetDevice()->CreateVertexBuffer(30000*sizeof(VERTEX),
	  D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, D3DFVF_VERTEX, 
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
}

 void CAnimatedInstanceModel::Render(CGraphicsManager *RM)
 {
	RenderModelBySoftware(RM);
 }

 void CAnimatedInstanceModel::LoadTextures()
 {
	 for(int i=0; i < m_AnimatedCoreModel->GetNumTextures(); ++i)
	 {
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
  if(!pCalRenderer->beginRendering()) return;

  m_NumVtxs=0;
  m_NumFaces=0;

  DWORD dwVBLockFlags=D3DLOCK_NOOVERWRITE;
  DWORD dwIBLockFlags=D3DLOCK_NOOVERWRITE;

  RM->GetDevice()->SetStreamSource( 0, m_pVB, 0,  sizeof(VERTEX) ); //<= Poner lo nuestro
  RM->GetDevice()->SetFVF(D3DFVF_VERTEX);//<= Poner lo nuestro
  RM->GetDevice()->SetIndices(m_pIB);
}