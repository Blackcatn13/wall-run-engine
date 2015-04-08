#pragma once
#ifndef POLY_PLATFORM_H
#define POLY_PLATFORM_H

#include <string>
#include "Math\Vector3.h"
#include "SceneElements\StaticPlatform.h"
#include "Utils\Defines.h"

class CGraphicsManager;
class CLight;
class CPolyPlatform : public CStaticPlatform {
 protected:
  // std::string	m_RedimensionAxis;
  Vect3f		m_OriginalPosition;
  Vect3f		m_Direction;
  // float		m_RedimScale;
  bool		m_Activated;
  bool		m_Enabled;
  float		m_ActivationDistance;
  float		m_TimeOut;
  float		m_ActiveTime;
  // bool		m_Collission;
  bool		m_IsMoving;
  float		m_Speed;
  //std::string		m_RedimAxis;
  float		m_Dt;
  Vect3f		m_FinalPosition;
  CLight	 *m_Light;
  std::string m_LightName;
  Vect3f	m_LightOriginalPosition;

 public:
  CPolyPlatform(std::string platformName, std::string coreName,  Vect3f finalPosition, Vect3f direction,  float activationDistance);
  virtual  ~ CPolyPlatform ();
  void ActivatePoly();
  void DeactivatePoly();
  void Update(float ElapsedTime);
  void ApplyPhysicsToPlayer(Vect3f direction, float ElapsedTime);

  bool IsActivated() {
    return m_Activated;
  }
  void SetActivated(bool activated) {
    m_Activated = activated;
  }

  GET_SET(Vect3f, OriginalPosition)
  GET_SET(Vect3f, FinalPosition)
  GET_SET(Vect3f, Direction)
  // GET_SET(float, RedimScale)
  GET_SET(bool, Enabled)
  GET_SET(bool,  IsMoving)
  GET_SET(float, ActivationDistance)
  GET_SET(float, TimeOut)
  GET_SET(float, Speed)
  GET_SET(float, ActiveTime)
  GET_SET(std::string, LightName)
  GET_SET(Vect3f, LightOriginalPosition)

  CLight *GetLight() {return m_Light;}
  void SetLight(CLight *light) {m_Light = light;}
  //GET_SET(float, Collission)
  // GET_SET(std::string, RedimAxis)
  /* std::string getRedimAxis()
   {
       return m_RedimAxis;
   }
   void setRedimAxis(std::string redimaxis)
   {
       m_RedimAxis = redimaxis;
   }*/
};

#endif