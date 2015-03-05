#include "TestCommands.h"

#include "Core/Core.h"
#include "GraphicsManager.h"
#include "Math/Color.h"
#include "Math/Vector3.h"
#include "InputManager.h"
#include "Camera/Camera.h"
#include "Camera/FPSCamera.h"
#include "Object/Object3D.h"
#include "InputManager.h"
#include "Camera/ThPSCamera.h"
#include "Font/FontManager.h"
#include "Language/LanguageManager.h"
#include "ActionToInput.h"
#include "Camera/CameraController.h"
//#include "Core_Utils/MemLeaks.h"
#include "Utils\Defines.h"

#include "RenderableVertex/VertexTypes.h"
#include "RenderableVertex/RenderableVertexs.h"
#include "Texture/Texture.h"
#include "StaticMeshes/StaticMesh.h"
#include "RenderableVertex/IndexedVertexs.h"
#include "Mesh\MeshInstance.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObject.h"
#include "Core\ScriptManager.h"

#include "Cinematics\CinematicController.h"
#include "Cinematics\Cinematic.h"

#include "Effects\EffectManager.h"

#include "Core\ScriptedController.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "RenderableCommands\SceneRendererCommandManager.h"
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"

#include "Cooking Mesh\PhysicCookingMesh.h"

#include "Granade.h"
#include "AI\AIController.h"

#include <sstream>
#include <string>
#include "Utils\LuaGlobals.h"

#include "SceneElements\StaticPlatform.h"

//#include "Utils\PhysicUserAllocator.h"
CAIController* AI;

CTestCommands::CTestCommands(void)
{
    tTerra1_yaw = 0;
    tTerra2_yaw = 0;
    tlluna1_yaw = 0;
    skip = 0;
    m_dt = 0;
    m_textPosition(50, 2);
    m_printInfo = false;
    m_FPSMode = false;
}

CTestCommands::~CTestCommands(void)
{
    delete m_ObjectFPS;
    delete m_ObjectThPS;
    //delete m_ThPSCamera;
//    delete m_ThPSCamera1;
    //delete m_FPSCamera;
    //PHYSXM->ReleaseAllActors();
    /*CHECKED_DELETE(m_PhysicActor);
    CHECKED_DELETE(m_PhysicUserData);
    CHECKED_DELETE(m_PhysicUserDataCube);
    CHECKED_DELETE(m_PhysicActorCubeFix);*/
    // CHECKED_DELETE(m_Granade);
}

void CTestCommands::Init()
{
    /* Script Manager Tests*/
//	SCRIPTM->RunFile(".\\Data\\test2.lua");
//
//	SCRIPTM->RunFile(SCRIPTM->GetScriptsMap().find("test2")->second);
    /*float l_ElapsedTime=0.3f;
    char l_Text[256];
    _snprintf_s(l_Text, 256, 256, "on_update_scripted_controller(%f)", l_ElapsedTime);
    SCRIPTM->RunCode(l_Text);*/
    m_ObjectFPS = new CObject3D(Vect3f(1, 1, 1), 0, 0, 0);
    m_ObjectThPS = new CObject3D(Vect3f(1, 1, 1), 0, 0, 0);
    //m_RenderableObject = RENDM->GetResourcesMap().find("Box005")->second.m_Value;
    m_ThPSCamera = new CThPSCamera(0.1f, 1000.f, 45.0f * D3DX_PI / 180.0f, 1.f, m_ObjectThPS, 50);
    //m_ThPSCamera1 = new CThPSCamera(0.1f, 100.f, 45.0f * D3DX_PI / 180.0f, 1.f, m_RenderableObject, 10);
    m_FPSCamera = new CFPSCamera(0.1f, 1000.f, 45.0f * D3DX_PI / 180.0f, 1.f, m_ObjectFPS);
    m_ThPSCamera->SetTypeCamera(CCamera::TC_ESF);
    //m_CameraController = new CCameraController();
    CAMCONTM->AddNewCamera("FPS", m_FPSCamera);
    CAMCONTM->AddNewCamera("ThPSESF", m_ThPSCamera);
    //m_CameraController->AddNewCamera("ThPS", m_ThPSCamera1);
    CAMCONTM->setActiveCamera("ThPSESF");
    m_Camera = CAMCONTM->getActiveCamera();
    m_PlayerMode = true;
//	m_ScriptedController = new CScriptedController();
//	RENDM->AddResource("ScriptedController", m_ScriptedController);
    //Lights
    //LIGHTM->Load(".\\Data\\lights.xml");
    //SCRIPTM->Load(".\\Data\\lua_actions.xml");
    //EFFECTM->Load(".\\Data\\effects.xml");
    // m_PhysicUserDataCube = new CPhysicUserData("fixbox");
    // m_PhysicUserDataCube->SetPaint(true);
    // m_PhysicActorCubeFix = new CPhysicActor(m_PhysicUserDataCube);
    // m_PhysicActorCubeFix->AddPlaneShape(Vect3f(0, 1, 0), 0);
    // PHYSXM->AddPhysicActor(m_PhysicActorCubeFix);
    /*CPhysicUserData* m_TriggerData = new CPhysicUserData("trigger");
    m_TriggerData->SetPaint(true);
    m_TriggerData->SetColor(colMAGENTA);
    CPhysicActor* m_Trigger = new CPhysicActor(m_TriggerData);
    m_Trigger->CreateBoxTrigger(Vect3f(5.f, 5.f, 5.f), Vect3f(2.f, 2.f, 2.f));
    PHYSXM->AddPhysicActor(m_Trigger);
    PHYSXM->SetTriggerReport((CPhysicTriggerReport*)TRIGGM);*/
    CPhysicActor* m_PhysicActorCubeFix;
    CPhysicUserData* m_PhysicUserDataCube;
    m_PhysicUserDataCube = new CPhysicUserData("fixbox");
    m_PhysicUserDataCube->SetPaint(true);
    m_PhysicActorCubeFix = new CPhysicActor(m_PhysicUserDataCube);
    m_PhysicActorCubeFix->AddBoxSphape(Vect3f(40, 1, 40));
    PHYSXM->AddPhysicActor(m_PhysicActorCubeFix);
    m_PhysicActorCubeFix = new CPhysicActor(m_PhysicUserDataCube);
    m_PhysicActorCubeFix->AddBoxSphape(Vect3f(5, .2, 5), Vect3f(0, 1, 3));
    PHYSXM->AddPhysicActor(m_PhysicActorCubeFix);
    m_PhysicActorCubeFix = new CPhysicActor(m_PhysicUserDataCube);
    m_PhysicActorCubeFix->AddBoxSphape(Vect3f(5, .2, 5), Vect3f(0, 1.2, 4));
    PHYSXM->AddPhysicActor(m_PhysicActorCubeFix);
    m_PhysicActorCubeFix = new CPhysicActor(m_PhysicUserDataCube);
    m_PhysicActorCubeFix->AddBoxSphape(Vect3f(5, .2, 5), Vect3f(0, 1.2, 4), v3fZERO, Vect3f(0, 0, 0.35));
    PHYSXM->AddPhysicActor(m_PhysicActorCubeFix);
    //m_Granade = new CGranade();
    //CPhysicUserAllocator* m_Alloc = new CPhysicUserAllocator();
    //CPhysicCookingMesh* m_CockMesh = new CPhysicCookingMesh();
    //m_CockMesh->Init(PHYSXM->GetPhysicsSDK(), m_Alloc);
    //m_CockMesh->CreateMeshFromASE("./Data/sceneTrainingPiky.ASE", "sceneTraining");
    m_LuaInitLevelFunc = CCORE->getLuaLoadLevelFunc();
    char l_InitLevelText[256];
    _snprintf_s(l_InitLevelText, 256, 256, m_LuaInitLevelFunc.c_str());
    std::stringstream ss;
    int var = 1;
    ss << CCORE->getLuaLoadLevelFunc() << "(" << var << ")";
    std::string toRun = ss.str();
    SCRIPTM->RunCode(toRun.c_str());
    std::string l_Hi =	CLuaGlobals::getInstance()->getString();
    AI = new CAIController();
    //CStaticPlatform * l_Platform = new CStaticPlatform("Platform1", "UserDataPlatform1", Vect3f(2, 2, 2), Vect3f(5, 7, 2), Vect3f(0, 0, 0));
}

void CTestCommands::DeInit()
{
}

Vect2i CTestCommands::RenderDebugInfo(bool render, float dt)
{
    Vect2i size;
    if (m_printInfo) {
        // get Process debug size
        size = CProcess::RenderDebugInfo(false, m_dt);
        // get this process debug size
        size.Add_Max(FONTM->GetDefaultTextSize(size.x, size.y, colWHITE, "Info from %s", __FILE__));
        // print quad
        GRAPHM->DrawRectangle2D (m_textPosition, size.x, size.y , CColor(0, 1, 0, 0.2), 2, 2, CColor(0, 0, 0, 1));
        // print Process debug info
        size = CProcess::RenderDebugInfo(true, m_dt);
        // print this process debug info
        size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Info from %s", __FILE__));
        if (GRAPHM->isSphereVisible(Vect3f(15, 0, 0), 2))
            size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Drawing 3"));
        else
            size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Drawing 2"));
        size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Numero de vertices: %d", m_totalVertices));
        size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Numero de caras: %d", m_totalFaces));
        size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Numero de primitivas: %d", m_numPrimitives));
    } else {
        // print a message asking for key to open the menu
        Vect2i aux = m_textPosition;
        aux.Add_Max(FONTM->GetLiteralSize(aux.x, aux.y, "OpenDebug"));
        GRAPHM->DrawRectangle2D (m_textPosition, aux.x, aux.y , CColor(0, 1, 0, 0.2), 2, 2, CColor(0, 0, 0, 1));
        FONTM->DrawLiteral(m_textPosition.x, m_textPosition.y, "OpenDebug");
        if (GRAPHM->isSphereVisible(Vect3f(15, 0, 0), 2))
            size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Drawing 3"));
        else
            size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Drawing 2"));
        size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Numero de vertices: %d", m_totalVertices));
        size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Numero de caras: %d", m_totalFaces));
        size.Add_Max(FONTM->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Numero de primitivas: %d", m_numPrimitives));
    }
    return size;
}

void CTestCommands::Update(float dt)
{
    // m_Granade->Update(dt);
    //CInputManager* im = INPUTM;
    //float deltaX =  im->GetMouseDelta().x;
    //float deltaY = im->GetMouseDelta().y;
    //float deltaZ = im->GetMouseDelta().z;
    if (ACT2IN->DoAction("ChangeCatalan")) {
        LANGM->SetCurrentLanguage("catalan");
        // CAMCONTM->setActiveCamera("Camera002");
        //CAMCONTM->Play(true);
        //CCORE->GetCinematicController()->GetResource("DestroyW")->Play(true);
    }
    if (ACT2IN->DoAction("ChangeCastellano")) {
        LANGM->SetCurrentLanguage("castellano");
        //CCORE->GetCinematicController()->GetResource("DestroyW")->Stop();
        // CAMCONTM->Stop();
        //CAMCONTM->setActiveCamera("FPS");
    }
    if (ACT2IN->DoAction("ChangeIngles")) {
        LANGM->SetCurrentLanguage("ingles");
        // CAMCONTM->Pause();
        // CCORE->GetCinematicController()->GetResource("DestroyW")->Pause();
    }
    if (ACT2IN->DoAction("CommutationCamera")) {
        if (m_PlayerMode) {
            m_PlayerMode = false;
            //  m_CameraController->setActiveCamera("ThPSESF");
            CAMCONTM->setActiveCamera("ThPSESF");
        } else {
            m_PlayerMode = true;
            CAMCONTM->setActiveCamera("FPS");
        }
    }
    // CCORE->GetCinematicController()->Update(dt);
    m_Camera = CAMCONTM->getActiveCamera();
    /*  m_Granade->Update(dt);
    //	m_ScriptedController->Update(dt);*/
    /* if (m_PlayerMode) {
         m_ObjectFPS->SetYaw(m_ObjectFPS->GetYaw() -  deltaX * dt);
         m_ObjectFPS->SetPitch(m_ObjectFPS->GetPitch() - deltaY * dt);
         Vect3f dir = m_Camera->GetDirection();
         float yaw = m_ObjectFPS->GetYaw();
         Vect3f nor = Vect3f(mathUtils::Cos(yaw + D3DX_PI * 0.5), 0, (mathUtils::Sin(yaw + D3DX_PI * 0.5)));
         nor.Normalize();
         if (im->IsDown(IDV_KEYBOARD, KEY_W)) {
             m_ObjectFPS->SetPosition(m_ObjectFPS->GetPosition() + dir * 2 * dt);
         }
         if (im->IsDown(IDV_KEYBOARD, KEY_S)) {
             m_ObjectFPS->SetPosition(m_ObjectFPS->GetPosition() - dir * 2 * dt);
         }
         if (im->IsDown(IDV_KEYBOARD, KEY_A)) {
             m_ObjectFPS->SetPosition(m_ObjectFPS->GetPosition() + nor * 2 * dt);
         }
         if (im->IsDown(IDV_KEYBOARD, KEY_D)) {
             m_ObjectFPS->SetPosition(m_ObjectFPS->GetPosition() - nor * 2 * dt);
         }
     } else {
         m_ObjectThPS->SetYaw(m_ObjectThPS->GetYaw() - deltaX * dt);
         m_ObjectThPS->SetPitch(m_ObjectThPS->GetPitch() - deltaY * dt);
         m_ThPSCamera->AddZoom(-deltaZ * dt );
     }*/
    if (ACT2IN->DoAction("ToggleFPSCam"))
        m_FPSMode = !m_FPSMode;
    if (m_FPSMode)
        CAMCONTM->Update("FPS", dt);
    else
        CAMCONTM->Update(dt);
    if (ACT2IN->DoAction("ReloadScriptedController")) {
        SCRIPTM->Reload(".\\Data\\scripted_controller.lua");
        //	SCRIPTM->RunFile(SCRIPTM->GetScriptsMap().find("test2")->second);
        //SCRIPTM->RunFile(".\\Data\\scripted_controller.lua");
    }
    skip += dt;
    //tTerra1_yaw += dt * 30 * 0.005;
    //tTerra2_yaw += dt * 80 * 0.005;
    //tlluna1_yaw -= dt * 60 * 0.05;
    m_dt = dt;
    RENDLM->Update(dt);
    AI->MoveTo(dt, Vect3f(15, 2, 15));
}

void CTestCommands::Render()
{
    //TO DO: Actualizar variables m_numPrimitives, m_totalVertexs y m_totalFaces
    m_numPrimitives = 0;
    m_totalVertices = 0;
    m_totalFaces = 0;
    /* Mat44f t;
     t.SetIdentity();
     GRAPHM->SetTransform(t);
     GRAPHM->DrawGrid(20);
     GRAPHM->DrawAxis(10);*/
    //text->Activate(0);
    //RENDLM->Render(GRAPHM, RENDLM->getCurrentLayer);
    SCENRENDCOMM->Execute(*GRAPHM);
    //g_RV->Render(CCORE->GetGraphicsManager(º+-));
    /* Mat44f t;
     Mat44f trot1;
     Mat44f trot2;
     Mat44f ttrans;
     Mat44f ltrans;
     Mat44f lrot;
     trot1.SetIdentity();

    GRAPHM->SetTransform(trot1);
    GRAPHM->DrawAxis(5);

    GRAPHM->DrawQuad3D(Vect3f(0,20,0), Vect3f(0, 1, 0), Vect3f(-1, 0, 0), 3, 4, CColor(1, 0, 0, 1));

     trot2.SetIdentity();
     ttrans.SetIdentity();
     ltrans.SetIdentity();
     lrot.SetIdentity();
     trot1.RotByAngleY(tTerra1_yaw);
     trot2.RotByAngleY(tTerra2_yaw);
     lrot.RotByAngleY(tlluna1_yaw);
     ttrans.Translate(Vect3f(15, 0 , 0));
     ltrans.Translate(Vect3f(3, 0 , 0));
     t.SetIdentity();
     GRAPHM->SetTransform(t);
     GRAPHM->DrawCamera(m_FPSCamera);
     if (skip < 1) {
         GRAPHM->DrawSphere(4, colYELLOW, 10);
     } else if (skip > 3) {
         skip = 0;
     }
     t.SetIdentity();
     GRAPHM->SetTransform(t);
    GRAPHM->SetTransform(ttrans);
    GRAPHM->DrawCylinder (1, 1, 4, 10, colRED);
    GRAPHM->SetTransform(t);
    GRAPHM->SetTransform(ltrans);
    GRAPHM->DrawCapsule (1, 4);
    GRAPHM->SetTransform(t);
     t = trot2 * ttrans * trot1;
     GRAPHM->SetTransform(t);
     GRAPHM->DrawSphere(2, colBLUE, 10);
     t = t * lrot * ltrans;
     GRAPHM->SetTransform(t);
     GRAPHM->DrawSphere(0.5, colWHITE, 10);
     //GRAPHM->DrawQuad2D (Vect2i(8,8), 500, 15 + 80 , UPPER_LEFT, colGREEN);
     //int ypos = FONTM->DrawDefaultText(10, 10, colWHITE, "Frame Rate %lf FPS", 1/m_dt);
     //int fid = FONTM->GetTTF_Id("Deco");
     //ypos += FONTM->DrawTextA(10, 10 + ypos, colRED, fid, "Frame Rate %lf FPS", 1/m_dt);
     //fid = FONTM->GetTTF_Id("Titania");
     //ypos += FONTM->DrawTextA(10, 10 + ypos, colBLUE, fid, "Frame Rate %lf FPS", 1/m_dt);
     //ypos += FONTM->DrawLiteral(10, 10 + ypos, "HiWorld");
     //ypos += FONTM->DrawLiteral(10, 10 + ypos, "Exit");
     //ypos += FONTM->DrawLiteral(10, 10 + ypos, "PARTY");
    GRAPHM->DrawQuad3D(Vect3f(0,15,0), Vect3f(0, 1, 0), Vect3f(-1, 0, 0), 3, 4, CColor(1, 0, 0, 0.5));*/
}
