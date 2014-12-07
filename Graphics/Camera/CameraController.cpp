#include "CameraController.h"
#include "Object/Object3D.h"
#include "Camera.h"
#include "FPSCamera.h"
#include "ThPSCamera.h"
#include "Core/Core.h"
#include "ActionToInput.h"
#include "Math/Vector3.h"
#include "Math/MathTypes.h"

CCameraController::CCameraController() : m_Speed(2),
    m_PanSpeed(10),
    m_FlyMode(false),
    m_FPSMaxPitch(90),
    m_ThPSMaxPitch(90),
    m_BigZoom(20),
    m_LittleZoom(1)
{
}

void CCameraController::AddNewObject(std::string objName, CObject3D* obj)
{
    m_Objects.insert(PairString2Object(objName, obj));
}

void CCameraController::AddNewCamera(std::string camName, CCamera* cam)
{
    m_Cameras.insert(PairString2Camera(camName, cam));
}

void CCameraController::Update(float dt)
{
//	Camera pfs:
//-limitació pitch
//-moviment en diagonal
//-correr
//-ajupir-se
//-botó dret del mouse (mentre estigui apretat) fer efecte zoom com sniper.
//
//camera esfèrica-debug:
//-dibuixar l'objecte (cub+eix local de coordenades)
//-zoom de rodeta més fort, però posar amb combinació de tecla zoom més sensible.
//
//controlador de càmera thps:
//-dibuixar ojecte (cub+eix_local).
//-zoom amb limitacions.
//-control de pitch.
//-correr, ajupir-se, ..
//
    float deltaX, deltaY, deltaZ;
    CCamera::ETypeCamera camType = m_ActiveCamera->GetTypeCamera();
    CObject3D* camObject = m_ActiveCamera->GetObject3D();
    CActionToInput* ATI = ACT2IN;
    if (ATI->DoAction("FlyMode"))
        m_FlyMode = !m_FlyMode;
    if (camType == CCamera::TC_FPS) {
        if (ATI->DoAction("yaw", deltaX))
            camObject->SetYaw(camObject->GetYaw() - deltaX * dt);
        if (ATI->DoAction("pitch", deltaY))
            camObject->SetPitch(camObject->GetPitch() - deltaY * dt);
        float yaw = camObject->GetYaw();
        Vect3f dir = m_ActiveCamera->GetDirection();
        Vect3f nor = Vect3f(mathUtils::Cos(yaw + ePI2f), 0, (mathUtils::Sin(yaw + ePI2f)));
        nor.Normalize();
        if (!m_FlyMode) {
            dir.y = 0;
            nor. y = 0;
        }
        if (ATI->DoAction("MoveForward"))
            camObject->SetPosition(camObject->GetPosition() + dir * m_Speed * dt);
        if (ATI->DoAction("MoveBack"))
            camObject->SetPosition(camObject->GetPosition() - dir * m_Speed * dt);
        if (ATI->DoAction("MoveRigth"))
            camObject->SetPosition(camObject->GetPosition() - nor * m_Speed * dt);
        if (ATI->DoAction("MoveLeft"))
            camObject->SetPosition(camObject->GetPosition() + nor * m_Speed * dt);
    }
    if (camType == CCamera::TC_THPS) {
        if (ATI->DoAction("yaw", deltaX))
            camObject->SetYaw(camObject->GetYaw() - deltaX * dt);
        if (ATI->DoAction("pitch", deltaY))
            camObject->SetPitch(camObject->GetPitch() - deltaY * dt);
        float yaw = camObject->GetYaw();
        Vect3f dir = m_ActiveCamera->GetDirection();
        Vect3f nor = Vect3f(mathUtils::Cos(yaw + ePI2f), 0, (mathUtils::Sin(yaw + ePI2f)));
        nor.Normalize();
        if (!m_FlyMode) {
            dir.y = 0;
            nor. y = 0;
        }
        if (ATI->DoAction("MoveForward"))
            camObject->SetPosition(camObject->GetPosition() + dir * m_Speed * dt);
        if (ATI->DoAction("MoveBack"))
            camObject->SetPosition(camObject->GetPosition() - dir * m_Speed * dt);
        if (ATI->DoAction("MoveRigth"))
            camObject->SetPosition(camObject->GetPosition() - nor * m_Speed * dt);
        if (ATI->DoAction("MoveLeft"))
            camObject->SetPosition(camObject->GetPosition() + nor * m_Speed * dt);
    }
    if (camType == CCamera::TC_ESF) {
        if (ATI->DoAction("scrollLittle", deltaZ))
            ((CThPSCamera *) m_ActiveCamera)->AddZoom(-deltaZ * m_LittleZoom * dt);
        else if (ATI->DoAction("scroll", deltaZ))
            ((CThPSCamera *) m_ActiveCamera)->AddZoom(-deltaZ * m_BigZoom * dt);
        float panX, panY;
        if (ATI->DoAction("PanX", panX))
            camObject->SetPosition(camObject->GetPosition() + Vect3f(0, 0, panX) * m_PanSpeed * dt);
        if (ATI->DoAction("PanY", panY))
            camObject->SetPosition(camObject->GetPosition() + Vect3f(panY, 0, 0) * m_PanSpeed * dt);
        if (ATI->DoAction("RotX", panX))
            camObject->SetYaw(camObject->GetYaw() - panX * dt);
        if (ATI->DoAction("RotY", panY))
            camObject->SetPitch(camObject->GetPitch() - panY * dt);
    }
}

void CCameraController::Update(std::string camera, float dt)
{
    CCamera* aux = m_ActiveCamera;
    m_ActiveCamera = m_Cameras[camera];
    Update(dt);
    m_ActiveCamera = aux;
}

