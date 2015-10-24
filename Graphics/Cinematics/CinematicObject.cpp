#include "CinematicObject.h"
#include "CinematicObjectKeyFrame.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "Core_Utils/MemLeaks.h"
#include "Utils\LerpAnimator1D.h"
#include "Utils\LerpAnimator3D.h"


CCinematicObject::CCinematicObject(CXMLTreeNode &atts)
  : CCinematicPlayer()
  , m_CurrentKeyFrame(0)
  , m_NextKeyFrame(1) {
  std::string l_Resource = atts.GetPszProperty("resource");
  std::string l_Layer = atts.GetPszProperty("renderable_objects_manager");
  int room = atts.GetIntProperty("room", 0);
  m_RenderableObject = RENDLM->GetRenderableObjectsManagerByStrAndRoom(l_Layer, room)->GetResource(l_Resource);
  int count = atts.GetNumChildren();
  for (int i = 0; i < count; ++i) {
    if (!strcmp(atts(i).GetName(), "cinematic_object_key_frame")) {
      CCinematicObjectKeyFrame *l_cokf = new CCinematicObjectKeyFrame(atts(i));
      AddCinematicObjectKeyFrame(l_cokf);
    }
    if (i == (count - 1)) {
      m_Duration = atts(i).GetFloatProperty("time");
    }
  }
}

CCinematicObject::~CCinematicObject() {
  for (size_t i = 0; i < m_CinematicObjectKeyFrames.size(); ++i) {
    CHECKED_DELETE(m_CinematicObjectKeyFrames[i]);
  }
  m_CinematicObjectKeyFrames.clear();
}

void CCinematicObject::AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame) {
  m_CinematicObjectKeyFrames.push_back(CinematicObjectKeyFrame);
}

void CCinematicObject::CalculateFrame() {
  for (size_t i = m_CurrentKeyFrame; i < m_CinematicObjectKeyFrames.size(); ++i) {
    if (m_CinematicObjectKeyFrames[i]->GetKeyFrameTime() > m_CurrentTime) {
      if (i == 0)
        i++;
      m_CurrentKeyFrame = i - 1;
      if (m_CinematicObjectKeyFrames.size() > i)
        m_NextKeyFrame = i;
      return;
    }
  }
}

void CCinematicObject::Update(float ElapsedTime) {
  if (m_Playing == true) {
    m_CurrentTime += ElapsedTime;
    if (m_CurrentTime >= m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime()) {
      if (m_CurrentTime > m_Duration) {
        Stop();
      } else {
        CalculateFrame();

        CCinematicObjectKeyFrame *current = m_CinematicObjectKeyFrames[m_CurrentKeyFrame];
        CCinematicObjectKeyFrame *next = m_CinematicObjectKeyFrames[m_NextKeyFrame];

        float currentP = (m_CurrentTime - current->GetKeyFrameTime()) / (next->GetKeyFrameTime() - current->GetKeyFrameTime());
        CLerpAnimator1D interpolator1D;
        CLerpAnimator3D interpolator3D;

        interpolator3D.SetValues(current->GetPosition(), next->GetPosition(), 1.0f, FUNC_CONSTANT);
        Vect3f position;
        interpolator3D.Update(currentP, position);
        m_RenderableObject->SetPosition(position);

        interpolator3D.SetValues(current->GetScale(), next->GetScale(), 1.0f, FUNC_CONSTANT);
        Vect3f scale;
        interpolator3D.Update(currentP, scale);
        m_RenderableObject->SetScale(scale);

        interpolator1D.SetValues(current->GetYaw(), next->GetYaw(), 1.0f, FUNC_CONSTANT);
        float yaw;
        interpolator1D.Update(currentP, yaw);
        m_RenderableObject->SetYaw(yaw);

        interpolator1D.SetValues(current->GetPitch(), next->GetPitch(), 1.0f, FUNC_CONSTANT);
        float pitch;
        interpolator1D.Update(currentP, pitch);
        m_RenderableObject->SetPitch(pitch);

        interpolator1D.SetValues(current->GetRoll(), next->GetRoll(), 1.0f, FUNC_CONSTANT);
        float roll;
        interpolator1D.Update(currentP, roll);
        m_RenderableObject->SetRoll(roll);

        if (m_CurrentKeyFrame == (m_CinematicObjectKeyFrames.size() - 1)) {
          if (m_Cycle == true) {
            OnRestartCycle();
          } else {
            Stop();
          }
        }
      }
    }
  }
}

void CCinematicObject::Stop() {
  m_Playing = false;
  m_CurrentTime = 0.0;
  m_CurrentKeyFrame = 0;
  m_NextKeyFrame = 1;
  /*Vect3f l_Position;
  float l_Yaw;
  float l_Pitch;
  float l_Roll;
  Vect3f l_Scale;
  l_Position = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition();
  l_Yaw = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetYaw();
  l_Pitch = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch();
  l_Roll = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll();
  l_Scale = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale();
  m_RenderableObject->SetPosition(l_Position);
  m_RenderableObject->SetYaw(l_Yaw);
  m_RenderableObject->SetPitch(l_Pitch);
  m_RenderableObject->SetRoll(l_Roll);
  m_RenderableObject->SetScale(l_Scale);*/
}

void CCinematicObject::OnRestartCycle() {
  m_CurrentTime = 0.0;
  m_CurrentKeyFrame = 0;
  m_NextKeyFrame = 1;
}