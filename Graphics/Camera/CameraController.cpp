#include "CameraController.h"
#include "Object/Object3D.h"
#include "Camera.h"
#include "FPSCamera.h"
#include "ThPSCamera.h"
#include "Core/Core.h"
#include "ActionToInput.h"
#include "Math/Vector3.h"
#include "Math/MathTypes.h"
#include "Importer\CameraKeyController.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Logger.h"
#include "Core_Utils/MemLeaks.h"

CCameraController::CCameraController() : m_Speed(2),
  m_PanSpeed(10),
  m_FlyMode(false),
  m_FPSMaxPitch(90),
  m_ThPSMaxPitch(90),
  m_BigZoom(20),
  m_LittleZoom(1),
  m_FileName("") {
}

CCameraController::~CCameraController() {
  Destroy();
  for (map2Object::iterator it = m_Objects.begin(); it != m_Objects.end(); ++it) {
    CHECKED_DELETE(it->second);
  }
  m_Objects.clear();
}

std::string CCameraController::GetCameraName(CCamera *cam) {
  TMapResource::iterator it = GetResources().begin();
  for (it; it != GetResources().end(); ++it) {
    if (it->second == cam)
      return it->first;
  }
  return NULL;
}

void CCameraController::AddNewObject(std::string objName, CObject3D *obj) {
  m_Objects.insert(PairString2Object(objName, obj));
}

void CCameraController::AddNewCamera(std::string camName, CCamera *cam) {
  //m_Cameras.insert(PairString2Camera(camName, cam));
  AddResource(camName, cam);
}

void CCameraController::Update(float dt) {
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
#ifdef _PARTICLEVIEWER
  float deltaX, deltaY, deltaZ;
  CCamera::ETypeCamera camType = m_ActiveCamera->GetTypeCamera();
  CObject3D *camObject = m_ActiveCamera->GetObject3D();
  CActionToInput *ATI = ACT2IN;
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
    /*if (ATI->DoAction("PanX", panX))
      camObject->SetPosition(camObject->GetPosition() + Vect3f(0, 0, panX) * m_PanSpeed * dt);
    if (ATI->DoAction("PanY", panY))
      camObject->SetPosition(camObject->GetPosition() + Vect3f(panY, 0, 0) * m_PanSpeed * dt);
    */
    if (ATI->DoAction("RotX", panX))
      camObject->SetYaw(camObject->GetYaw() - panX * dt);
    if (ATI->DoAction("RotY", panY))
      camObject->SetPitch(camObject->GetPitch() - panY * dt);
  }
  //CamUpdates(camType, dt);
  //CCamera::ETypeCamera camType = m_ActiveCamera->GetTypeCamera();
  if (camType == CCamera::TC_CIN) {
    CamUpdates(camType, dt);
  }
#endif
}

void CCameraController::CamUpdates(CCamera::ETypeCamera camType, float dt) {
  //CINEMATICS
  TMapResource::iterator it = m_Resources.begin();
  for (it; it != m_Resources.end(); ++it) {
    if (camType == CCamera::TC_CIN) {
      ((CCameraKeyController *)(* it).second)->Update(dt);
    }
  }

}
void CCameraController::Update(std::string camera, float dt) {
  CCamera *aux = m_ActiveCamera;
  m_ActiveCamera = m_Resources[camera];
  Update(dt);
  m_ActiveCamera = aux;
}

// -------------------------------------------------------------------------------------------------
// CINEMATICS
// -------------------------------------------------------------------------------------------------

//Función para cargar cámeras para las cinemáticas.
bool CCameraController::Load(const std::string &FileName) {
  m_FileName = FileName;
  CXMLTreeNode l_XMLParser;
  if (!l_XMLParser.LoadFile(FileName.c_str())) {
    LOGGER->AddNewLog(ELL_ERROR, "No se ha encontrado el xml de cameras: %s", FileName.c_str());
    return false;
  } else {
    CXMLTreeNode  m = l_XMLParser["cameras"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode nodeChild = m.getNextChild();
        if (!strcmp(nodeChild.GetName(), "key_camera_controller")) {
          std::string l_Name = nodeChild.GetPszProperty("name");
          std::string l_file = nodeChild.GetPszProperty("file");
          CCameraKeyController *l_CameraKeyController = new CCameraKeyController();
          l_CameraKeyController->LoadXML(l_file);
          bool l_Ret = AddResource(l_Name, l_CameraKeyController);
          //m_Cameras.insert(PairString2Camera(l_Name, l_CameraKeyController));
          if (l_Ret == false) {
            CHECKED_DELETE(l_CameraKeyController);
          }
        }
        if (!strcmp(nodeChild.GetName(), "fixed_camera")) {
          std::string l_Name = nodeChild.GetPszProperty("name");
          Vect3f pos = nodeChild.GetVect3fProperty("pos", v3fONE);
          Vect3f lookat = nodeChild.GetVect3fProperty("lookat", v3fONE);
          float l_fov = nodeChild.GetFloatProperty("fov");
          float l_near_plane = nodeChild.GetFloatProperty("near_plane");
          float l_far_plane = nodeChild.GetFloatProperty("far_plane");
          Vect3f l_V = pos - lookat;
          float l_yaw = atan2(l_V.z, l_V.x) - ePIf;
          float l_pitch = -atan2(l_V.y, sqrt((l_V.z * l_V.z) + (l_V.x * l_V.x)));
          float l_roll = 0.0f;
          CObject3D *m_Object = new CObject3D(pos, l_yaw, l_pitch, l_roll);
          m_Objects.insert(PairString2Object(l_Name, m_Object));
          CFPSCamera *cam = new CFPSCamera(l_near_plane, l_far_plane, l_fov, 1, m_Object);
          AddResource(l_Name, cam);
          //m_Cameras.insert(PairString2Camera(l_Name, cam));
        }
      }
    }
  }
  return true;
}

bool CCameraController::Reload() {
  Destroy();
  return Load(m_FileName);
}

bool CCameraController::Reload(const std::string &FileName) {
  Destroy();
  return Load(FileName);
}

bool CCameraController::IsAnyCinematicPlaying() {
  CCamera::ETypeCamera camType;
  TMapResource::iterator it = m_Resources.begin();
  for (it; it != m_Resources.end(); ++it) {
    camType = it->second->GetTypeCamera();
    if (camType == CCamera::TC_CIN) {
      if (((CCameraKeyController *)(* it).second)->IsPlayOn())
        return true;
    }
  }
  return false;
}

void CCameraController::Play(bool Cycle) {
  CCamera::ETypeCamera camType;
  TMapResource::iterator it = m_Resources.begin();
  for (it; it != m_Resources.end(); ++it) {
    camType = it->second->GetTypeCamera();
    if (camType == CCamera::TC_CIN) {
      ((CCameraKeyController *)(* it).second)->Play();
      ((CCameraKeyController *)(* it).second)->SetCycle(Cycle);
    }
  }
}

void CCameraController::Pause() {
  CCamera::ETypeCamera camType;
  TMapResource::iterator it = m_Resources.begin();
  for (it; it != m_Resources.end(); ++it) {
    camType = it->second->GetTypeCamera();
    if (camType == CCamera::TC_CIN) {
      ((CCameraKeyController *)(* it).second)->Pause();
    }
  }
}

void CCameraController::Stop() {
  CCamera::ETypeCamera camType;
  TMapResource::iterator it = m_Resources.begin();
  for (it; it != m_Resources.end(); ++it) {
    camType = it->second->GetTypeCamera();
    if (camType == CCamera::TC_CIN) {
      ((CCameraKeyController *)(* it).second)->Stop();
    }
  }
}

void CCameraController::UpdateCinematicCameras(float dt) {
  CCamera::ETypeCamera camType;
  TMapResource::iterator it = m_Resources.begin();
  for (it; it != m_Resources.end(); ++it) {
    camType = it->second->GetTypeCamera();
    if (camType == CCamera::TC_CIN) {
      ((CCameraKeyController *)(it)->second)->Update(dt);
    }
  }
}

