#include "CameraKeyController.h"
#include "CameraKey.h"
#include "CameraInfo.h"
#include "Utils/Logger.h"
#include "Object\Object3D.h"
#include "Utils\Defines.h"
#include "Core_Utils/MemLeaks.h"
#include "Math\MathUtils.h"
#include "Utils\LerpAnimator1D.h"
#include "Utils\LerpAnimator3D.h"
#include <iostream>


CCameraKeyController::CCameraKeyController()
  : m_CurrentKey(0)
  , m_NextKey(0)
  , m_CurrentTime(0.0)
  , m_TotalTime(0.0)
  , m_IsPlaying(false)
  , m_Once(false)
  , m_Cycle(false)
  , m_Reverse(false)
  , m_IsReversing(false)
  , m_YawInterpolated(0.0)
  , m_PitchInterpolated(0.0)
  , CCamera(0, 0, 0, 1, 0, TC_CIN) {
  m_pObject3D = NULL;
}

CCameraKeyController::CCameraKeyController(CXMLTreeNode &atts)
{}

CCameraKeyController::~CCameraKeyController() {
  for (size_t i = 0; i < m_Keys.size(); i++) {
    CHECKED_DELETE(m_Keys[i]);
  }
  CHECKED_DELETE(m_pObject3D);
}

void CCameraKeyController::LoadXML(const std::string &FileName) {
  CXMLTreeNode l_XMLParser;
  if (!l_XMLParser.LoadFile(FileName.c_str())) {
    LOGGER->AddNewLog(ELL_ERROR, "No se ha encontrado el xml de camera: %s", FileName.c_str());
  } else {
    CXMLTreeNode  m = l_XMLParser["camera_key_controller"];
    if (m.Exists()) {
      if (m.GetNumChildren() > 1) m_NextKey = 1;
      bool l_once = m.GetBoolProperty("once");
      bool l_cycle = m.GetBoolProperty("cycle");
      bool l_reverse = false;//m.GetBoolProperty("reverse");
      float aspect = m.GetFloatProperty("aspect", 1.77777);
      m_fAspectRatio = aspect;
      SetOnce(l_once);
      SetCycle(l_cycle);
      SetReverse(l_reverse);
      Vect3f l_aux1 = m(0).GetVect3fProperty("pos", NULL, true);
      Vect3f l_aux2 = m(0).GetVect3fProperty("lookat", NULL, true);
      Vect3f l_V = l_aux1 - l_aux2;
      float l_yaw = atan2(l_V.z, l_V.x);
      float l_pitch = -atan2(l_V.y, sqrt((l_V.z * l_V.z) + (l_V.x * l_V.x)));
      float l_roll = 0.0f;
      m_pObject3D = new CObject3D(l_aux1, l_yaw, l_pitch, l_roll);
      m_PositionInit = l_aux1;
      m_YawInit = l_yaw;
      m_PitchInit = l_pitch;
      m_PosInterpolated = l_aux1;
      m_LookAt = l_aux2;
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode nodeChild = m.getNextChild();
        if (!strcmp(nodeChild.GetName(), "key")) {
          float l_time = nodeChild.GetFloatProperty("time");
          Vect3f l_pos = nodeChild.GetVect3fProperty("pos", NULL, true);
          Vect3f l_look_at = nodeChild.GetVect3fProperty("lookat", NULL, true);
          float l_fov = nodeChild.GetFloatProperty("fov");
          float l_near_plane = nodeChild.GetFloatProperty("near_plane");
          float l_far_plane = nodeChild.GetFloatProperty("far_plane");
          CCameraInfo *m_CamInfo = new CCameraInfo(l_pos, l_look_at, l_pos, l_near_plane, l_far_plane, l_fov);
          CCameraKey *m_CamKey = new CCameraKey(m_CamInfo, l_time);
          m_Keys.push_back(m_CamKey);
          m_TotalTime = l_time;
        }
      }
      m_fZNear = m_Keys[0]->m_CameraInfo->m_NearPlane;
      m_fZFar = m_Keys[0]->m_CameraInfo->m_FarPlane;
      m_fFOV = m_Keys[0]->m_CameraInfo->m_FOV;
      //m_pObject3D = m_Object;
    }
  }
}

void CCameraKeyController::GetCurrentKey() {
  if (!m_IsReversing) {
    if (m_CurrentTime >= m_Keys[m_NextKey]->m_Time) {
      if (m_NextKey == m_Keys.size() - 1) {
        ++m_CurrentKey;
        m_NextKey = 0;
      } else {
        ++m_CurrentKey;
        ++m_NextKey;
      }
    }
  } else {
    if (m_CurrentTime >= m_Keys[m_Keys.size() - (m_NextKey + 1)]->m_Time) {
      if (m_NextKey == 0) {
        --m_CurrentKey;
        m_NextKey = 0;
      } else {
        --m_CurrentKey;
        --m_NextKey;
      }
    }
  }
}

void CCameraKeyController::Update(float ElapsedTime) {
  if ((m_IsPlaying) && (m_Keys.size() > 1)) {
    m_CurrentTime += ElapsedTime;
    GetCurrentKey();
    if ( m_CurrentTime >= m_TotalTime ) {
      if (IsOnce()) m_IsPlaying = false;
      else {
        if (!IsCycle()) m_IsPlaying = false;
      }
      ResetTime(IsReverse());
      if (m_Keys.size() > 1) {
        if (!IsReverse()) m_NextKey = 1;
        else m_NextKey = m_Keys.size() - 2;
      } else m_NextKey = 0;
      Vect3f l_p = m_Keys[0]->m_CameraInfo->GetPos();
      Vect3f l_la = m_Keys[0]->m_CameraInfo->GetLookAt();
      Vect3f l_V = l_p - l_la;
      float l_yaw = atan2(l_V.z, l_V.x) - ePIf;
      float l_pitch = -atan2(l_V.y, sqrt((l_V.z * l_V.z) + (l_V.x * l_V.x)));
      m_PosInterpolated = l_p;
      m_YawInterpolated = l_yaw;
      m_PitchInterpolated = l_pitch;
      m_LookAt = l_la;
    } else {
      float currentP = (m_CurrentTime - m_Keys[m_CurrentKey]->GetTime()) / (m_Keys[m_NextKey]->GetTime() - m_Keys[m_CurrentKey]->GetTime());
      CLerpAnimator3D interpolator3D;
      interpolator3D.SetValues(m_Keys[m_CurrentKey]->m_CameraInfo->GetPos(), m_Keys[m_NextKey]->m_CameraInfo->GetPos(), 1.0f, FUNC_CONSTANT);
      interpolator3D.Update(currentP, m_PosInterpolated);
      interpolator3D.SetValues(m_Keys[m_CurrentKey]->m_CameraInfo->GetLookAt(), m_Keys[m_NextKey]->m_CameraInfo->GetLookAt(), 1.0f, FUNC_CONSTANT);
      interpolator3D.Update(currentP, m_LookAt);
      m_pObject3D->SetPosition(m_PosInterpolated);
      CLerpAnimator1D interpolator1D;
      interpolator1D.SetValues(m_Keys[m_CurrentKey]->m_CameraInfo->m_FOV, m_Keys[m_NextKey]->m_CameraInfo->m_FOV, 1.0f, FUNC_CONSTANT);
      interpolator1D.Update(currentP, m_fFOV);
    }
  }
}

void CCameraKeyController::SetCurrentTime(float CurrentTime) {
  m_CurrentTime = CurrentTime;
}

void CCameraKeyController::ResetTime(bool reverse) {
  m_CurrentTime = 0.0;
  if (!reverse || m_IsReversing) {
    m_CurrentKey = 0;
    m_IsReversing = false;
  } else {
    m_CurrentKey = m_Keys.size() - 1;
    m_IsReversing = true;
  }
}

void CCameraKeyController::SetOnce(bool Once) {
  m_Once = Once;
}

bool CCameraKeyController::IsOnce() const {
  return m_Once;
}

void CCameraKeyController::SetCycle(bool Cycle) {
  m_Cycle = Cycle;
}

bool CCameraKeyController::IsCycle() const {
  return m_Cycle;
}

void CCameraKeyController::SetReverse(bool Reverse) {
  m_Reverse = Reverse;
}

bool CCameraKeyController::IsReverse() const {
  return m_Reverse;
}

float CCameraKeyController::GetTotalTime() {
  return m_TotalTime;
}

void CCameraKeyController::Play() {
  m_IsPlaying = true;
}

void CCameraKeyController::Pause() {
  m_IsPlaying = false;
}

void CCameraKeyController::Stop() {
  m_IsPlaying = false;
  ResetTime(false);
  if (m_Keys.size() > 1) m_NextKey = 1;
  else m_NextKey = 0;
  m_PosInterpolated = m_PositionInit;
  m_YawInterpolated = m_YawInit;
  m_PitchInterpolated = m_PitchInit;
}

bool CCameraKeyController::IsPlayOn() {
  return m_IsPlaying;
}

Vect3f CCameraKeyController::GetDirection () const {
  return (m_LookAt - GetEye());
}

Vect3f CCameraKeyController::GetLookAt () const {
  return m_LookAt;
}

Vect3f CCameraKeyController::GetEye () const {
  return m_pObject3D->GetPosition();
}

Vect3f CCameraKeyController::GetVecUp () const {
  return Vect3f(0, 1, 0);
}


