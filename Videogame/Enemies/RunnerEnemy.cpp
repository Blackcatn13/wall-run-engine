#include "RunnerEnemy.h"
#include "Utils\PhysicUserData.h"

//Nuevo sistema
CRunnerEnemy::CRunnerEnemy(CRenderableObject *renderableObject, float speed, std::string fsmName, Vect2f characterSize, float AttackDistance) :
  CEnemy(renderableObject, speed, 10.0f, NULL, characterSize, 0.0f) {
  Init(fsmName);
  //InitWpVector(numWp, distWp);
  //m_PhysicUserData->SetMyCollisionGroup()
}



void CRunnerEnemy::Update(float elapsedTime) {
  UpdateFSM(elapsedTime);
}


