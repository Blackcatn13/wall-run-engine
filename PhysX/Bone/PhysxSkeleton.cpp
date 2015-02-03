#define __DONT_INCLUDE_MEM_LEAKS__
#include "PhysicsManager.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include "PhysicFixedJoint.h"
#include "PhysicSphericalJoint.h"
#include "PhysicRevoluteJoint.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "PhysicUserData.h"
#include "PhysxBone.h"

#include <cal3d/cal3d.h>
#include <XML/XMLTreeNode.h>
#include "RenderManager.h"
#include "Core.h"
#include "base.h"
#include "Math\Matrix44.h"
#include "Logger\Logger.h"
#include "Object3D.h"

//---PhysX Includes---//
#undef min
#undef max
#include "NxPhysics.h"
//---------------------//

bool CPhysxSkeleton::Init( const std::string& _szFileName, CalModel* _pCalModel, Mat44f _vMat, int _iColisionGroup, CObject3D *_pEntity )
{
  m_pEntity = _pEntity;
  m_mTransform = _vMat;
  _pCalModel->getSkeleton()->calculateState();
  SetSkeleton(_pCalModel->getSkeleton());
  vector<CalBone*> l_vLlistaBones = m_pCalSkeleton->getVectorBone();

  //Inicialitzem l'estructura del PhysXSkeleton i creem les instancies dels PhysXBones.
  //Tindrem ja tota la info relativa de bones, pares i fills.
  for (size_t i=0;i<l_vLlistaBones.size();++i)
  {
    CalBone* l_pBone = l_vLlistaBones[i];
    CPhysxBone* l_pPhysXBone = new CPhysxBone(l_pBone->getCoreBone()->getName());
    l_pPhysXBone->Init(l_pBone,_vMat,_iColisionGroup);
    m_vBones.push_back(l_pPhysXBone);
  }

  //Load la info del XML I POSA ELS ACTORS!!!!
  Load(_szFileName);

  InitParents();
  InitPhysXJoints(_szFileName);

  SetOk(true);
  return IsOk();
}

void CPhysxSkeleton::Release()
{
  for (size_t i=0;i<m_vBones.size();++i)
  {
    CHECKED_DELETE(m_vBones[i])
  }

  for (size_t i=0;i<m_vFixedJoints.size();++i)
  {
    CHECKED_DELETE(m_vFixedJoints[i])
  }

  for (size_t i=0;i<m_vRevoluteJoints.size();++i)
  {
    CHECKED_DELETE(m_vRevoluteJoints[i])
  }

  for (size_t i=0;i<m_vSphericalJoints.size();++i)
  {
    CHECKED_DELETE(m_vSphericalJoints[i])
  }
  
  m_vFixedJoints.clear();
  m_vRevoluteJoints.clear();
  m_vSphericalJoints.clear();
  m_vBones.clear();
}


bool CPhysxSkeleton::Load(const std::string& _szFileName)
{
  CXMLTreeNode l_XML;
  CXMLTreeNode l_XMLObjects;
	if(!l_XML.LoadFile(_szFileName.c_str()))
	{
		LOGGER->AddNewLog(ELL_WARNING,"CPhysxRagdoll:: No s'ha trobat el XML \"%s\"", _szFileName.c_str());
		return false;
	}
  
  l_XMLObjects = l_XML(0);
  int l_iNumObjects = l_XMLObjects.GetNumChildren();
  
	for(int i = 0; i < l_iNumObjects; i++)
	{
		CXMLTreeNode l_XMLObject = l_XMLObjects(i);
    string l_szType,l_szName;
 
		if(l_XMLObject.IsComment())
		{
			continue;
		}

    l_szName = l_XMLObject.GetPszISOProperty("name" ,"");
    l_szType = l_XMLObject.GetPszISOProperty("type" ,"");
    
    CPhysxBone* l_pBone = GetPhysxBoneByName(l_szName);

    if (l_pBone!=0)
    {
      if (l_szType=="box")
      {
        l_pBone->AddBoxActor(l_XMLObject, m_pEntity);
      }

      if (l_szType=="sphere")
      {
        l_pBone->AddSphereActor(l_XMLObject, m_pEntity);
      }

      if (l_szType=="capsule")
      {
        l_pBone->AddCapsuleActor(l_XMLObject, m_pEntity);
      }
    }
  }
  

  
  if (!m_bRagdollActive)
  {
    SleepPhysxBones();
  }

  return true;
}


CPhysxBone* CPhysxSkeleton::GetPhysxBoneByName(const std::string& _szName)
{

  CPhysxBone* l_pBone = 0;

  for(size_t i=0;i<m_vBones.size();++i)
  {
    string l_szName;
    l_szName = m_vBones[i]->GetName();
    if (l_szName == _szName)
    {
      l_pBone = m_vBones[i];
      return l_pBone;
    }

  }

  return 0;
}


//Main function pels Joints.
bool CPhysxSkeleton::InitPhysXJoints(const std::string& _szFileName)
{

  CXMLTreeNode l_XML;
  CXMLTreeNode l_XMLObjects;
	if(!l_XML.LoadFile(_szFileName.c_str()))
	{
		LOGGER->AddNewLog(ELL_WARNING,"CPhysxRagdoll:: No s'ha trobat el XML \"%s\"", _szFileName.c_str());
		return false;
	}

  l_XMLObjects = l_XML(1);

  int l_iNumObjects = l_XMLObjects.GetNumChildren();

  
	for(int i = 0; i < l_iNumObjects; i++)
	{
    string l_szType;
    CXMLTreeNode l_XMLObject = l_XMLObjects(i);
    if(l_XMLObject.IsComment())
	  {
		  continue;
	  }

    l_szType = l_XMLObject.GetPszISOProperty("type" ,"");
    
    if (l_szType=="spherical")
    {
      AddSphericalJoint(l_XMLObject);  
    }
    
    if (l_szType=="fixed")
    {
      AddFixedJoint(l_XMLObject);     
    }

    if (l_szType=="revolute")
    {
      AddRevoluteJoint(l_XMLObject); 
    }
  }

  return true;
  
}


bool CPhysxSkeleton::AddSphericalJoint(CXMLTreeNode _XMLObjects)
{
  string l_szActor1,l_szActor2, l_szDirection;

  l_szActor1		= _XMLObjects.GetPszISOProperty("Actor1" ,"");
  l_szActor2		= _XMLObjects.GetPszISOProperty("Actor2" ,"");
  l_szDirection	= _XMLObjects.GetPszISOProperty("Direction" ,"");

  SSphericalLimitInfo l_pJointInfo = GetJointParameterInfo(_XMLObjects);

  CPhysxBone* l_pBone1 = GetPhysxBoneByName(l_szActor1);
  CPhysxBone* l_pBone2 = GetPhysxBoneByName(l_szActor2);

  CPhysicActor* l_pActor1 = 0;
  CPhysicActor* l_pActor2 = 0;
  l_pActor1 = l_pBone1->GetPhysxActor();

  CPhysicSphericalJoint* l_pSphericalJoint = 0;
  l_pSphericalJoint = new CPhysicSphericalJoint();


  CalVector l_vCalVect = l_pBone1->GetCalBone()->getTranslationAbsolute();
  Vect3f l_vJointPoint(-l_vCalVect.x,l_vCalVect.y,l_vCalVect.z);
  l_vJointPoint = m_mTransform*l_vJointPoint;
  Vect3f l_vAxis;
  CalVector l_vVect;

  

  //MES PROVES
  if (l_szDirection == "Out")
  {
    if (l_pBone1->GetChildList().size() > 0)
    {
      int l_pChildId = l_pBone1->GetChildList()[0];
      string l_szNameChild = m_pCalSkeleton->getBone(l_pChildId)->getCoreBone()->getName();
      CPhysxBone* l_pPhysChild = GetPhysxBoneByName(l_szNameChild);
      l_vVect = l_pPhysChild->GetCalBone()->getTranslationAbsolute();
      l_vVect.x = -l_vVect.x;
    /*  l_vAxis = Vect3f(l_vVect.x-l_vJointPoint.x,l_vVect.y-l_vJointPoint.y,l_vVect.z-l_vJointPoint.z);
      l_vAxis.Normalize();*/
    }
    else
    {
      Vect3f l_vMiddle = l_pBone1->GetMiddlePoint();
      l_vVect = CalVector(l_vMiddle.x,l_vMiddle.y,l_vMiddle.z);
      /*l_vAxis(l_vMiddle.x-l_vJointPoint.x,l_vMiddle.y-l_vJointPoint.y,l_vMiddle.z-l_vJointPoint.z);
      l_vAxis.Normalize();*/
    }
  }
  else if (l_szDirection == "In")
  {
    if (!l_pBone1->IsBoneRoot())
    {
      int l_pParentID = l_pBone1->GetParentID();
      string l_szNameParent = m_pCalSkeleton->getBone(l_pParentID)->getCoreBone()->getName();
      CPhysxBone* l_pPhysParent = GetPhysxBoneByName(l_szNameParent);
      CalVector l_vVect = l_pPhysParent->GetCalBone()->getTranslationAbsolute();
      l_vVect.x = -l_vVect.x;
     /* l_vAxis = Vect3f(l_vVect.x-l_vJointPoint.x,l_vVect.y-l_vJointPoint.y,l_vVect.z-l_vJointPoint.z);
      l_vAxis.Normalize();*/
    }
    
  }

  
  Vect3f l_vAxisAux(l_vVect.x,l_vVect.y,l_vVect.z);
  l_vAxisAux = m_mTransform*l_vAxisAux;
  l_vAxis = Vect3f(l_vAxisAux.x-l_vJointPoint.x,l_vAxisAux.y-l_vJointPoint.y,l_vAxisAux.z-l_vJointPoint.z);
  l_vAxis.Normalize();

  l_pJointInfo.m_vAnchor = l_vJointPoint;
  l_pJointInfo.m_vAxis = l_vAxis;



  if (l_szActor2=="NULL")
  {
    //l_pSphericalJoint->SetInfoComplete(l_vJointPoint,l_vAxis,l_pActor1);
    l_pSphericalJoint->SetInfoRagdoll(l_pJointInfo,l_pActor1);
  }
  else
  {
    l_pActor2 = l_pBone2->GetPhysxActor();
    //l_pSphericalJoint->SetInfoComplete(l_vJointPoint,l_vAxis,l_pActor1,l_pActor2);
    l_pSphericalJoint->SetInfoRagdoll(l_pJointInfo,l_pActor1,l_pActor2);
  }
  CORE->GetPhysicsManager()->AddPhysicSphericalJoint(l_pSphericalJoint);
  m_vSphericalJoints.push_back(l_pSphericalJoint);

	return true;
}


bool CPhysxSkeleton::AddFixedJoint(CXMLTreeNode _XMLObjects)
{
  string l_szActor1,l_szActor2, l_szDirection;

  l_szActor1		= _XMLObjects.GetPszISOProperty("Actor1" ,"");
  l_szActor2		= _XMLObjects.GetPszISOProperty("Actor2" ,"");
  l_szDirection	= _XMLObjects.GetPszISOProperty("Direction" ,"");

  CPhysxBone* l_pBone1 = GetPhysxBoneByName(l_szActor1);
  CPhysxBone* l_pBone2 = GetPhysxBoneByName(l_szActor2);

  CPhysicActor* l_pActor1 = 0;
  CPhysicActor* l_pActor2 = 0;

  CPhysicFixedJoint* l_pFixedJoint = 0;
  l_pFixedJoint = new CPhysicFixedJoint();
  l_pActor1 = l_pBone1->GetPhysxActor();

  if (l_szActor2=="NULL")
  {
    l_pFixedJoint->SetInfo(l_pActor1);
  }
  else
  {
    l_pActor2 = l_pBone2->GetPhysxActor();
    l_pFixedJoint->SetInfo(l_pActor1,l_pActor2);
  }

  CORE->GetPhysicsManager()->AddPhysicFixedJoint(l_pFixedJoint);
  m_vFixedJoints.push_back(l_pFixedJoint);

	return true;
}

bool CPhysxSkeleton::AddRevoluteJoint(CXMLTreeNode _XMLObjects)
{
	return true;
}

SSphericalLimitInfo CPhysxSkeleton::GetJointParameterInfo(CXMLTreeNode _XMLObjects)
{
  SSphericalLimitInfo l_sInfo;

  bool l_bTwistLimitLow = _XMLObjects.ExistsKey("TwistLimitLow");
  bool l_bTwistLimitHigh = _XMLObjects.ExistsKey("TwistLimitHigh");

  l_sInfo.JointSpring = _XMLObjects.ExistsKey("JointSpring");
  l_sInfo.TwistSpring = _XMLObjects.ExistsKey("TwistSpring");
  l_sInfo.SwingSpring = _XMLObjects.ExistsKey("SwingSpring");
  l_sInfo.SwingLimit = _XMLObjects.ExistsKey("SwingLimit");

  if (l_sInfo.JointSpring)
  {
    Vect2f l_vJointSpring = _XMLObjects.GetVect2fProperty("JointSpring",v2fZERO,false);
    l_sInfo.JointSpringValue = l_vJointSpring.x;
    l_sInfo.JointSpringDamper = l_vJointSpring.y;
  }

  if (l_sInfo.TwistSpring)
  {
    Vect2f l_vTwistSpring = _XMLObjects.GetVect2fProperty("TwistSpring",v2fZERO,false);
    l_sInfo.TwistSpringValue = l_vTwistSpring.x;
    l_sInfo.TwistSpringDamper = l_vTwistSpring.y;
  }

  if (l_sInfo.SwingSpring)
  {
    Vect2f l_vSwingSpring = _XMLObjects.GetVect2fProperty("SwingSpring",v2fZERO,false);
    l_sInfo.SwingSpringValue = l_vSwingSpring.x;
    l_sInfo.SwingSpringDamper = l_vSwingSpring.y;
  }

  if (l_sInfo.SwingLimit)
  {
    Vect2f l_vSwingLimit = _XMLObjects.GetVect2fProperty("SwingLimit",v2fZERO,false);
    l_sInfo.SwingValue = l_vSwingLimit.x;
    l_sInfo.SwingRestitution = l_vSwingLimit.y;
  }

  if (l_bTwistLimitLow && l_bTwistLimitHigh)
  {
    l_sInfo.TwistLimit = true;
    Vect2f l_vTwistLimitLow = _XMLObjects.GetVect2fProperty("TwistLimitLow",v2fZERO,false);
    Vect2f l_vTwistLimitHigh = _XMLObjects.GetVect2fProperty("TwistLimitHigh",v2fZERO,false);

    l_sInfo.TwistLowValue = l_vTwistLimitLow.x;
    l_sInfo.TwistLowRestitution = l_vTwistLimitLow.y;
    l_sInfo.TwistHighValue = l_vTwistLimitHigh.x;
    l_sInfo.TwistHighRestitution = l_vTwistLimitHigh.y;
  }
  else
  {
    l_sInfo.TwistLimit = false;
  }
  
  return l_sInfo;
}

void CPhysxSkeleton::UpdateCal3dFromPhysx()
{
  for(size_t i=0;i<m_vBones.size();++i)
  {
    m_vBones[i]->UpdateCal3dFromPhysx();
  }

  m_mTransform = GetPhysxBoneByName("Bip01 Pelvis")->GetTransform();
  m_pCalSkeleton->calculateState();
}

void CPhysxSkeleton::InitParents()
{
 

  for(size_t i=0;i<m_vBones.size();++i)
  {
    if (!m_vBones[i]->IsBoneRoot())
    {
      int l_iParentId = m_vBones[i]->GetParentID();
      if (l_iParentId != -1)
      {
        string l_szNameParent = m_pCalSkeleton->getBone(l_iParentId)->getCoreBone()->getName();
        CPhysxBone* l_pParentPhysBone = GetPhysxBoneByName(l_szNameParent);
        m_vBones[i]->SetParent(l_pParentPhysBone);
      }
      else
      {
        m_vBones[i]->SetParent(0);
      }
    }
    else
    {
      m_vBones[i]->SetParent(0);
    }
  }


}

void CPhysxSkeleton::UpdatePhysxFromCal3d()
{

  for(size_t i=0;i<m_vBones.size();++i)
  {
    m_vBones[i]->UpdatePhysxFromCal3d(m_mTransform);
  }

}


void CPhysxSkeleton::Update()
{
  if (IsRagdollActive())
  {
    UpdateCal3dFromPhysx();
  }
  else
  {
    UpdatePhysxFromCal3d();
  }

}


void CPhysxSkeleton::SetRagdollActive(bool _bRagdollActive)
{
  m_bRagdollActive = _bRagdollActive;

  if(m_bRagdollActive)
  {
    //SleepPhysxBones();
    WakeUpPhysxBones();
  }
  else
  {
    SleepPhysxBones();
  }
};

void CPhysxSkeleton::SetCollisions(bool _bValue)
{
  if(_bValue)
  {
    for(size_t i=0;i<m_vBones.size();++i)
    {
      CPhysicActor* l_pActor = m_vBones[i]->GetPhysxActor();

      if (l_pActor != 0)
      { 
        l_pActor->GetPhXActor()->clearActorFlag(NX_AF_DISABLE_COLLISION);
      }
    }
  }else{
    for(size_t i=0;i<m_vBones.size();++i)
    {
      CPhysicActor* l_pActor = m_vBones[i]->GetPhysxActor();

      if (l_pActor != 0)
      { 
        l_pActor->GetPhXActor()->raiseActorFlag(NX_AF_DISABLE_COLLISION);
      }
    }
  }
}

void CPhysxSkeleton::WakeUpPhysxBones()
{
  for(size_t i=0;i<m_vBones.size();++i)
  {
    CPhysicActor* l_pActor = m_vBones[i]->GetPhysxActor();

    if (l_pActor != 0)
    {
      ////l_pActor->GetPhXActor()->setSleepAngularVelocity(0.0f);
      //l_pActor->GetPhXActor()->setSleepEnergyThreshold(0.0f);
      ////l_pActor->GetPhXActor()->setSleepLinearVelocity(0.0f);
      ////l_pActor->GetPhXActor()->raiseBodyFlag(NX_);
      //l_pActor->GetPhXActor()->clearBodyFlag(NX_BF_DISABLE_GRAVITY);
      //l_pActor->GetPhXActor()->wakeUp();
      
      l_pActor->GetPhXActor()->clearBodyFlag(NX_BF_KINEMATIC);
      //l_pActor->GetPhXActor()->clearActorFlag(NX_AF_DISABLE_COLLISION);
    }
  }
};

void CPhysxSkeleton::SleepPhysxBones()
{
  for(size_t i=0;i<m_vBones.size();++i)
  {
    CPhysicActor* l_pActor = m_vBones[i]->GetPhysxActor();

    if (l_pActor != 0)
    {
      //l_pActor->GetPhXActor()->putToSleep();
      //l_pActor->GetPhXActor()->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
      
      l_pActor->GetPhXActor()->raiseBodyFlag(NX_BF_KINEMATIC);
      //l_pActor->GetPhXActor()->raiseActorFlag(NX_AF_DISABLE_COLLISION);
    }
  }
};

bool CPhysxSkeleton::IsRagdollPhysXActor(const std::string& _szName)
{
  for(size_t i=0;i<m_vBones.size();++i)
  {
    CPhysicActor* l_pActor = m_vBones[i]->GetPhysxActor();

    if (l_pActor != 0)
    {
      if (_szName == l_pActor->GetUserData()->GetName())
      {
        return true;
      }
    }
  }

  return false;
}

void CPhysxSkeleton::SetContactReportFlags(unsigned int _uiFlags)
{
  for(size_t i=0;i<m_vBones.size();++i)
  {
    CPhysicActor* l_pActor = m_vBones[i]->GetPhysxActor();

    if (l_pActor != 0)
    {
      l_pActor->SetContactReportFlags(_uiFlags);
    }
  }
}

void CPhysxSkeleton::SetContactReportThreshold(float _fThreshold)
{
  for(size_t i=0;i<m_vBones.size();++i)
  {
    CPhysicActor* l_pActor = m_vBones[i]->GetPhysxActor();

    if (l_pActor != 0)
    {
      l_pActor->SetContactReportThreshold(_fThreshold);
    }
  }
}

void CPhysxSkeleton::SetTransformAfterUpdate(const Mat44f& _mTransform)
{
  m_mTransform = _mTransform;

  for(size_t i=0;i<m_vBones.size();++i)
  {
    m_vBones[i]->SetTransformAfterUpdate(m_mTransform);
  }
}

CBoundingBox CPhysxSkeleton::ComputeBoundingBox()
{
  vector<CPhysxBone*>::iterator l_it  = m_vBones.begin();
  vector<CPhysxBone*>::iterator l_end = m_vBones.end();
  Mat44f m;
  bool m_bFirst = true;
  Vect3f v, min, max;

  for(; l_it != l_end; ++l_it)
  {
    CPhysicActor* l_pPA = (*l_it)->GetPhysxActor();
    if(l_pPA)
    {
      l_pPA->GetMat44(m);
      v = m.GetTranslationVector();
      v += (*l_it)->GetMiddlePoint();
      if(m_bFirst)
      {
        min = max = v;
        m_bFirst = false;
      }
      else
      {
        min.x = (min.x < v.x) ? min.x : v.x;
        min.y = (min.y < v.y) ? min.y : v.y;
        min.z = (min.z < v.z) ? min.z : v.z;

        max.x = (max.x > v.x) ? max.x : v.x;
        max.y = (max.y > v.y) ? max.y : v.y;
        max.z = (max.z > v.z) ? max.z : v.z;
      }
    }
  }

  CBoundingBox l_BB;
  l_BB.Init(min, max);

  return l_BB;
}
