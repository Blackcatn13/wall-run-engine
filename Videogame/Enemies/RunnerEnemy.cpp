#include "RunnerEnemy.h"

//Nuevo sistema
CRunnerEnemy::CRunnerEnemy(CRenderableObject *renderableObject, float speed, std::string fsmName, Vect2f characterSize) :
  CEnemy(renderableObject, speed, 0.0f, NULL, characterSize) {
  Init(fsmName);
  //InitWpVector(numWp, distWp);
}



void CRunnerEnemy::Update(float elapsedTime) {
  UpdateFSM(elapsedTime);
}


