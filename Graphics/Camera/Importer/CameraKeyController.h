#pragma once
#ifndef CAMERA_KEY_CONTROLLER_H_
#define CAMERA_KEY_CONTROLLER_H_

#include "XML\XMLTreeNode.h"
#include "Object\Object3D.h"
#include <vector>
#include "Camera\Camera.h"

class CCameraKey;
class CCameraInfo;

class CCameraKeyController: public CCamera {
 private:
  std::vector<CCameraKey *>			m_Keys;
  size_t								        m_CurrentKey;
  size_t                        m_NextKey;
  float								          m_CurrentTime;
  float                         m_TotalTime;
  bool								          m_IsPlaying;
  bool								          m_Once;
  bool								          m_Cycle;
  bool								          m_Reverse;
  bool								          m_IsReversing;
  //CObject3D							        *m_Object;

  Vect3f								        m_PosInterpolated;
  float								          m_YawInterpolated;
  float								          m_PitchInterpolated;
  Vect3f                        m_LookAt;

  Vect3f								        m_PositionInit;
  float								          m_YawInit;
  float								          m_PitchInit;

  void GetCurrentKey();
 public:
  CCameraKeyController();
  CCameraKeyController(CXMLTreeNode &atts);
  ~CCameraKeyController();
  void LoadXML(const std::string &FileName);
  void Update(float ElapsedTime);
  void SetCurrentTime(float CurrentTime);
  void ResetTime(bool reverse);
  float GetTotalTime();
  bool IsOnce() const;
  void SetOnce(bool Once);
  bool IsCycle() const;
  void SetCycle(bool Cycle);
  bool IsReverse() const;
  void SetReverse(bool Reverse);

  void Play();
  void Pause();
  void Stop();
  bool IsPlayOn();

  virtual Vect3f GetDirection() const;
  virtual Vect3f GetLookAt() const;
  virtual Vect3f GetEye() const;
  virtual Vect3f GetVecUp() const;

  CObject3D *GetObject3D() {
    return m_pObject3D;
  }
};

#endif