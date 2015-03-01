#ifndef CTEST_COMMANDS_H
#define CTEST_COMMANDS_H

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

class CTestCommands : public CProcess
{
private:
    CCameraController*  m_CameraController;
    CObject3D*		    m_ObjectFPS;
    CObject3D*		    m_ObjectThPS;
    CFPSCamera*		    m_FPSCamera;
    CThPSCamera*	    m_ThPSCamera;
    CThPSCamera*	    m_ThPSCamera1;
    CScriptedController * m_ScriptedController;
    CRenderableObject	*m_RenderableObject;
    CPhysicActor		*m_PhysicActor;
    CPhysicActor		*m_PhysicActorCubeFix;
    CPhysicUserData		*m_PhysicUserData;
    CPhysicUserData		*m_PhysicUserDataCube;
    CGranade			*m_Granade;
    bool			    m_PlayerMode;
    bool				m_FPSMode;
    float			    tTerra1_yaw;
    float			    tTerra2_yaw;
    float			    tlluna1_yaw;
    float			    skip;
    float			    m_dt;
    Vect2i              m_textPosition;
    int					m_numPrimitives, m_totalVertices, m_totalFaces;
    CPlayerController*  m_PlayerController;
    std::string			m_LuaInitLevelFunc;
public:
    CTestCommands(void);
    ~CTestCommands(void);
    void Update(float dt);
    void Render();
    void Init();
    void DeInit();
    Vect2i RenderDebugInfo(bool render, float dt);
};

#endif
