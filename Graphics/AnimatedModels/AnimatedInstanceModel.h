#ifndef CANIMATED_INSTANCE_MODEL_H
#define CANIMATED_INSTANCE_MODEL_H


#include "cal3d\global.h"
#include "cal3d\coremodel.h"
#include "cal3d\model.h"
#include "AnimatedCoreModel.h"
#include <d3dx9.h>
#include "Renderable\RenderableObject.h"
#include <vector>
#include "Texture\Texture.h"
#include "Math\Vector3.h"

class CEffectTechnique;

class CAnimatedInstanceModel : public CRenderableObject {
 private:
  CalModel *m_CalModel;
  CAnimatedCoreModel *m_AnimatedCoreModel;
  std::vector<CTexture *> m_TextureList;
  LPDIRECT3DVERTEXBUFFER9 m_pVB;
  LPDIRECT3DINDEXBUFFER9 m_pIB;

  CEffectTechnique *m_EffectTechnique;
  int m_NumVtxs;
  int m_NumFaces;
  //bool LoadVertexBuffer(CGraphicsManager *RM);
  void LoadTextures();
  float m_oldPosition;
 public:
  CAnimatedInstanceModel();
  ~CAnimatedInstanceModel();
  void Render(CGraphicsManager *RM);
  void RenderModelBySoftware(CGraphicsManager *RM);
  void RenderModelByHardware(CGraphicsManager *RM);
  void Update(float ElapsedTime);
  void Initialize(CAnimatedCoreModel *AnimatedCoreModel, CGraphicsManager *RM);
  void Destroy();
  void ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget = 1.0f, bool AutoLock = true);
  void BlendCycle(int Id, float Weight, float DelayIn);
  void ClearCycle(int Id, float DelayOut);
  void RemoveAction(int Id);
  bool IsCycleAnimationActive(int Id) const;
  bool IsActionAnimationActive(int Id) const;
  Vect3f GetBoneMovement();
  Vect3f GetBonePosition();
  Vect3f GetAnimationBonePosition();
};

#endif

//****************************************************************************//
