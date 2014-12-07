#include "Test_Process.h"
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



CTest_Process::CTest_Process(void)
{
    m_ObjectFPS = new CObject3D(Vect3f(1, 1, 1), 0, 0, 0);
    m_ObjectThPS = new CObject3D(Vect3f(0, 0, 0), 0, 0, 0);
    m_ThPSCamera = new CThPSCamera(1.f, 100.f, 45.0f * D3DX_PI / 180.0f, 1.f, m_ObjectThPS, 3);
    m_FPSCamera = new CFPSCamera(1.f, 100.f, 45.0f * D3DX_PI / 180.0f, 1.f, m_ObjectFPS);
    m_Camera = m_FPSCamera;
    m_PlayerMode = true;
    t1.SetIdentity();
    skip = 0;
}


CTest_Process::~CTest_Process(void)
{
}

void CTest_Process::Init()
{
}

void CTest_Process::DeInit()
{
}

void CTest_Process::Update(float dt)
{
    CInputManager* im = INPUTM;
    float deltaX =  im->GetMouseDelta().x;
    float deltaY = im->GetMouseDelta().y;
    float deltaZ = im->GetMouseDelta().z;
    if (im->IsDownUp(IDV_KEYBOARD, KEY_TAB)) {
        if (m_PlayerMode) {
            m_PlayerMode = false;
            m_Camera = m_ThPSCamera;
        } else {
            m_PlayerMode = true;
            m_Camera = m_FPSCamera;
        }
    }
    if (m_PlayerMode) {
        m_ObjectFPS->SetYaw(m_ObjectFPS->GetYaw() -  deltaX * 0.005f);
        m_ObjectFPS->SetPitch(m_ObjectFPS->GetPitch() - deltaY * 0.005f);
        Vect3f dir = m_Camera->GetDirection();
        float yaw = m_ObjectFPS->GetYaw();
        Vect3f nor = Vect3f(mathUtils::Cos(yaw + D3DX_PI * 0.5), 0, (mathUtils::Sin(yaw + D3DX_PI * 0.5)));
        nor.Normalize();
        if (im->IsDown(IDV_KEYBOARD, KEY_W)) {
            m_ObjectFPS->SetPosition(m_ObjectFPS->GetPosition() + dir * 0.01);
        }
        if (im->IsDown(IDV_KEYBOARD, KEY_S)) {
            m_ObjectFPS->SetPosition(m_ObjectFPS->GetPosition() - dir * 0.01);
        }
        if (im->IsDown(IDV_KEYBOARD, KEY_A)) {
            m_ObjectFPS->SetPosition(m_ObjectFPS->GetPosition() + nor * 0.01);
        }
        if (im->IsDown(IDV_KEYBOARD, KEY_D)) {
            m_ObjectFPS->SetPosition(m_ObjectFPS->GetPosition() - nor * 0.01);
        }
    } else {
        m_ObjectThPS->SetYaw(m_ObjectThPS->GetYaw() - deltaX * 0.005f);
        m_ObjectThPS->SetPitch(m_ObjectThPS->GetPitch() - deltaY * 0.005f);
        m_ThPSCamera->AddZoom(-deltaZ * 0.005f);
    }
}

void CTest_Process::Render()
{
    Mat44f t;
    t.SetIdentity();
    GRAPHM->SetTransform(t);
    GRAPHM->DrawAxis(5);
    //GRAPHM->DrawCube(2);
    //GRAPHM->DrawBox(1,2,-1,colRED);
    GRAPHM->DrawGrid(5, colBLACK, 10, 10);
    //GRAPHM->DrawPlane(2, Vect3f(0,1,0), 3);
    if (skip < 100)
        skip++;
    else {
        t1.RotByAngleY(20);
        skip = 0;
    }
    GRAPHM->SetTransform(t1);
    GRAPHM->DrawCube(0.5, colMAGENTA);
    GRAPHM->DrawSphere(2, colWHITE, 20);
}