#include "CameraKeyController.h"
#include "CameraKey.h"
#include "CameraInfo.h"
#include "Utils/Logger.h"
#include "Object\Object3D.h"
#include "Utils\Defines.h"
#include "Core_Utils/MemLeaks.h"
#include "Math\MathUtils.h"


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
      m_YawInterpolated = l_yaw;
      m_PitchInterpolated = l_pitch;
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
    SetCurrentTime(m_CurrentTime + ElapsedTime);
    GetCurrentKey();
    //Si el siguiente keyframe está a más de 1 frame de distancia (0.034 seg), toca interpolar.
    /*if ( ( !m_IsReversing && (m_Keys[m_NextKey]->GetTime() - m_Keys[m_CurrentKey]->GetTime() > 0.034) ) ||
         ( m_IsReversing && (m_Keys[m_CurrentKey]->GetTime() - m_Keys[m_NextKey]->GetTime() > 0.034) ) ) {
      Vect3f l_p, l_p2;
      Vect3f l_la;
      Vect3f l_V;
      float l_yaw, l_yaw2;
      float l_pitch, l_pitch2;
      l_p = m_Keys[m_CurrentKey]->m_CameraInfo->GetPos();
      l_la = m_Keys[m_CurrentKey]->m_CameraInfo->GetLookAt();
      l_V = l_p - l_la;
      l_yaw = atan2(l_V.x, l_V.z);
      l_pitch = atan2(sqrt((l_V.z * l_V.z) + (l_V.x * l_V.x)), l_V.y);
      l_p2 = m_Keys[m_NextKey]->m_CameraInfo->GetPos();
      l_la = m_Keys[m_NextKey]->m_CameraInfo->GetLookAt();
      l_V = l_p2 - l_la;
      l_yaw2 = atan2(l_V.x, l_V.z);
      l_pitch2 = atan2(sqrt((l_V.z * l_V.z) + (l_V.x * l_V.x)), l_V.y);
      //Interpolación entre frames
      InterpolatePosition(l_p, l_p2, m_Keys[m_CurrentKey]->GetTime(), m_Keys[m_NextKey]->GetTime(), ElapsedTime);
      InterpolateYaw(l_yaw, l_yaw2, m_Keys[m_CurrentKey]->GetTime(), m_Keys[m_NextKey]->GetTime(), ElapsedTime);
      InterpolatePitch(l_pitch, l_pitch2, m_Keys[m_CurrentKey]->GetTime(), m_Keys[m_NextKey]->GetTime(), ElapsedTime);
      m_pObject3D->SetPosition(m_PosInterpolated);
      m_pObject3D->SetYaw(m_YawInterpolated);
      m_pObject3D->SetPitch(m_PitchInterpolated);
    }*/
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
    } else {

      Vect3f l_p, l_p2;
      Vect3f l_la;
      Vect3f l_V;
      float l_yaw, l_yaw2;
      float l_pitch, l_pitch2;
      l_p = m_Keys[m_CurrentKey]->m_CameraInfo->GetPos();
      l_la = m_Keys[m_CurrentKey]->m_CameraInfo->GetLookAt();
      l_V = l_p - l_la;
      l_yaw = atan2(l_V.z, l_V.x);
      l_pitch = atan2(l_V.y, sqrt((l_V.z * l_V.z) + (l_V.x * l_V.x)));
      l_p2 = m_Keys[m_NextKey]->m_CameraInfo->GetPos();
      l_la = m_Keys[m_NextKey]->m_CameraInfo->GetLookAt();
      l_V = l_p2 - l_la;
      l_yaw2 = atan2(l_V.z, l_V.x);
      l_pitch2 = atan2(l_V.y, sqrt((l_V.z * l_V.z) + (l_V.x * l_V.x)));
      //Interpolación entre frames
      InterpolatePosition(l_p, l_p2, m_Keys[m_CurrentKey]->GetTime(), m_Keys[m_NextKey]->GetTime(), ElapsedTime);
      InterpolateYaw(l_yaw, l_yaw2, m_Keys[m_CurrentKey]->GetTime(), m_Keys[m_NextKey]->GetTime(), ElapsedTime);
      InterpolatePitch(l_pitch, l_pitch2, m_Keys[m_CurrentKey]->GetTime(), m_Keys[m_NextKey]->GetTime(), ElapsedTime);
      m_pObject3D->SetPosition(m_PosInterpolated);
      m_pObject3D->SetYaw(m_YawInterpolated);
      m_pObject3D->SetPitch(m_PitchInterpolated);

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


void CCameraKeyController::InterpolatePosition(Vect3f PointA, Vect3f PointB, float TimeA, float TimeB, float ElapsedTime) {
  float t = (m_CurrentTime - TimeA) / TimeB;
  m_PosInterpolated = PointA;
  m_PosInterpolated.Lerp(PointB, t);
}

void CCameraKeyController::InterpolateYaw(float YawA, float YawB, float TimeA, float TimeB, float ElapsedTime) {
  float t = (m_CurrentTime - TimeA) / TimeB;
  m_YawInterpolated = mathUtils::Lerp(YawA, YawB, t);
}

void CCameraKeyController::InterpolatePitch(float PitchA, float PitchB, float TimeA, float TimeB, float ElapsedTime) {
  float t = (m_CurrentTime - TimeA) / TimeB;
  m_PitchInterpolated = mathUtils::Lerp(PitchA, PitchB, t);
}

Vect3f CCameraKeyController::GetDirection () const {
  return (GetLookAt() - GetEye());
}

Vect3f CCameraKeyController::GetLookAt () const {
  return m_Keys[m_CurrentKey]->m_CameraInfo->GetLookAt();
}

Vect3f CCameraKeyController::GetEye () const {
  return m_pObject3D->GetPosition();
}

Vect3f CCameraKeyController::GetVecUp () const {
  float yaw		= m_pObject3D->GetYaw();
  float pitch	= m_pObject3D->GetPitch();
  Vect3f vUpVec(	-cos(yaw) * sin(pitch),
                  cos(pitch),
                  -sin(yaw) * sin(pitch) );
  return vUpVec;
}


