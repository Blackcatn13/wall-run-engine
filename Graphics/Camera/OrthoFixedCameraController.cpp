#include "OrthoFixedCameraController.h"
#include "Object/Object3D.h"
#include "Camera.h"
#include "Camera\OrthoFixedCamera.h"
#include "Utils\Logger.h"
#include "Core_Utils/MemLeaks.h"

COrthoFixedCameraController::COrthoFixedCameraController(Vect3f shadow, Vect3f position, float width, float heigth, float aspectRatio, float endAttenuation) {
  CObject3D *l_Object = new CObject3D(position, 0.f, 0.f);
  m_Camera = COrthoFixedCamera(0.1, endAttenuation, 45.0f * D3DX_PI / 180.0f, 1.f, l_Object);
  m_Camera.SetWidth(width);
  m_Camera.SetHeight(heigth);
}

COrthoFixedCameraController::~COrthoFixedCameraController() {
}

