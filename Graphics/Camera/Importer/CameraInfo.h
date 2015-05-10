#pragma once
#ifndef CAMERA_INFO_H_
#define CAMERA_INFO_H_

#include "Math\Vector3.h"
#include "XML\XMLTreeNode.h"

class CCameraInfo {
 public:
  float				m_NearPlane;
  float       m_FarPlane;
  float				m_FOV;
  Vect3f			m_Eye;
  Vect3f      m_LookAt;
  Vect3f			m_Up;

  CCameraInfo();
  CCameraInfo(const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, float NearPlane, float FarPlane, float FOV);
  CCameraInfo(CXMLTreeNode &atts);

  Vect3f GetPos() {
    return m_Eye;
  }
  Vect3f GetLookAt() {
    return m_LookAt;
  }
};

#endif

