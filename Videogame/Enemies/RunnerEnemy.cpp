#include "RunnerEnemy.h"
#include "Utils\PhysicUserData.h"

//Nuevo sistema
CRunnerEnemy::CRunnerEnemy(CRenderableObject *renderableObject, float speed, std::string fsmName, Vect2f characterSize) :
  CEnemy(renderableObject, speed, 0.0f, NULL, characterSize) {
  Init(fsmName);
  //InitWpVector(numWp, distWp);
  //m_PhysicUserData->SetMyCollisionGroup()
}



void CRunnerEnemy::Update(float elapsedTime) {
  UpdateFSM(elapsedTime);
}


