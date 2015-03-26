#include "EasyEnemy.h"
#include "XML\XMLTreeNode.h"
#include "Utils\PhysicUserData.h"
#include "Core\Core.h"
#include "PhysicsManager.h"

#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "AI\AIController.h"


CEasyEnemy::CEasyEnemy(CXMLTreeNode &info1) : CEnemy(info1) {
  Init();
}

CEasyEnemy::CEasyEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw) :
  CEnemy(mesh, name, position, speed, turnSpeed, gravity, yaw) {
  Init();
}


void CEasyEnemy::Init() {
  CRenderableObject *malla = RENDLM->GetDefaultRenderableObjectManager()->GetResource(getMesh());
  malla->SetYaw(m_fYaw);
  bool visible = malla->getVisible();
  malla->SetPosition(m_Position);
}

void CEasyEnemy::Update(float elapsedTime) {
}

void CEasyEnemy::Render() {
}