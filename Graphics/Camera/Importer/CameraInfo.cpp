#include "CameraInfo.h"
#include "Core_Utils/MemLeaks.h"

CCameraInfo::CCameraInfo() {
}

CCameraInfo::CCameraInfo(const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, float NearPlane, float FarPlane, float FOV) {
  m_Eye = Eye;
  m_LookAt = LookAt;
  m_Up = Up;
  m_NearPlane = NearPlane;
  m_FarPlane = FarPlane;
  m_FOV = FOV;
}

CCameraInfo::CCameraInfo(CXMLTreeNode &atts) {
}