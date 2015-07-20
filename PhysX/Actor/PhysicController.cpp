#define __DONT_INCLUDE_MEM_LEAKS__
#include "Utils\BaseUtils.h"
#include "Core\Core.h"
#include "Actor\PhysicController.h"

//---PhysX Includes---//
#undef min
#undef max
#include "NxPhysics.h"
#include "NxController.h"
#include "NxCapsuleController.h"
#include "NxBoxController.h"

#include "Utils\PhysicUserData.h"
#include "Actor\PhysicController.h"
#include "PhysicsManager.h"
#include "Reports\PhysicsControllerHitReport.h"
//---------------------//

//#if defined (_DEBUG)
//#include "Memory\MemLeaks.h"
//#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CPhysicController::CPhysicController ( float _fRadius, float _fHeight, float _fSlope, float _fSkinwidth, float _fStepOffset
                                       , ECollisionGroup _uiCollisionGroups, CPhysicUserData *_pUserData, const Vect3f &_vPos, float _fGravity )
  : m_pPhXController				( NULL )
  , m_pPhXCapsuleControllerDesc	( NULL )
  , m_pPhXBoxControllerDesc		( NULL )
  , m_pUserData					( _pUserData )
  , m_fRadiusControler			( _fRadius )
  , m_fHeightControler			( _fHeight )
  , m_fSlopeLimitControler		( _fSlope )
  , m_fSkinWidthControler			( _fSkinwidth )
  , m_fStepOffsetControler		( _fStepOffset )
  , m_fGravity					( _fGravity )
  , m_uCollisionGroups			( _uiCollisionGroups )
  , m_bUseGravity					( true)
  , m_Type						( ::CAPSULE )

{
  assert( _pUserData );
  m_pUserData->SetController(this);
  //---- Crear un nuevo NxController----
  m_pPhXCapsuleControllerDesc				= new NxCapsuleControllerDesc();
  CPhysicsControllerHitReport *l_Report	= new CPhysicsControllerHitReport();
  m_pReport = l_Report;
  m_pPhXCapsuleControllerDesc->position.x			= _vPos.x;
  m_pPhXCapsuleControllerDesc->position.y			= _vPos.y;
  m_pPhXCapsuleControllerDesc->position.z			= _vPos.z;
  m_pPhXCapsuleControllerDesc->radius				= m_fRadiusControler;
  m_pPhXCapsuleControllerDesc->height				= m_fHeightControler;
  m_pUserData->SetRadius(m_fRadiusControler);
  // Dona la pendent màxima que pot pujar. 0 ho desactiva. En funció del cosinus angle. Per defecte 0.707
  m_pPhXCapsuleControllerDesc->slopeLimit			= cosf(NxMath::degToRad(m_fSlopeLimitControler));
  m_pPhXCapsuleControllerDesc->skinWidth			= m_fSkinWidthControler;	// Dona l'amplada de la pell. Permet donar un marxe per evitar càlculs excesius. Defecte : 0.1
  m_pPhXCapsuleControllerDesc->stepOffset			= m_fStepOffsetControler;	// Alçada màxima que el controler pot pujar. Massa petit + costa de pujar obstacles. Defecte : 0.5
  m_pPhXCapsuleControllerDesc->upDirection		= NX_Y;						// Direcció cap al cel
  m_pPhXCapsuleControllerDesc->callback			= l_Report;					// Es crida quan el controler colisiona
  m_pPhXCapsuleControllerDesc->interactionFlag	= NXIF_INTERACTION_USE_FILTER;	// Diu si el controler colisiona amb altres controlers
}

CPhysicController::CPhysicController ( Vect3f _Dim, float _fSlope, float _fSkinwidth, float _fStepOffset
                                       , ECollisionGroup _uiCollisionGroups, CPhysicUserData *_pUserData, const Vect3f &_vPos, float _fGravity )
  : m_pPhXController				( NULL )
  , m_pPhXCapsuleControllerDesc	( NULL )
  , m_pPhXBoxControllerDesc		( NULL )
  , m_pUserData					( _pUserData )
  , m_vExtensio					( _Dim )
  , m_fSlopeLimitControler		( _fSlope )
  , m_fSkinWidthControler			( _fSkinwidth )
  , m_fStepOffsetControler		( _fStepOffset )
  , m_fGravity					( _fGravity )
  , m_uCollisionGroups			( _uiCollisionGroups )
  , m_bUseGravity					( true)
  , m_Type						( ::BOX )

{
  assert( _pUserData );
  //---- Crear un nuevo NxController----
  m_pPhXBoxControllerDesc = new NxBoxControllerDesc();
  CPhysicsControllerHitReport *l_Report  = new CPhysicsControllerHitReport();
  m_pReport = l_Report;
  m_pPhXBoxControllerDesc->position.x			= _vPos.x;
  m_pPhXBoxControllerDesc->position.y			= _vPos.y;
  m_pPhXBoxControllerDesc->position.z			= _vPos.z;
  m_pPhXBoxControllerDesc->extents.x			= _Dim.x;
  m_pPhXBoxControllerDesc->extents.y			= _Dim.y;
  m_pPhXBoxControllerDesc->extents.z			= _Dim.z;
  m_pPhXBoxControllerDesc->slopeLimit			= cosf(NxMath::degToRad(m_fSlopeLimitControler));
  m_pPhXBoxControllerDesc->skinWidth			= m_fSkinWidthControler;
  m_pPhXBoxControllerDesc->stepOffset			= m_fStepOffsetControler;
  m_pPhXBoxControllerDesc->upDirection		= NX_Y;
  m_pPhXBoxControllerDesc->callback			= l_Report;
  m_pPhXBoxControllerDesc->interactionFlag	= NXIF_INTERACTION_USE_FILTER;
}

CPhysicController::~CPhysicController() {
  //delete m_pPhXControllerDesc->callback;
  CHECKED_DELETE ( m_pPhXCapsuleControllerDesc );
  CHECKED_DELETE ( m_pPhXBoxControllerDesc );
  CHECKED_DELETE ( m_pReport );
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CPhysicController::CreateController ( NxController *_pController, NxScene *_pScene) {
  m_pPhXScene			= _pScene;
  m_pPhXController	= _pController;
  assert ( m_pPhXScene );
  assert ( m_pPhXController );
  CHECKED_DELETE ( m_pPhXBoxControllerDesc );
  CHECKED_DELETE ( m_pPhXCapsuleControllerDesc );
}

void CPhysicController::SetPosition	(const Vect3f &pos) {
  if ( m_pPhXController != NULL ) {
    NxExtendedVec3 position;
    position.x = pos.x;
    position.y = pos.y;
    position.z = pos.z;
    m_pPhXController->setPosition(position);
  } else if (m_pPhXBoxControllerDesc != NULL) {
    if ( GetType() == ::BOX ) {
      m_pPhXBoxControllerDesc->position.x	= pos.x;
      m_pPhXBoxControllerDesc->position.y	= pos.y;
      m_pPhXBoxControllerDesc->position.z	= pos.z;
    } else {
      m_pPhXCapsuleControllerDesc->position.x	= pos.x;
      m_pPhXCapsuleControllerDesc->position.y	= pos.y;
      m_pPhXCapsuleControllerDesc->position.z	= pos.z;
    }
  } else {
    return;
  }
  //CObject3D::m_vPosition = pos;
  //CObject3D::InitMat44();
  CObject3D::SetPosition(pos);
}

Vect3f CPhysicController::GetPosition () {
  Vect3f vec(0, 0, 0);
  if (m_pPhXController != NULL) {
    // Parche Jordi!!
    //NxExtendedVec3 tmp = m_pPhXController->getPosition();
    NxExtendedVec3 tmp = m_pPhXController->getDebugPosition();
    vec.x = (float)tmp.x;
    vec.y = (float)tmp.y;
    vec.z = (float)tmp.z;
  } else if (m_pPhXBoxControllerDesc != NULL) {
    if ( GetType() == ::BOX ) {
      vec.x = (float)m_pPhXBoxControllerDesc->position.x;
      vec.y = (float)m_pPhXBoxControllerDesc->position.y;
      vec.z = (float)m_pPhXBoxControllerDesc->position.z;
    } else {
      vec.x = (float)m_pPhXCapsuleControllerDesc->position.x;
      vec.y = (float)m_pPhXCapsuleControllerDesc->position.y;
      vec.z = (float)m_pPhXCapsuleControllerDesc->position.z;
    }
  }
  return vec;
}

void CPhysicController::Jump(float ammount) {
  m_Jump.StartJump(ammount);
}

void CPhysicController::Move ( const Vect3f &_vDirection, float _ElapsedTime ) {
  assert ( m_pPhXController != NULL );
  float l_fDirectionY = _vDirection.y;
  if ( m_bUseGravity ) {
    l_fDirectionY += ( m_fGravity * _ElapsedTime );
  }
  NxVec3 l_Direction ( _vDirection.x, l_fDirectionY, _vDirection.z);
  NxF32 sharpness = 1.0f;
  NxU32 collisionFlags = 0;
  //NxU32 Collision = 0;
  float heightDelta = m_Jump.GetHeight( _ElapsedTime );
  if ( heightDelta != 0.f ) {
    l_Direction.y += heightDelta;
    /*l_Direction.x *= 0.3f;
    l_Direction.z *= 0.3f;*/
  }
  int mask = 1 << ECG_PLAYER;
  mask |= 1 << ECG_DYNAMIC_OBJECTS;
  mask |= 1 << ECG_STATIC_OBJECTS;
  mask |= 1 << ECG_ESCENE;
  mask |= 1 << ECG_ENEMY;
  mask |= 1 << ECG_LIMITS;
  m_pPhXController->move( l_Direction , mask, 0.0001f, collisionFlags, sharpness );
  if ( ( collisionFlags & NXCC_COLLISION_DOWN ) || ( collisionFlags & NXCC_COLLISION_UP ) ) {
    m_Jump.StopJump();
  }
  NxExtendedVec3 tmp;
  tmp = m_pPhXController->getDebugPosition();
  SetPosition ( Vect3f ( (float) tmp.x, (float) tmp.y, (float) tmp.z ) );
  CObject3D::InitMat44();
}

bool CPhysicController::MovePlayer ( const Vect3f &_vDirection, float _ElapsedTime ) {
  assert ( m_pPhXController != NULL );
  bool l_isGrounded = false;
  NxVec3 l_Direction ( _vDirection.x, _vDirection.y, _vDirection.z);
  NxF32 sharpness = 1.0f;
  NxU32 collisionFlags = 0;
  int mask = 1 << ECG_PLAYER;
  mask |= 1 << ECG_DYNAMIC_OBJECTS;
  mask |= 1 << ECG_STATIC_OBJECTS;
  mask |= 1 << ECG_ESCENE;
  mask |= 1 << ECG_ENEMY;
  mask |= 1 << ECG_LIMITS;
  m_pPhXController->move( l_Direction , mask, 0.0001f, collisionFlags, sharpness );
  if ( ( collisionFlags & NXCC_COLLISION_DOWN )) // || ( collisionFlags & NXCC_COLLISION_UP ) ) {
    l_isGrounded = true;
  NxExtendedVec3 tmp;
  tmp = m_pPhXController->getDebugPosition();
  SetPosition ( Vect3f ( (float) tmp.x, (float) tmp.y, (float) tmp.z ) );
  CObject3D::InitMat44();
  return l_isGrounded;
}

void CPhysicController::SetCollision ( bool _bFlag ) {
  assert ( m_pPhXController );
  m_pPhXController->setCollision ( _bFlag );
}

bool CPhysicController::UpdateCharacterExtents (bool bent, float ammount) {
  NxF32 height = m_fHeightControler;
  NxF32 radius = m_fRadiusControler;
  NxExtendedVec3 pos = m_pPhXController->getPosition();
  if ( bent ) {
    //Ponerse de pie
    height += ammount;
    pos.y += ammount * 0.5f;
  } else {
    //Agacharse
    height -= ammount;
    pos.y -= ammount * 0.5f;
  }
  NxCapsule worldCapsule;
  worldCapsule.p0.x = worldCapsule.p1.x = (NxReal)pos.x;
  worldCapsule.p0.y = worldCapsule.p1.y = (NxReal)pos.y;
  worldCapsule.p0.z = worldCapsule.p1.z = (NxReal)pos.z;
  worldCapsule.p0.y -= height * 0.5f;
  worldCapsule.p1.y += height * 0.5f;
  worldCapsule.radius = radius;
  m_pPhXController->setCollision(false);	// Avoid checking overlap with ourself
  bool Status = m_pPhXScene->checkOverlapCapsule(worldCapsule);
  m_pPhXController->setCollision(true);
  /*if (Status) {
    return false;
  }*/
  NxExtendedVec3 position(pos.x, pos.y, pos.z);
  m_pPhXController->setPosition(position);
  SetPosition ( Vect3f ( (float)pos.x, (float)pos.y, (float)pos.z ) );
  NxCapsuleController *c = static_cast<NxCapsuleController *> (m_pPhXController);
  c->setHeight(height);
  m_fHeightControler = height;
  //CObject3D::InitMat44();
  return true;
}

void CPhysicController::SetGroup(int _iGroup) {
  //m_pPhXController->getActor()->setGroup(_iGroup);
  NxShape *const *shapes = m_pPhXController->getActor()->getShapes();
  int l_iNumShapes = m_pPhXController->getActor()->getNbShapes();
  for (int i = 0; i < l_iNumShapes; ++i) {
    shapes[i]->setGroup(_iGroup);
  }
}

void CPhysicController::SetHeight(float _fHeight) {
  NxCapsuleController *l_CC = dynamic_cast<NxCapsuleController *>(m_pPhXController);
  if (l_CC) {
    l_CC->setHeight(_fHeight);
  }
}

void CPhysicController::SetRadius(float _fRadius) {
  NxCapsuleController *l_CC = dynamic_cast<NxCapsuleController *>(m_pPhXController);
  if (l_CC) {
    l_CC->setRadius(_fRadius);
  }
}

void CPhysicController::SetActive ( bool _bActive ) {
  m_pPhXController->setCollision ( _bActive );
}

void CPhysicController::SetStepOffset(float _step) {
  // Alçada màxima que el controler pot pujar. Massa petit + costa de pujar obstacles.
  m_fStepOffsetControler = _step;
  m_pPhXController->setStepOffset(_step);
}

NxControllerDesc *CPhysicController::GetPhXControllerDesc ( void ) {
  NxControllerDesc *l_Controler = NULL;
  switch ( m_Type ) {
    case NX_CONTROLLER_BOX:
      l_Controler = m_pPhXBoxControllerDesc;
    case NX_CONTROLLER_CAPSULE:
      l_Controler = m_pPhXCapsuleControllerDesc;
  };
  return l_Controler;
}