#ifndef CTEST_GAMEPLAY_H
#define CTEST_GAMEPLAY_H

#include "Core/Process.h"
#include "Math/Matrix44.h"
#include "Renderable\RenderableObject.h"

class CCameraController;
class CObject3D;
class CFPSCamera;
class CThPSCamera;
class CScriptManager;
class CScriptedController;
class CPhysicUserData;
class CPhysicActor;
class CPhysicMaterial;
class CGranade;
class CPlayerController;
//class CPhysicMaterial;
class CFSMManager;
class CWPManager;

class CTestGameplay : public CProcess
{
private:
    CCameraController  *m_CameraController;
    CObject3D		    *m_ObjectFPS;
    CObject3D		    *m_ObjectThPS;
    CObject3D		*m_ObjectThPSEsf;
    CObject3D		    *m_Object3D;
    CObject3D		    *m_Object2D;
    CObject3D			*m_ObjectGUI;
    CFPSCamera		    *m_FPSCamera;
    CThPSCamera	    *m_ThPSCamera;
    CThPSCamera	    *m_ThPSCamera1;
    CThPSCamera	    *m_3DCamera;
	CThPSCamera	    *m_BossCamera;
    CThPSCamera	    *m_GUICamera;
    CCamera			*tempCam;
    CThPSCamera	    *m_2DCamera;
    CScriptedController *m_ScriptedController;
    CRenderableObject	*m_RenderableObject;
    //CPhysicActor		*m_PhysicActor;
    CPhysicActor		*m_PhysicActorCubeFix;
    //  CPhysicActor		*m_PhysicActorCubeFix2;
    //  CPhysicUserData		*m_PhysicUserData;
    CPhysicUserData		*m_PhysicUserDataCube;
    //  CPhysicUserData		*m_PhysicUserDataCube2;
    CFSMManager			*m_fsmManager;
    CWPManager			*m_WPManager;
    // CPhysicMaterial		* m_MyMaterial;
    CGranade			*m_Granade;
    bool			    m_PlayerMode;
    bool				m_FPSMode;
    float			    tTerra1_yaw;
    float			    tTerra2_yaw;
    float			    tlluna1_yaw;
    float			    skip;
    //float			    m_dt;

    int					m_numPrimitives, m_totalVertices, m_totalFaces;
    CPlayerController  *m_PlayerController;
    std::string			m_LuaInitLevelFunc;
    CColor		m_Color;
public:
    CTestGameplay(void);
    ~CTestGameplay(void);
    void Update(float dt);
    void Render();
    void Init();
    void DeInit();
    Vect2i RenderDebugInfo(bool render/*, float dt*/);
};

#endif
