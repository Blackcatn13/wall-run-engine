#pragma once
#ifndef INC_PHYSICS_SKELETON_H_
#define INC_PHYSICS_SKELETON_H_

//base.h
#include "Utils\BaseUtils.h"
#include "Utils\BoundingBox.h"
#include "Utils\BaseControl.h"
#include "Math\Matrix44.h"

class CPhysxBone;
class CalSkeleton;
class CalModel;
class CPhysicFixedJoint;
class CPhysicSphericalJoint;
class CPhysicRevoluteJoint;
class CXMLTreeNode;
struct SSphericalLimitInfo;
//class CGameEntity;
class CObject3D;

class CPhysxSkeleton : public CBaseControl
{
public:
  CPhysxSkeleton(bool _bActive) : m_pCalSkeleton(0), m_bRagdollActive(_bActive),m_pEntity(0) {};
  ~CPhysxSkeleton() {Done();};

  virtual void Release();

  bool              Init(const std::string& _szFileName, CalModel* _pCalModel, Mat44f _vMat, int _iColisionGroup, CObject3D *_pEntity = 0);
  const CalSkeleton* GetSkeleton()                        {return m_pCalSkeleton;};
  void              SetSkeleton(CalSkeleton* _pSkeleton)  {m_pCalSkeleton = _pSkeleton;};
  const Mat44f&     GetTransform()                 {return m_mTransform;};
  void              SetTransform(const Mat44f& _mTransform) {m_mTransform = _mTransform;};
  void              SetTransformAfterUpdate(const Mat44f& _mTransform);
  CBoundingBox      ComputeBoundingBox();

  bool              IsRagdollActive()                     {return m_bRagdollActive;};  
  //void              ActivateRagdoll()                     {m_bRagdollActive = true;};
  //void              ActivateCal3d()                       {m_bRagdollActive = false;};
  void              SetRagdollActive(bool _bRagdollActive);
  void              WakeUpPhysxBones();
  void              SleepPhysxBones();
  void              SetCollisions(bool _bValue);
  bool              IsRagdollPhysXActor(const std::string& _szName);
  CPhysxBone*       GetPhysxBoneByName(const std::string& _szName);

  void              SetContactReportFlags(unsigned int _uiFlags);
  void              SetContactReportThreshold(float _fThreshold);

  void Update();


private:
  //Funcions per inicialitzar el esquelet corresponent
  bool              Load(const std::string& _szFileName);
  void              InitParents();
  bool              InitPhysXJoints(const std::string& _szFileName);

  //Funcions per afegir joints al Skeleton.
  bool				              AddSphericalJoint(CXMLTreeNode _XMLObjects);
  bool				              AddFixedJoint(CXMLTreeNode _XMLObjects);
  bool				              AddRevoluteJoint(CXMLTreeNode _XMLObjects);
  SSphericalLimitInfo				GetJointParameterInfo(CXMLTreeNode _XMLObjects);

  void UpdateCal3dFromPhysx();
  void UpdatePhysxFromCal3d();

  CalSkeleton*							m_pCalSkeleton;
  Mat44f								m_mTransform;
  std::vector<CPhysxBone*>				m_vBones;
  std::vector<CPhysicFixedJoint*>		m_vFixedJoints;
  std::vector<CPhysicSphericalJoint*>	m_vSphericalJoints;
  std::vector<CPhysicRevoluteJoint*>	m_vRevoluteJoints;
  bool									m_bRagdollActive;
  CObject3D*							m_pEntity;
};

#endif 

