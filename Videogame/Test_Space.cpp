#include "Test_Space.h"
#include "Core.h"
#include "GraphicsManager.h"
#include "Math\Color.h"
#include "Math\Vector3.h"
#include "InputManager.h"
#include "Camera.h"
#include "FPSCamera.h"
#include "Object3D.h"
#include "InputManager.h"
#include "ThPSCamera.h"
#include "FontManager.h"
#include "LanguageManager.h"
#include "ActionToInput.h"
#include "CameraController.h"
#include "MemLeaks.h"

#include "RenderableVertex\VertexTypes.h"
#include "RenderableVertex\RenderableVertexs.h"
#include "Texture\Texture.h"
#include "StaticMeshes\StaticMesh.h"
#include "RenderableVertex\IndexedVertexs.h"



CRenderableVertexs *g_RV = 0;
CTexture *text = 0;
CStaticMesh *sm = 0;

CTest_Space::CTest_Space(void)
{
    m_ObjectFPS = new CObject3D(Vect3f(1, 1, 1), 0, 0, 0);
    m_ObjectThPS = new CObject3D(Vect3f(0, 0, 0), 0, 0, 0);
    m_ThPSCamera = new CThPSCamera(1.f, 100.f, 45.0f * D3DX_PI / 180.0f, 1.f, m_ObjectThPS, 50);
    m_ThPSCamera1 = new CThPSCamera(1.f, 100.f, 45.0f * D3DX_PI / 180.0f, 1.f, m_ObjectThPS, 50);
    m_FPSCamera = new CFPSCamera(1.f, 100.f, 45.0f * D3DX_PI / 180.0f, 1.f, m_ObjectFPS);
    m_ThPSCamera->SetTypeCamera(CCamera::TC_ESF);
    m_CameraController = new CCameraController();
    m_CameraController->AddNewCamera("FPS", m_FPSCamera);
    m_CameraController->AddNewCamera("ThPSESF", m_ThPSCamera);
    m_CameraController->AddNewCamera("ThPS", m_ThPSCamera1);
    m_CameraController->setActiveCamera("FPS");
    m_Camera = m_CameraController->getActiveCamera();
    m_PlayerMode = true;
    tTerra1_yaw = 0;
    tTerra2_yaw = 0;
    tlluna1_yaw = 0;
    skip = 0;
    m_dt = 0;
    m_textPosition(50, 2);
    m_printInfo = false;
    m_FPSMode = false;
    text = new CTexture();
    sm = new CStaticMesh();
}


CTest_Space::~CTest_Space(void)
{
    delete m_ObjectFPS;
    delete m_ObjectThPS;
    delete m_ThPSCamera;
    delete m_ThPSCamera1;
    delete m_FPSCamera;
    delete m_CameraController;
    delete g_RV;
    delete text;
    delete sm;
}

void CTest_Space::Init()
{
    TTEXTURE_VERTEX VB[4] = {{0, 0, 0, 0, 0}, {0, 5, 0, 0, 1}, {5, 5, 0, 1, 1}, {5, 0, 0, 1, 0}};
    unsigned short IB[6] = {0, 1, 2, 0, 2, 3};
    g_RV = new CIndexedVertexs<TTEXTURE_VERTEX>(CCore::GetInstance()->GetGraphicsManager(), VB, IB, 4, 6);
    text->Load("text.jpg");
    sm->Load(".\\Data\\a.mesh");
}

void CTest_Space::DeInit()
{
}

Vect2i CTest_Space::RenderDebugInfo(bool render, float dt)
{
    Vect2i size;
    if (m_printInfo) {
        // get Process debug size
        size = CProcess::RenderDebugInfo(false, m_dt);
        // get this process debug size
        size.Add_Max(CCore::GetInstance()->GetFontManager()->GetDefaultTextSize(size.x, size.y, colWHITE, "Info from %s", __FILE__));
        // print quad
        CCore::GetInstance()->GetGraphicsManager()->DrawRectangle2D (m_textPosition, size.x, size.y , CColor(0, 1, 0, 0.2), 2, 2, CColor(0, 0, 0, 1));
        // print Process debug info
        size = CProcess::RenderDebugInfo(true, m_dt);
        // print this process debug info
        size.Add_Max(CCore::GetInstance()->GetFontManager()->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Info from %s", __FILE__));
        if (CCore::GetInstance()->GetGraphicsManager()->isSphereVisible(Vect3f(15, 0, 0), 2))
            size.Add_Max(CCore::GetInstance()->GetFontManager()->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Drawing 3"));
        else
            size.Add_Max(CCore::GetInstance()->GetFontManager()->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Drawing 2"));
    } else {
        // print a message asking for key to open the menu
        Vect2i aux = m_textPosition;
        aux.Add_Max(CCore::GetInstance()->GetFontManager()->GetLiteralSize(aux.x, aux.y, "OpenDebug"));
        CCore::GetInstance()->GetGraphicsManager()->DrawRectangle2D (m_textPosition, aux.x, aux.y , CColor(0, 1, 0, 0.2), 2, 2, CColor(0, 0, 0, 1));
        CCore::GetInstance()->GetFontManager()->DrawLiteral(m_textPosition.x, m_textPosition.y, "OpenDebug");
        if (CCore::GetInstance()->GetGraphicsManager()->isSphereVisible(Vect3f(15, 0, 0), 2))
            size.Add_Max(CCore::GetInstance()->GetFontManager()->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Drawing 3"));
        else
            size.Add_Max(CCore::GetInstance()->GetFontManager()->DrawDefaultText(m_textPosition.x, size.y, colWHITE, "Drawing 2"));
    }
    return size;
}

void CTest_Space::Update(float dt)
{
    CInputManager* im = CCore::GetInstance()->GetInputManager();
    //float deltaX =  im->GetMouseDelta().x;
    //float deltaY = im->GetMouseDelta().y;
    //float deltaZ = im->GetMouseDelta().z;
    if (CCore::GetInstance()->GetActionToInput()->DoAction("ChangeCatalan")) {
        CCore::GetInstance()->GetLanguageManager()->SetCurrentLanguage("catalan");
    }
    if (CCore::GetInstance()->GetActionToInput()->DoAction("ChangeCastellano")) {
        CCore::GetInstance()->GetLanguageManager()->SetCurrentLanguage("castellano");
    }
    if (CCore::GetInstance()->GetActionToInput()->DoAction("ChangeIngles")) {
        CCore::GetInstance()->GetLanguageManager()->SetCurrentLanguage("ingles");
    }
    if (CCore::GetInstance()->GetActionToInput()->DoAction("CommutationCamera")) {
        if (m_PlayerMode) {
            m_PlayerMode = false;
            m_CameraController->setActiveCamera("ThPSESF");
            m_Camera = m_CameraController->getActiveCamera();
        } else {
            m_PlayerMode = true;
            m_CameraController->setActiveCamera("FPS");
            m_Camera = m_CameraController->getActiveCamera();
        }
    }
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
    if (CCore::GetInstance()->GetActionToInput()->DoAction("ToggleFPSCam"))
        m_FPSMode = !m_FPSMode;
    if (m_FPSMode)
        m_CameraController->Update("FPS", dt);
    else
        m_CameraController->Update(dt);
    skip += dt;
    tTerra1_yaw += dt * 30 * 0.005;
    tTerra2_yaw += dt * 80 * 0.005;
    tlluna1_yaw -= dt * 60 * 0.05;
    m_dt = dt;
}

void CTest_Space::Render()
{
    CCore::GetInstance()->GetGraphicsManager()->DrawGrid(20);
    CCore::GetInstance()->GetGraphicsManager()->DrawAxis(10);
    text->Activate(0);
    sm->Render(CCore::GetInstance()->GetGraphicsManager());
    //g_RV->Render(CCore::GetInstance()->GetGraphicsManager(º+-));
    /* Mat44f t;
     Mat44f trot1;
     Mat44f trot2;
     Mat44f ttrans;
     Mat44f ltrans;
     Mat44f lrot;
     trot1.SetIdentity();

    CCore::GetInstance()->GetGraphicsManager()->SetTransform(trot1);
    CCore::GetInstance()->GetGraphicsManager()->DrawAxis(5);

    CCore::GetInstance()->GetGraphicsManager()->DrawQuad3D(Vect3f(0,20,0), Vect3f(0, 1, 0), Vect3f(-1, 0, 0), 3, 4, CColor(1, 0, 0, 1));

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
     CCore::GetInstance()->GetGraphicsManager()->SetTransform(t);
     CCore::GetInstance()->GetGraphicsManager()->DrawCamera(m_FPSCamera);
     if (skip < 1) {
         CCore::GetInstance()->GetGraphicsManager()->DrawSphere(4, colYELLOW, 10);
     } else if (skip > 3) {
         skip = 0;
     }
     t.SetIdentity();
     CCore::GetInstance()->GetGraphicsManager()->SetTransform(t);
    CCore::GetInstance()->GetGraphicsManager()->SetTransform(ttrans);
    CCore::GetInstance()->GetGraphicsManager()->DrawCylinder (1, 1, 4, 10, colRED);
    CCore::GetInstance()->GetGraphicsManager()->SetTransform(t);
    CCore::GetInstance()->GetGraphicsManager()->SetTransform(ltrans);
    CCore::GetInstance()->GetGraphicsManager()->DrawCapsule (1, 4);
    CCore::GetInstance()->GetGraphicsManager()->SetTransform(t);
     t = trot2 * ttrans * trot1;
     CCore::GetInstance()->GetGraphicsManager()->SetTransform(t);
     CCore::GetInstance()->GetGraphicsManager()->DrawSphere(2, colBLUE, 10);
     t = t * lrot * ltrans;
     CCore::GetInstance()->GetGraphicsManager()->SetTransform(t);
     CCore::GetInstance()->GetGraphicsManager()->DrawSphere(0.5, colWHITE, 10);
     //CCore::GetInstance()->GetGraphicsManager()->DrawQuad2D (Vect2i(8,8), 500, 15 + 80 , UPPER_LEFT, colGREEN);
     //int ypos = CCore::GetInstance()->GetFontManager()->DrawDefaultText(10, 10, colWHITE, "Frame Rate %lf FPS", 1/m_dt);
     //int fid = CCore::GetInstance()->GetFontManager()->GetTTF_Id("Deco");
     //ypos += CCore::GetInstance()->GetFontManager()->DrawTextA(10, 10 + ypos, colRED, fid, "Frame Rate %lf FPS", 1/m_dt);
     //fid = CCore::GetInstance()->GetFontManager()->GetTTF_Id("Titania");
     //ypos += CCore::GetInstance()->GetFontManager()->DrawTextA(10, 10 + ypos, colBLUE, fid, "Frame Rate %lf FPS", 1/m_dt);
     //ypos += CCore::GetInstance()->GetFontManager()->DrawLiteral(10, 10 + ypos, "HiWorld");
     //ypos += CCore::GetInstance()->GetFontManager()->DrawLiteral(10, 10 + ypos, "Exit");
     //ypos += CCore::GetInstance()->GetFontManager()->DrawLiteral(10, 10 + ypos, "PARTY");
    CCore::GetInstance()->GetGraphicsManager()->DrawQuad3D(Vect3f(0,15,0), Vect3f(0, 1, 0), Vect3f(-1, 0, 0), 3, 4, CColor(1, 0, 0, 0.5));*/
}
