#pragma once
#ifndef INC_PHYSICS_BONE_H_
#define INC_PHYSICS_BONE_H_

//#include "base.h"
#include "Utils\BaseUtils.h"
#include "Utils\Named.h"
#include "Math\Matrix44.h"
#include <vector>
#include "Utils\BaseControl.h"

using namespace std;

class CalBone;
class CPhysicActor;
class CPhysicUserData;
class CXMLTreeNode;
class CalSkeleton;
//class CGameEntity;
class CObject3D;


class CPhysxBone :
  public CNamed,
  public CBaseControl
{
public:
  CPhysxBone(string _szName) : CNamed(_szName),m_pCalBone(0), m_iParentID(-1), m_pActor(0), m_bRoot(true),m_pBoneUserData(0), m_pParent(0),m_iCollisionGroup(0) {};
  ~CPhysxBone() {Done();};

  bool                Init(CalBone* _pBone,Mat44f _vMat44,int _iColisionGroup);
  virtual void        Release();

  void                UpdateCal3dFromPhysx();
  void                UpdatePhysxFromCal3d(const Mat44f& _mTransform);

  //GETERS I SETERS
  CalBone*            GetCalBone()                           {return m_pCalBone;};
  Vect3f              GetMiddlePoint()                       {return m_vMiddlePoint;};
  CPhysicActor*       GetPhysxActor()                        {return m_pActor;};
  const vector<int>   GetChildList()                         {return m_vChildListID;};
  int                 GetParentID()                          {return m_iParentID;};
  void                SetPhysxActor(CPhysicActor* _pActor)   {m_pActor = _pActor;};
  void                SetCalBone(CalBone* _pCalBone)         {m_pCalBone = _pCalBone;};
  void                SetParent(CPhysxBone* _pParent)        {m_pParent = _pParent;};
  const Mat44f&       GetTransform()                         {return m_vMatActor;};
  void                SetTransformAfterUpdate(const Mat44f& _mTransform);
  
  
  bool                IsBoneRoot()                           {return m_bRoot;};
  static Mat44f       GetBoneLeftHandedAbsoluteTransformation(CalBone* _pBone);

  //Funcions per afegir la info del actor de fisica.
  bool				        AddBoxActor(CXMLTreeNode _XMLObjects, CObject3D* _pEntity);
  bool				        AddSphereActor(CXMLTreeNode _XMLObjects, CObject3D* _pEntity);
  bool				        AddCapsuleActor(CXMLTreeNode _XMLObjects, CObject3D* _pEntity);

  
private:

  CPhysxBone*         m_pParent;
  Mat44f              m_vMatAnimatedModel;
  CalBone*            m_pCalBone;
  int                 m_iParentID;
  Vect3f              m_vMiddlePoint;
  bool                m_bRoot;
  vector<int>         m_vChildListID;
  int                 m_iCollisionGroup;

  Mat44f              m_vMatActor;

  CPhysicActor*       m_pActor;
  CPhysicUserData*    m_pBoneUserData;
};

#endif