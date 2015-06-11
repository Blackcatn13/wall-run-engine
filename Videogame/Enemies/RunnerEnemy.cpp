#include "RunnerEnemy.h"
#include "Utils\PhysicUserData.h"

//Nuevo sistema
CRunnerEnemy::CRunnerEnemy(CRenderableObject *renderableObject, float speed, std::string fsmName, Vect2f characterSize, float AttackDistance, float zone) :
  CEnemy(renderableObject, speed, 10.0f, NULL, characterSize, 0.0f, zone) {
  Init(fsmName);
  //InitWpVector(numWp, distWp);
  //m_PhysicUserData->SetMyCollisionGroup()
}



void CRunnerEnemy::Update(float elapsedTime) {
  UpdateFSM(elapsedTime);
}


