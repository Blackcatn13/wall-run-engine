#include "Door.h"
#include "Core\ScriptManager.h"
#include "Core\Core.h"


CDoor::CDoor(std::string switchName, std::string coreName, std::string lua_function, Vect3f final_Position)
  : CSceneElement(switchName, coreName),
    m_IsOpening(false) {
}

void CDoor::Update(float dt) {
  if (m_IsOpening) {

  }
}
