#include "CameraKeyController.h"
#include "CameraKey.h"
#include "CameraInfo.h"
#include "Utils/Logger.h"
#include "Object\Object3D.h"
#include "Utils\Defines.h"
#include "Core_Utils/MemLeaks.h"


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
  , m_Object(NULL)
  , m_YawInterpolated(0.0)
  , m_PitchInterpolated(0.0)
  , CCamera(0, 0, 0, 1, 0, TC_CIN)
{}

CCameraKeyController::CCameraKeyController(CXMLTreeNode &atts)
{}

CCameraKeyController::~CCameraKeyController() {
  for (size_t i = 0; i < m_Keys.size(); i++) {
    CHECKED_DELETE(m_Keys[i]);
  }
  CHECKED_DELETE(m_Object);
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
      Vect3f l_aux2 = m(0).GetVect3fProperty("look_at", NULL, true);
      Vect3f l_V = l_aux1 - l_aux2;
      float l_yaw = atan2(l_V.z, l_V.x) - ePIf;
      float l_pitch = -atan2(l_V.y, sqrt((l_V.z * l_V.z) + (l_V.x * l_V.x)));
      float l_roll = 0.0f;
      m_Object = new CObject3D(l_aux1, l_yaw, l_pitch, l_roll);
      m_PositionInit = l_aux1;
      m_YawInit = l_yaw;
      m_PitchInit = l_pitch;
      m_PosInterpolated = l_aux1;
      m_YawInterpolated = l_yaw;
      m_PitchInterpolated = l_pitch;
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        if (!strcmp(m(i).GetName(), "key")) {
          float l_time = m(i).GetFloatProperty("time");
          Vect3f l_pos = m(i).GetVect3fProperty("pos", NULL, true);
          Vect3f l_look_at = m(i).GetVect3fProperty("look_at", NULL, true);
          float l_fov = m(i).GetFloatProperty("fov");
          float l_near_plane = m(i).GetFloatProperty("near_plane");
          float l_far_plane = m(i).GetFloatProperty("far_plane");
          CCameraInfo *m_CamInfo = new CCameraInfo(l_pos, l_look_at, l_pos, l_near_plane, l_far_plane, l_fov);
          CCameraKey *m_CamKey = new CCameraKey(m_CamInfo, l_time);
          m_Keys.push_back(m_CamKey);
          m_TotalTime = l_time;
        }
      }
      m_fZNear = m_Keys[0]->m_CameraInfo->m_NearPlane;
      m_fZFar = m_Keys[0]->m_CameraInfo->m_FarPlane;
      m_fFOV = m_Keys[0]->m_CameraInfo->m_FOV;
      m_pObject3D = m_Object;
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
    if ( ( !m_IsReversing && (m_Keys[m_NextKey]->GetTime() - m_Keys[m_CurrentKey]->GetTime() > 0.034) ) ||
         ( m_IsReversing && (m_Keys[m_CurrentKey]->GetTime() - m_Keys[m_NextKey]->GetTime() > 0.034) ) ) {
      Vect3f l_p, l_p2;
      Vect3f l_la;
      Vect3f l_V;
      float l_yaw, l_yaw2;
      float l_pitch, l_pitch2;
      l_p = m_Keys[m_CurrentKey]->m_CameraInfo->GetPos();
      l_la = m_Keys[m_CurrentKey]->m_CameraInfo->GetLookAt();
      l_V = l_p - l_la;
      l_yaw = atan2(l_V.z, l_V.x);
      l_pitch = -atan2(l_V.y, sqrt((l_V.z * l_V.z) + (l_V.x * l_V.x)));
      l_p2 = m_Keys[m_NextKey]->m_CameraInfo->GetPos();
      l_la = m_Keys[m_NextKey]->m_CameraInfo->GetLookAt();
      l_V = l_p2 - l_la;
      l_yaw2 = atan2(l_V.z, l_V.x);
      l_pitch2 = -atan2(l_V.y, sqrt((l_V.z * l_V.z) + (l_V.x * l_V.x)));
      //Interpolación entre frames
      InterpolatePosition(l_p, l_p2, m_Keys[m_CurrentKey]->GetTime(), m_Keys[m_NextKey]->GetTime(), ElapsedTime);
      InterpolateYaw(l_yaw, l_yaw2, m_Keys[m_CurrentKey]->GetTime(), m_Keys[m_NextKey]->GetTime(), ElapsedTime);
      InterpolatePitch(l_pitch, l_pitch2, m_Keys[m_CurrentKey]->GetTime(), m_Keys[m_NextKey]->GetTime(), ElapsedTime);
      m_Object->SetPosition(m_PosInterpolated);
      m_Object->SetYaw(m_YawInterpolated);
      m_Object->SetPitch(m_PitchInterpolated);
    }
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
  //Interpolación entre 2 puntos (posición).
  if (PointB != m_PosInterpolated) {
    Vect3f l_NewInterpolatedPoint = m_PosInterpolated;
    Vect3f l_AuxVector;
    l_AuxVector.x = PointB.x - PointA.x;
    l_AuxVector.y = PointB.y - PointA.y;
    l_AuxVector.z = PointB.z - PointA.z;
    //Para determinar la interpolación, calculamos la distancia y dividimos entre la constante de tiempo.
    float l_PointIncreaseX = (sqrt (pow(l_AuxVector.x, 2)) ) / ((TimeB - TimeA) / ElapsedTime);
    float l_PointIncreaseY = (sqrt (pow(l_AuxVector.y, 2)) ) / ((TimeB - TimeA) / ElapsedTime);
    float l_PointIncreaseZ = (sqrt (pow(l_AuxVector.z, 2)) ) / ((TimeB - TimeA) / ElapsedTime);
    if (PointB.x != l_NewInterpolatedPoint.x) {
      if (PointB.x < l_NewInterpolatedPoint.x) {
        l_NewInterpolatedPoint.x -= l_PointIncreaseX;
      } else {
        l_NewInterpolatedPoint.x += l_PointIncreaseX;
      }
    }
    if (PointB.y != l_NewInterpolatedPoint.y) {
      if (PointB.y < l_NewInterpolatedPoint.y) {
        l_NewInterpolatedPoint.y -= l_PointIncreaseY;
      } else {
        l_NewInterpolatedPoint.y += l_PointIncreaseY;
      }
    }
    if (PointB.z != l_NewInterpolatedPoint.z) {
      if (PointB.z < l_NewInterpolatedPoint.z) {
        l_NewInterpolatedPoint.z -= l_PointIncreaseZ;
      } else {
        l_NewInterpolatedPoint.z += l_PointIncreaseZ;
      }
    }
    m_PosInterpolated = l_NewInterpolatedPoint;
  } else {
    m_PosInterpolated = PointB;
  }
}

void CCameraKeyController::InterpolateYaw(float YawA, float YawB, float TimeA, float TimeB, float ElapsedTime) {
  //Interpolación entre 2 floats (yaw).
  if (YawB != m_YawInterpolated) {
    float l_NewInterpolatedYaw = m_YawInterpolated;
    float l_YawIncrease;
    if (YawB > YawA) {
      if (YawA < 0.0 && YawB >= 0.0) {
        if (YawA < (-FLOAT_PI_VALUE / 2) && YawB > (FLOAT_PI_VALUE / 2)) {
          l_YawIncrease = (((FLOAT_PI_VALUE - YawB) + (YawA + FLOAT_PI_VALUE)) / ((TimeB - TimeA) / ElapsedTime));
          if (l_NewInterpolatedYaw - l_YawIncrease < -FLOAT_PI_VALUE) {
            l_NewInterpolatedYaw = FLOAT_PI_VALUE - (-FLOAT_PI_VALUE - (l_NewInterpolatedYaw - l_YawIncrease));
          } else {
            l_NewInterpolatedYaw -= l_YawIncrease;
          }
        } else {
          l_YawIncrease = ((YawB - YawA) / ((TimeB - TimeA) / ElapsedTime));
          l_NewInterpolatedYaw += l_YawIncrease;
        }
      } else {
        l_YawIncrease = ((YawB - YawA) / ((TimeB - TimeA) / ElapsedTime));
        l_NewInterpolatedYaw += l_YawIncrease;
      }
    } else {
      if (YawA >= 0.0 && YawB < 0.0) {
        if (YawA > (FLOAT_PI_VALUE / 2) && YawB < (-FLOAT_PI_VALUE / 2)) {
          l_YawIncrease = (((FLOAT_PI_VALUE - YawA) + (YawB + FLOAT_PI_VALUE)) / ((TimeB - TimeA) / ElapsedTime));
          if (l_NewInterpolatedYaw + l_YawIncrease > FLOAT_PI_VALUE) {
            l_NewInterpolatedYaw = -FLOAT_PI_VALUE + ((l_NewInterpolatedYaw + l_YawIncrease) - FLOAT_PI_VALUE);
          } else {
            l_NewInterpolatedYaw += l_YawIncrease;
          }
        } else {
          l_YawIncrease = (YawA - YawB) / ((TimeB - TimeA) / ElapsedTime);
          l_NewInterpolatedYaw -= l_YawIncrease;
        }
      } else {
        l_YawIncrease = ((YawA - YawB) / ((TimeB - TimeA) / ElapsedTime));
        l_NewInterpolatedYaw -= l_YawIncrease;
      }
    }
    m_YawInterpolated = l_NewInterpolatedYaw;
  } else {
    m_YawInterpolated = YawB;
  }
}

void CCameraKeyController::InterpolatePitch(float PitchA, float PitchB, float TimeA, float TimeB, float ElapsedTime) {
  //Interpolación entre 2 floats (pitch).
  if (PitchB != m_PitchInterpolated) {
    float l_NewInterpolatedPitch = m_PitchInterpolated;
    float l_PitchIncrease;
    if (PitchB > PitchA) {
      if (PitchA < 0.0 && PitchB >= 0.0) {
        if (PitchA < (-FLOAT_PI_VALUE / 2) && PitchB > (FLOAT_PI_VALUE / 2)) {
          l_PitchIncrease = (((FLOAT_PI_VALUE - PitchB) + (PitchA + FLOAT_PI_VALUE)) / ((TimeB - TimeA) / ElapsedTime));
          if (l_NewInterpolatedPitch - l_PitchIncrease < -FLOAT_PI_VALUE) {
            l_NewInterpolatedPitch = FLOAT_PI_VALUE - (-FLOAT_PI_VALUE - (l_NewInterpolatedPitch - l_PitchIncrease));
          } else {
            l_NewInterpolatedPitch -= l_PitchIncrease;
          }
        } else {
          l_PitchIncrease = ((PitchB - PitchA) / ((TimeB - TimeA) / ElapsedTime));
          l_NewInterpolatedPitch += l_PitchIncrease;
        }
      } else {
        l_PitchIncrease = ((PitchB - PitchA) / ((TimeB - TimeA) / ElapsedTime));
        l_NewInterpolatedPitch += l_PitchIncrease;
      }
    } else {
      if (PitchA >= 0.0 && PitchB < 0.0) {
        if (PitchA > (FLOAT_PI_VALUE / 2) && PitchB < (-FLOAT_PI_VALUE / 2)) {
          l_PitchIncrease = (((FLOAT_PI_VALUE - PitchA) + (PitchB + FLOAT_PI_VALUE)) / ((TimeB - TimeA) / ElapsedTime));
          if (l_NewInterpolatedPitch + l_PitchIncrease > FLOAT_PI_VALUE) {
            l_NewInterpolatedPitch = -FLOAT_PI_VALUE + ((l_NewInterpolatedPitch + l_PitchIncrease) - FLOAT_PI_VALUE);
          } else {
            l_NewInterpolatedPitch += l_PitchIncrease;
          }
        } else {
          l_PitchIncrease = (PitchA - PitchB) / ((TimeB - TimeA) / ElapsedTime);
          l_NewInterpolatedPitch -= l_PitchIncrease;
        }
      } else {
        l_PitchIncrease = ((PitchA - PitchB) / ((TimeB - TimeA) / ElapsedTime));
        l_NewInterpolatedPitch -= l_PitchIncrease;
      }
    }
    m_PitchInterpolated = l_NewInterpolatedPitch;
  } else {
    m_PitchInterpolated = PitchB;
  }
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


