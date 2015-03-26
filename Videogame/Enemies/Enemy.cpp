#include "Enemy.h"
#include "Math\Vector3.h"
#include "XML\XMLTreeNode.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "Actor\PhysicController.h"

CEnemy::CEnemy(CXMLTreeNode &info1)
  :
  m_Mesh((info1.GetPszISOProperty("mesh", "", false)))
  //  CAIController(info1.GetPszISOProperty("mesh", "", false), m_Name, m_Position)
{
  // ask for it to enemyManager
  //m_Life(info2.GetFloatProperty("life"))
  //m_ShootAccuracy(info2.GetFloatProperty("shoot_accuracy"))
  //, m_TimeToShoot(info2.GetFloatProperty("time_to_shoot"))
  //, m_TimeToSpawn(info2.GetFloatProperty("time_to_spawn"))
  m_fYaw = info1.GetFloatProperty("yaw",  .0f, false);
  m_Name = info1.GetPszISOProperty("name", "", false);
  m_Position = info1.GetVect3fProperty("pos", Vect3f(), false);
}

void CEnemy::SetMesh(std::string mesh) {
  CRenderableObject *malla = RENDLM->GetDefaultRenderableObjectManager()->GetResource(mesh);
  malla->SetYaw(m_fYaw);
  Vect3f position = Vect3f(m_PhysicController->GetPosition());
  bool visible = malla->getVisible();
  malla->SetPosition(m_PhysicController->GetPosition());
}

CEnemy::~CEnemy() {
  CHECKED_DELETE(m_PhysicUserData);
  CHECKED_DELETE(m_PhysicController);
}
