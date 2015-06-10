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
#include "Core\PlayerController.h"

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

CEnemy::CEnemy(CRenderableObject *renderableObject, float speed, float turnSpeed, int life, Vect2f controller_size, float AttackDistance):
  //m_RenderableObject(renderableObject),
  m_Life(life),
// m_Speed(speed),
  m_CurrentTime(0.0f),
  m_isAlive(true),
  m_AttackPlayerDistance(AttackDistance),
  m_OriginalPosition(renderableObject->GetPosition()),
  CAIController(renderableObject, speed, turnSpeed, 13.0f, controller_size ) {
  m_fYaw = renderableObject->GetYaw();
  m_Position = renderableObject->GetPosition();
}
CEnemy::CEnemy(std::string mesh, std::string name, Vect3f position,  float speed, float turnSpeed, float gravity, float yaw) :
  CAIController(mesh, name, position, speed, turnSpeed, gravity) ,
  //m_RenderableObject(NULL) ,
  m_CurrentTime(0.0f),
  m_isAlive(true),
  m_AttackPlayerDistance(0.0f),
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
  if (m_Fsm->getonEnter() == false) {
    char l_EnterFunction[256];
    _snprintf_s(l_EnterFunction, 256, 256, "%s(\"%s\")", s->onEnter.c_str(), getName().c_str());
    SCRIPTM->RunCode(l_EnterFunction);
    m_Fsm->setonEnter(true);
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
    m_Fsm->setonEnter(false);
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

void CEnemy::AddDamagePlayer() {
//	PLAYC->SetPosition(Vect3f(PLAYC->GetPosition().x, PLAYC->GetPosition().y, PLAYC->GetPosition().z + 5.0));
  Vect3f damageVector = PLAYC->GetPosition() - GetPosition();
  damageVector.Normalize();
  std::stringstream buffer;
  buffer << "Player:get_instance().player_take_damage(Vect3f(" << damageVector.x << ", " << damageVector.y << ", " << damageVector.z << "))";
  SCRIPTM->RunCode(buffer.str());
}

void CEnemy::AddDamageEnemyMikMik() {
  /*m_Life = m_Life - 1;
  //Diferenciar si se está sltando o no para lanzar la animación de segundo salto o no
  if (m_Life <= 0)
  {
  	m_isAlive = false;
  	getRenderableObject()->setPrintable(false);
  	//m_PhysicController->GetUserData()->GetActor()->Activate(false);
  }*/
  m_Fsm->setNewState("Take_Damage");
}

void CEnemy::AddDamageEnemyPumPum() {
  /*m_Life = m_Life - 1;
  //Diferenciar si se está sltando o no para lanzar la animación de segundo salto o no
  if (m_Life <= 0)
  {
  	m_isAlive = false;
  	getRenderableObject()->setPrintable(false);
  	//m_PhysicController->GetUserData()->GetActor()->Activate(false);
  }*/
  m_Fsm->setNewState("Take_Damage");
}

void CEnemy::ActualizarHitboxEnemigo() {
  switch (CheckPlayerCollision()) {
    case 1:
      if (m_enemyType == MIKMIK) {
        AddDamageEnemyMikMik();
      } else if (m_enemyType == PUMPUM) {
        AddDamageEnemyPumPum();
      }
      break;
    case 2:
      AddDamagePlayer();
      break;
    case 3:
      break;
    default:
      break;
  }
}

void CEnemy::ActualizarDisparo(float dt) {
  if (m_IsOnCooldown) {
    m_CurrentCooldown = m_CurrentCooldown - dt;
    if (m_CurrentCooldown < 0.0) {
      m_IsOnCooldown = false;
      DestruirDisparo();
    } else if (m_CurrentCooldown < (m_CooldownTimer - m_tiempoVidaDisparo)) {
      DestruirDisparo();
    } else {
      m_PosicionBala = m_PosicionBala + m_DireccionBala.Normalize() * m_BalaSpeed * dt;
      RENDLM->GetRenderableObjectsManagerByStr("enemies")->GetResource("disparo" + getName())->SetPosition(m_PosicionBala);
      if (CheckPlayerShotCollision()) {
        DestruirDisparo();
        AddDamagePlayer();
      }
    }
  }
}

void CEnemy::ShotToVector(float dt, Vect3f point) {
  if (m_isAlive) {
    m_PosicionBala = m_Position;
    RENDLM->GetRenderableObjectsManagerByStr("enemies")->GetResource("disparo" + getName())->SetPosition(m_PosicionBala);
    RENDLM->GetRenderableObjectsManagerByStr("enemies")->GetResource("disparo" + getName())->setPrintable(true);
  }
}

void CEnemy::OnlyRotate(float dt, Vect3f point) {
  if (point.Distance(m_Position) >= 2) {
    Vect3f direction = (point - m_Position);
    Vect3f diff = Vect3f(1, 0, 0).RotateY(m_fYaw);
    float angle = getAngleDiff(direction, diff);
    //if (angle > 0.5f)
    RotateYaw(dt, point);
    if ((!m_IsOnCooldown) && angle < 0.2) {
      //DISPARO, cooldown 500 ms
      m_IsOnCooldown = true;
      m_CurrentCooldown = m_CooldownTimer;
      ShotToVector(dt, m_Position);
      m_DireccionBala = direction;
    }
  }
}

int CEnemy::CheckPlayerCollision() {
  //returns:
  //1 if player hits
  //2 if player gets hit
  //0 if no hit
  float l_MargenSuperiorPlayer = 1.5;//HARDCODED
  if (getisAlive() && (m_Position.Distance(PLAYC->GetPosition()) < m_EnemyHitbox)) {
    if (PLAYC->GetPosition().y > m_Position.y + l_MargenSuperiorPlayer || PLAYC->getisAttack()) {
      return 1;
    } else {
      return 2;
    }
  }
  return 0;
}

