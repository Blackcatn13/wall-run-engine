//----------------------------------------------------------------------------------
// CPhysicsManager class
// Author: Enric Vergara
// Changed: Jordi Arenas
// Description:
// This secures availability of the necessary physic functions.
// It internally uses the PhysX library.
//----------------------------------------------------------------------------------

#pragma once

#ifndef __PHYSX_MANAGER_CLASS_H__
#define __PHYSX_MANAGER_CLASS_H__

#include <vector>
#include <string>
#include <map>
#include "Math\Color.h"
#include "Math\MathUtils.h"
#include "Math\Vector3.h"
#include "Utils\Named.h"
#include "PhysicsDefs.h"

//#include "Script/ScriptRegister.h"

//---Forward Declarations---//
class NxPhysicsSDK;
class NxScene;
class NxActor;
class NxCCDSkeleton;
class NxControllerManager;

class CPhysicActor;
class CPhysicCollisionReport;
class CPhysicCookingMesh;
class CPhysicFixedJoint;
class CPhysicController;
class CPhysicSphericalJoint;
class CPhysicRevoluteJoint;
class CPhysicTriggerReport;
class CPhysicUserAllocator;
class CPhysicUserData;
//class CScriptManager;
class CGameEntity;
class CGraphicsManager;
class CPhysicMaterial;
//--------------------------

class CPhysicsManager
{
public:
	//--- Init and End protocols------------------------------------------
							CPhysicsManager					();
	virtual					~CPhysicsManager				()		{ Done(); }

	//---- Main Functions ---------------------------------------
	bool					Init							();
	void					Done							();
	bool					IsOk							() const						{ return m_bIsOk; }
	bool					Load							( const std::string &_PhysXConfig );
	bool					Reload							();
	bool					ReloadXML						(const std::string &FileName);
	bool					CreateMeshFromXML				(const std::string &FileName);
	
	//--- Rendering Stuff:
	void					DebugRender						( CGraphicsManager *_RM );
	void					DrawActor						( NxActor* actor, CGraphicsManager* _RM );

	//--- Add/Release Actors
	bool					AddPhysicActor					( CPhysicActor* _pActor);
	bool					ReleasePhysicActor				( CPhysicActor* _pActor);
	
	bool					ReleaseAllActors				(); //EUserDataFlag _eFlags );

	//--- Add/Release CharacterControllers
	bool					AddPhysicController				( CPhysicController* _pController, EControleType _Tipus = ::CAPSULE, ECollisionGroup _Group = ::ECG_ENEMY );
	bool					ReleasePhysicController			( CPhysicController* _pController );

	//---- Add/Release Materials
	bool					AddMaterial (CPhysicMaterial* _pMaterial);
	bool					ReleaseMaterial (CPhysicMaterial* _pMaterial);

	////--- Add/Release Joints
	bool					AddPhysicSphericalJoint			( CPhysicSphericalJoint* _pJoint );
	bool					RelasePhysicSphericalJoint		( CPhysicSphericalJoint* _pJoint );
	bool					AddPhysicRevoluteJoint			( CPhysicRevoluteJoint* _pJoint );
	bool					RelasePhysicRevoluteJoint		( CPhysicRevoluteJoint* _pJoint );
	bool					AddPhysicFixedJoint				( CPhysicFixedJoint* _pJoint );
	bool					RelasePhysicFixedJoint			( CPhysicFixedJoint* _pJoint );

	////--- Intersection Functions:	
	//CPhysicUserData*		RaycastClosestActor				( const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, SCollisionInfo& info );
	CPhysicUserData*		RaycastClosestActor				( const Vect3f _vPosRay, const Vect3f& _vDirRay, uint32 _uiImpactMask, SCollisionInfo& _Info, float _uiMaxDistance = FLT_MAX );
	CPhysicUserData*		RaycastClosestActorShoot		( const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, SCollisionInfo& info, float _fPower );
	void					OverlapSphereActor				( float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> &impactObjects, uint32 impactMask );
	void					OverlapSphereActorGrenade		( float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> impactObjects, float _fPower );
	void					ApplyExplosion					( NxActor* _pActor, const Vect3f& _vPosSphere, float _fEffectRadius, float _fPower );

	//----Update
	void					Update							( float _ElapsedTime );
	void					WaitForSimulation				();

	////--- Create CCDSkeleton
	NxCCDSkeleton*			CreateCCDSkeleton				( float size );

	////---- Properties ( get & Set )---------------------------------------
	void					SetDebugRenderMode				( bool _Flag )								{ m_bDebugRenderMode = _Flag; }
	bool					GetDebugRenderMode				( void ) const								{ return m_bDebugRenderMode; }
	
	NxScene*				GetScene						() const									{ return m_pScene; }	
	
	void					SetInitParams					( const SPhysicsInitParams& initParams )	{ m_InitParams = initParams; }
	
	//--- Get CookingMesh
	CPhysicCookingMesh*		GetCookingMesh					() const									{ return m_pCookingMesh; }
	NxPhysicsSDK*			GetPhysicsSDK					()											{ return m_pPhysicsSDK; }
	CPhysicActor*			GetActor						( std::string _ActorName );

	void					SetTriggerReport				( CPhysicTriggerReport* _pReport );
	void					SetCollisionReport				( CPhysicCollisionReport* _pReport );
  
	int						GetCollisionMask				( ECollisionGroup _szGroup )				{ return m_CollisionMasks[_szGroup]; }
	const std::string&		GetConfigFileName				() const									{ return m_szConfigFileName; }
	int						GetCollisionGroup				( const std::string& _szGroup );
	ECollisionGroup			GetCollisionGroup				( const int _szGroup );

	inline void				SetDrawfrustum					( bool _draw )							{ m_bfrustum = _draw; }
	inline bool				GetDrawfrustum					( void )								{ return m_bfrustum; }
	
	inline void				SetDrawFront					( bool _draw )							{ m_bFront = _draw; }
	inline bool				GetDrawFront					( void )								{ return m_bFront; }
	
	inline void				SetDrawNames					( bool _draw )							{ m_bNames = _draw; }
	inline bool				GetDrawNames					( void )								{ return m_bNames; }
	
	inline void				SetDrawRays						( bool _draw )							{ m_bRays = _draw; }
	inline bool				GetDrawRays						( void )								{ return m_bRays; }
	
	inline void				SetRenderPositions				( bool _draw )							{ m_bRenderDebugPositions = _draw; }
	inline bool				GetRenderPositions				( void )								{ return m_bRenderDebugPositions; }

	inline void				SetDrawDistancesSPheres			( bool _draw )							{ m_bDistancesSheres = _draw; }
	inline bool				GetDrawDistancesSPheres			( void )								{ return m_bDistancesSheres; }

	////---- Helpers---------------------------------------
	bool					CompareUserDatas				( CPhysicUserData *_pUserData1, CPhysicUserData *_pUserData2);

	CPhysicUserData*		GetUserData						( std::string name);
	void                    ReleaseElement					(const std::string &_ase);
	void					SetGravity(Vect3f);
private:
	bool					LoadXML							();
	void					Release							();
	void					ReleaseVectors					();
	void					ReleaseToReload					();

private:
	bool					m_bIsOk;
	bool					m_bDebugRenderMode;
	std::string				m_szConfigFileName;
	std::string				m_FileName;
	
	std::map<int, int>		m_CollisionMasks;

	//---PhysX------------------------------
	NxPhysicsSDK*			m_pPhysicsSDK;
	NxScene*				m_pScene;
	NxControllerManager*	m_pControllerManager;
	CPhysicUserAllocator*	m_pMyAllocator;
	CPhysicCookingMesh*		m_pCookingMesh;
	SPhysicsInitParams		m_InitParams;
	//-------------------------------------------

	bool				m_bfrustum;					// dice si dibuja los frustums de los caracteres
	bool				m_bFront;					// dice si dibuja los fronts de los caracteres
	bool				m_bNames;					// dice si dibuja los Nombres de los caracteres
	bool				m_bRays;					// dice si dibuja los Rayos de los caracteres
	bool				m_bRenderDebugPositions;	// dice si dibuja las posiciones de caracteres activos
	bool				m_bDistancesSheres;			// dice si dibuja las esferas de las distancias de detección, etc

	// Físicas escenario
	std::vector<CPhysicUserData *>			m_vUsersData;	// para guardar los UsersData del CreateMeshFromXML
	std::vector<CPhysicActor *>				m_vActors;		// para guardar los Actors del CreateMeshFromXML	
	std::map<std::string,unsigned int>		m_vIds;			// para guardar los id's del vector de actors
};

#endif __PHYSX_MANAGER_CLASS_H__


