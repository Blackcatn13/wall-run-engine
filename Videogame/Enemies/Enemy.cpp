#include "Enemy.h"
#include "Math\Vector3.h"
#include "XML\XMLTreeNode.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "Actor\PhysicController.h"

CEnemy::CEnemy(CXMLTreeNode &info1)
  : CAIController(info1.GetPszISOProperty("mesh", "", false),
                  info1.GetPszISOProperty("name", "", false),
                  info1.GetVect3fProperty("position", v3fZERO, false),
                  info1.GetFloatProperty("speed", .0f, false),
                  info1.GetFloatProperty("turn_speed", .0f, false),
                  info1.GetFloatProperty("gravity", .0f, false))

    //  CAIController(info1.GetPszISOProperty("mesh", "", false), m_Name, m_Position)
{
  // ask for it to enemyManager
  //m_Life(info2.GetFloatProperty("life"))
  //m_ShootAccuracy(info2.GetFloatProperty("shoot_accuracy"))
  //, m_TimeToShoot(info2.GetFloatProperty("time_to_shoot"))
  //, m_TimeToSpawn(info2.GetFloatProperty("time_to_spawn"))
// setMesh(info1.GetPszISOProperty("mesh", "", false));
  m_fYaw = info1.GetFloatProperty("yaw",  .0f, false);
  //setName (info1.GetPszISOProperty("name", "", false));
  m_Position = info1.GetVect3fProperty("pos", Vect3f(), false);
}


CEnemy::CEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw) :
  CAIController(mesh, name, position, speed, turnSpeed, gravity) {
  m_fYaw = yaw;
}


void CEnemy::Init() {

}

void CEnemy::Update(float elapsedTime) {
}

void CEnemy::Render() {
}

