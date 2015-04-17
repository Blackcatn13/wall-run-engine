#include "Enemy.h"
#include "Math\Vector3.h"
#include "XML\XMLTreeNode.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "Actor\PhysicController.h"
#include "Core\ScriptManager.h"
#include "Utils\LuaGlobals.h"
#include "Utils\Defines.h"

CEnemy::CEnemy(CXMLTreeNode &info1)
  : CAIController(info1.GetPszISOProperty("mesh", "", false),
                  info1.GetPszISOProperty("name", "", false),
                  info1.GetVect3fProperty("position", v3fZERO, false),
                  info1.GetFloatProperty("speed", .0f, false),
                  info1.GetFloatProperty("turn_speed", .0f, false),
                  info1.GetFloatProperty("gravity", .0f, false)),
    m_RenderableObject(NULL)

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

CEnemy::CEnemy(CRenderableObject *renderableObject):
  m_RenderableObject(renderableObject),
  CAIController(renderableObject, 0.1f, 2.5f, 13.0f ) {
  m_fYaw = m_RenderableObject->GetYaw();
  m_Position = m_RenderableObject->GetPosition();
}
CEnemy::CEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw) :
  CAIController(mesh, name, position, speed, turnSpeed, gravity) ,
  m_RenderableObject(NULL) {
  m_fYaw = yaw;
  m_Position = position;

}


void CEnemy::Init() {
  if (m_RenderableObject == NULL) {
    CRenderableObject *malla = RENDLM->GetRenderableObjectsManagerByStr("enemies")->GetResource(getMesh());
    malla->SetYaw(m_fYaw);
    bool visible = malla->getVisible();
    malla->SetPosition(m_Position);
  } else {
    m_RenderableObject->SetYaw(m_fYaw);
    m_RenderableObject->SetPosition(m_Position);
    bool visible = m_RenderableObject->getVisible();

  }
  m_Fsm = FSMMGR->GetResource("Enemy");
}

void CEnemy::Update(float elapsedTime) {
// UpdateFSM(elapsedTime);
}

void CEnemy::Render() {
}
CEnemy::~CEnemy() {
  //CHECKED_DELETE(m_Fsm);
  //CHECKED_DELETE(m_RenderableObject);
}

void CEnemy::UpdateFSM(float elapsedTime) {
// for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
  STATE *s = m_Fsm->m_States.GetResource(m_Fsm->m_currentState);
  if (s->m_onEnter == false) {
    char l_LuaFunc[256];
    _snprintf_s(l_LuaFunc, 256, 256, "%s(\"%s\")", s->onEnter.c_str(), getName().c_str());
    SCRIPTM->RunCode(l_LuaFunc);

    s->m_onEnter = true;
  }
  s->m_ElapsedTime += elapsedTime;
  char l_InitLevelText[256];
  int doComprobation = 0;
  if (s->m_ElapsedTime >= s->m_UpdateTime) {
    doComprobation = 1;
  }
  _snprintf_s(l_InitLevelText, 256, 256, "%s(%f,%d,\"%s\")", s->onUpdate.c_str(), elapsedTime, doComprobation, getName().c_str());
  SCRIPTM->RunCode(l_InitLevelText);
  if (doComprobation == 1) {
    s->m_ElapsedTime = 0;
    doComprobation = 0;
  }
  bool change = CLuaGlobals::getInstance()->ValueChanged();
  if (change) {
    s->m_onEnter = false;
    char l_LuaExitFunc[256];
    _snprintf_s(l_LuaExitFunc, 256, 256, "%s(\"%s\")", s->onExit.c_str(), getName().c_str());
    SCRIPTM->RunCode(l_LuaExitFunc);

    m_Fsm->m_previousState = m_Fsm->m_currentState;
    m_Fsm->m_currentState = CLuaGlobals::getInstance()->getString();
  }
// }
}


