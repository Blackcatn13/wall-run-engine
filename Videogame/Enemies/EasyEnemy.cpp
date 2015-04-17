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

CEasyEnemy::CEasyEnemy(CRenderableObject *renderableObject) :
  CEnemy(renderableObject) {
  Init();
}



void CEasyEnemy::Update(float elapsedTime) {
  UpdateFSM(elapsedTime);
}

void CEasyEnemy::Render() {
}

void CEasyEnemy::MoveEnemy(float ElapsedTime, Vect3f wp) {
  MoveTo(ElapsedTime, wp);
}