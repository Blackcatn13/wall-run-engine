#include "EasyEnemy.h"
#include "XML\XMLTreeNode.h"
#include "Utils\PhysicUserData.h"
#include "Core\Core.h"
#include "PhysicsManager.h"
#include <stdlib.h>
#include <time.h>
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "AI\AIController.h"


CEasyEnemy::CEasyEnemy(CXMLTreeNode &info1) : CEnemy(info1),
  m_WpVector(NULL),
  m_CurrentWp(NULL),
  m_CurrentWpId(0) {
  Init("NoFSM");
  InitWpVector(2, 4);
}

CEasyEnemy::CEasyEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw) :
  CEnemy(mesh, name, position, speed, turnSpeed, gravity, yaw),
  m_WpVector(NULL),
  m_CurrentWp(NULL),
  m_CurrentWpId(0) {
  Init("NoFSM");
  InitWpVector(2, 4);
}
//Nuevo sistema
CEasyEnemy::CEasyEnemy(CRenderableObject *renderableObject, int numWp, int distWp, float speed, float life, std::string fsmName) :
  CEnemy(renderableObject, speed, life),
  m_WpVector(NULL),
  m_CurrentWp(NULL),
  m_CurrentWpId(0) {
  Init(fsmName);
  InitWpVector(numWp, distWp);
}



void CEasyEnemy::Update(float elapsedTime) {
  UpdateFSM(elapsedTime);
}

void CEasyEnemy::Render() {
}

void CEasyEnemy::MoveEnemy(float ElapsedTime, Vect3f wp) {
  MoveTo(ElapsedTime, wp);
}

void CEasyEnemy::InitWpVector(int numWp, int max_distance) { //TODO: Que estos valores se metan por XML tb
  float l_AuxX = 0.0f;
  float l_AuxZ = 0.0f;
  srand (time(NULL));
  m_WpVector.clear();
  bool wasSum = false;
  for (int i = 0; i < numWp; ++i) {
	  int random = (rand() % 2);
    if (random == 0 && !wasSum) {
      l_AuxX = rand() % max_distance + m_Position.x;
      l_AuxZ = rand() % max_distance + m_Position.z;
	  wasSum = true;
    } else if(random == 1 || wasSum){
      l_AuxX = rand() % max_distance - m_Position.x;
      l_AuxZ = rand() % max_distance - m_Position.z;
	  wasSum = false;
    }
	else{
		l_AuxX = rand() % max_distance - m_Position.x;
		l_AuxZ = rand() % max_distance + m_Position.z;
	}
    m_WpVector.push_back(Vect3f(l_AuxX, m_Position.y, l_AuxZ));
  }
}

Vect3f CEasyEnemy::GetNextWp() {
  Vect3f  l_NextWp = m_WpVector[m_CurrentWpId];
  ++ m_CurrentWpId;
  if ( m_CurrentWpId == m_WpVector.size())
    m_CurrentWpId = 0;
  return l_NextWp;
}