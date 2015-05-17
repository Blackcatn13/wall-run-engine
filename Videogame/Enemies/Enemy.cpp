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
#include "AI\FSMInstance.h"

CEnemy::CEnemy(CXMLTreeNode &info1)
  : CAIController(info1.GetPszISOProperty("mesh", "", false),
                  info1.GetPszISOProperty("name", "", false),
                  info1.GetVect3fProperty("position", v3fZERO, false),
                  info1.GetFloatProperty("speed", .0f, false),
                  info1.GetFloatProperty("turn_speed", .0f, false),
                  info1.GetFloatProperty("gravity", .0f, false)),
    // m_RenderableObject(NULL),
    m_CurrentTime(0.0f)

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

CEnemy::CEnemy(CRenderableObject *renderableObject, float speed, float turnSpeed, float life, Vect2f controller_size):
  //m_RenderableObject(renderableObject),
  m_Life(life),
// m_Speed(speed),
  m_CurrentTime(0.0f),
  m_OriginalPosition(renderableObject->GetPosition()),
  CAIController(renderableObject, speed, turnSpeed, 13.0f, controller_size ) {
  m_fYaw = renderableObject->GetYaw();
  m_Position = renderableObject->GetPosition();
}
CEnemy::CEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw) :
  CAIController(mesh, name, position, speed, turnSpeed, gravity) ,
  //m_RenderableObject(NULL) ,
  m_CurrentTime(0.0f),
  m_OriginalPosition(position) {
  m_fYaw = yaw;
  m_Position = position;

}


void CEnemy::Init(std::string fsmName) {
  /* if (m_RenderableObject == NULL) { //Si se usa el sistema viejo
     CRenderableObject *malla = RENDLM->GetRenderableObjectsManagerByStr("enemies")->GetResource(getMesh());
     malla->SetYaw(m_fYaw);
     bool visible = malla->getVisible();
     malla->SetPosition(m_Position);
   } else { //Si se usa el nuevo
    */ getRenderableObject()->SetYaw(m_fYaw);
  getRenderableObject()->SetPosition(m_Position);
  bool visible = getRenderableObject()->getVisible();

  //}
  if (fsmName != "NoFSM")
    m_Fsm = new CFSMInstance(FSMMGR->GetResource(fsmName)); //TODO: pasar nombre de FSM por el XML de enemyManager
}

void CEnemy::Update(float elapsedTime) {
// UpdateFSM(elapsedTime);
}

void CEnemy::Render() {
}
CEnemy::~CEnemy() {
  CHECKED_DELETE(m_Fsm);
  //CHECKED_DELETE(m_RenderableObject);
}

Vect3f CEnemy::GetOriginalPosition() {
  return m_OriginalPosition;
}

void CEnemy::UpdateFSM(float elapsedTime) {
// for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
// for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
  STATE *s = m_Fsm->getStates().GetResource(m_Fsm->getCurrentState());
  if (s->m_onEnter == false) {
    char l_EnterFunction[256];
    _snprintf_s(l_EnterFunction, 256, 256, "%s(\"%s\")", s->onEnter.c_str(), getName().c_str());
    SCRIPTM->RunCode(l_EnterFunction);
    s->m_onEnter = true;
  }
  s->m_ElapsedTime += elapsedTime;
// char l_InitLevelText[256];
  int doComprobation = 0;
  if (s->m_ElapsedTime >= s->m_UpdateTime) {
    doComprobation = 1;
  }
  char l_UpdateFunction[256];
  _snprintf_s(l_UpdateFunction, 256, 256, "%s(%f,%d,\"%s\")", s->onUpdate.c_str(), elapsedTime, doComprobation, getName().c_str());
  SCRIPTM->RunCode(l_UpdateFunction);
  if (doComprobation == 1) {
    s->m_ElapsedTime = 0;
    doComprobation = 0;
  }
  bool change = m_Fsm->getChanged(); // CLuaGlobals::getInstance()->ValueChanged();
  if (change) {
    s->m_onEnter = false;
    char l_ExitFunction[256];
    _snprintf_s(l_ExitFunction, 256, 256, "%s(\"%s\")", s->onExit.c_str(), getName().c_str());
    SCRIPTM->RunCode(l_ExitFunction);
    m_Fsm->setPreviousState( m_Fsm->getCurrentState());
    m_Fsm->setCurrentState( m_Fsm->getNewState());//CLuaGlobals::getInstance()->getString());
  }

  /* std::stringstream ss;
   ss << "Posicion de " << getName() << ": " << m_RenderableObject->GetPosition().x << ", " << m_RenderableObject->GetPosition().y << ", " << m_RenderableObject->GetPosition().z ;
   std::string str = ss.str();
   CCORE->Trace(str);*/
}



