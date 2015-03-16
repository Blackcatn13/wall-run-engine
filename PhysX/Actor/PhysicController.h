//----------------------------------------------------------------------------------
// CPhysicController class
// Author: Enric Vergara
//
// Description:
// Esta clase representa un controlador de caracter.
//----------------------------------------------------------------------------------
#pragma once

#ifndef __INC_PHYSIC_CONTROLLER_H__
#define __INC_PHYSIC_CONTROLLER_H__

#include "Jump\Jump.h"
#include "Object\Object3D.h"
#include "PhysicsDefs.h"

//---Forward Declarations---
class NxController;
class NxCapsuleControllerDesc;
class NxBoxControllerDesc;
class NxControllerDesc;
class NxScene;
class CPhysicUserData;
class CPhysicsControllerHitReport;

//--------------------------

class CPhysicController : public CObject3D
{
public:
    //--- Init and End protocols------------------------------------------
    CPhysicController ( float _fRadius, float _fHeight, float _fSlope, float _fSkinwidth, float _fStepOffset
                        , ECollisionGroup _uiCollisionGroups, CPhysicUserData* _pUserData
                        , const Vect3f& _vPos = Vect3f ( 0.f, 0.f, 0.f ), float _fGravity = -9.8f );

    CPhysicController ( Vect3f _Dim, float _fSlope, float _fSkinwidth, float _fStepOffset, ECollisionGroup _uiCollisionGroups, CPhysicUserData* _pUserData,
                        const Vect3f& _vPos = Vect3f ( 0.f, 0.f, 0.f ), float _fGravity = -9.8f );

    ~CPhysicController ( void );

    //---- Main Functions ------------------------------------------------
    void						Move					( const Vect3f& _vDirection, float _fElapsedTime );
    bool						MovePlayer				( const Vect3f& _vDirection, float _fElapsedTime );
    void						Jump					( float _fAmmount );
    void						CreateController		( NxController* _pController, NxScene* _PScene );

    //---- Functions -----------------------------------------------------
    bool						UpdateCharacterExtents	( bool _bBent, float _fAmmount);

    //---- Properties ( get & Set )---------------------------------------
    CPhysicUserData*			GetUserData				( void )
    {
        return m_pUserData;
    }

    void						SetCollision			( bool _bFlag );

    Vect3f						GetPosition				( void );
    void						SetPosition				( const Vect3f& _vPos );

    float						GetGravity				( void ) const
    {
        return m_fGravity;
    }
    void						SetGravity				( float _fGravity )
    {
        m_fGravity = _fGravity;
    }

    void						SetGroup                ( int _iGroup );

    void						SetHeight               ( float _fHeight );
    float						GetHeight				( void ) const
    {
        return m_fHeightControler;
    }

    void						SetActive               ( bool _bActive );
	void						SetStepOffset			(float _skin);

    float						GetRadius				( void ) const
    {
        return m_fRadiusControler;
    }
    void						SetRadius               ( float _fRadius );

    ECollisionGroup				GetColisionGroup		( void ) const
    {
        return m_uCollisionGroups;
    }

    //---PhsX Info---
    NxController*				GetPhXController		( void )
    {
        return m_pPhXController;
    }
    NxControllerDesc*			GetPhXControllerDesc	( void );

    void						UseGravity				( bool _bUseGravity )
    {
        m_bUseGravity = _bUseGravity;
    }

    EControleType				GetType					( void ) const
    {
        return m_Type;
    }
    void						SetType					( EControleType _Type )
    {
        m_Type = _Type;
    }

    //---- Members ---------------------------------------

private:
    CPhysicUserData*				m_pUserData;
    //CPhysicsControllerHitReport		m_Report;

    void*							m_pReport;

    ECollisionGroup					m_uCollisionGroups;
    CJump							m_Jump;
    float							m_fGravity;
    float							m_fRadiusControler;
    float							m_fHeightControler;
    float							m_fSlopeLimitControler;
    float							m_fSkinWidthControler;
    float							m_fStepOffsetControler;

    Vect3f							m_vExtensio;

    EControleType					m_Type;

    bool							m_bUseGravity;

    //--- PhysX--
    NxCapsuleControllerDesc*		m_pPhXCapsuleControllerDesc;
    NxBoxControllerDesc*			m_pPhXBoxControllerDesc;
    NxController*					m_pPhXController;
    NxScene*						m_pPhXScene;
};

#endif //__INC_PHYSIC_CONTROLLER_H__