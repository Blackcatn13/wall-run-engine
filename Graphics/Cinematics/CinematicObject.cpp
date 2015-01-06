#include "CinematicObject.h"
#include "CinematicObjectKeyFrame.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Core\Core.h"


CCinematicObject::CCinematicObject(CXMLTreeNode &atts)
    : CCinematicPlayer()
    , m_CurrentKeyFrame(0)
    , m_PosInterpolated(atts(0).GetVect3fProperty("pos", Vect3f(0.0, 0.0, 0.0)))
    , m_YawInterpolated(atts(0).GetFloatProperty("yaw"))
    , m_PitchInterpolated(atts(0).GetFloatProperty("pitch"))
    , m_RollInterpolated(atts(0).GetFloatProperty("roll"))
    , m_ScaleInterpolated(atts(0).GetVect3fProperty("scale", Vect3f(1.0, 1.0, 1.0)))
{
    std::string l_Resource = atts.GetPszProperty("resource");
    std::string l_Layer = atts.GetPszProperty("renderable_objects_manager");
    m_RenderableObject = CCORE->GetRenderableManager()->GetResource(l_Resource);
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

CCinematicObject::~CCinematicObject()
{
    for (size_t i = 0; i < m_CinematicObjectKeyFrames.size(); ++i) {
        CHECKED_DELETE(m_CinematicObjectKeyFrames[i]);
    }
    m_CinematicObjectKeyFrames.clear();
}

void CCinematicObject::InterpolatePosition(Vect3f PointA, Vect3f PointB, float TimeA, float TimeB, float ElapsedTime)
{
    //Interpolación de la Posición
    if (PointB != m_PosInterpolated) {
        Vect3f l_NewInterpolatedPoint = m_PosInterpolated;
        Vect3f l_AuxVector = Vect3f();
        l_AuxVector.x = PointB.x - PointA.x;
        l_AuxVector.y = PointB.y - PointA.y;
        l_AuxVector.z = PointB.z - PointA.z;
        float l_PointIncreaseX = ( sqrt(pow(l_AuxVector.x, 2)) ) / ((TimeB - TimeA) / ElapsedTime);
        float l_PointIncreaseY = ( sqrt(pow(l_AuxVector.y, 2)) ) / ((TimeB - TimeA) / ElapsedTime);
        float l_PointIncreaseZ = ( sqrt(pow(l_AuxVector.z, 2)) ) / ((TimeB - TimeA) / ElapsedTime);
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

void CCinematicObject::InterpolateYaw(float YawA, float YawB, float TimeA, float TimeB, float ElapsedTime)
{
    // Interpolación de Yaw
    if (YawB != m_YawInterpolated) {
        float l_NewInterpolatedYaw = m_YawInterpolated;
        float l_YawIncrease;
        if (YawB > YawA) {
            if (YawA < 0.0 && YawB >= 0.0) {
                if ((YawA < (-FLOAT_PI_VALUE / 2)) && (YawB > (FLOAT_PI_VALUE / 2))) {
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
                    l_YawIncrease = ( ((FLOAT_PI_VALUE - YawA) + (YawB + FLOAT_PI_VALUE)) / ((TimeB - TimeA) / ElapsedTime) );
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

void CCinematicObject::InterpolatePitch(float PitchA, float PitchB, float TimeA, float TimeB, float ElapsedTime)
{
    //Interpolación de Pitch
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
                    l_PitchIncrease = ( ((FLOAT_PI_VALUE - PitchA) + (PitchB + FLOAT_PI_VALUE)) / ((TimeB - TimeA) / ElapsedTime) );
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

void CCinematicObject::InterpolateRoll(float RollA, float RollB, float TimeA, float TimeB, float ElapsedTime)
{
    //Interpolación de Roll
    if (RollB != m_RollInterpolated) {
        float l_NewInterpolatedRoll = m_RollInterpolated;
        float l_RollIncrease;
        if (RollB > RollA) {
            if (RollA < 0.0 && RollB >= 0.0) {
                if (RollA < (-FLOAT_PI_VALUE / 2) && RollB > (FLOAT_PI_VALUE / 2)) {
                    l_RollIncrease = ( ((FLOAT_PI_VALUE - RollB) + (RollA + FLOAT_PI_VALUE)) / ((TimeB - TimeA) / ElapsedTime) );
                    if (l_NewInterpolatedRoll - l_RollIncrease < -FLOAT_PI_VALUE) {
                        l_NewInterpolatedRoll = FLOAT_PI_VALUE - (-FLOAT_PI_VALUE - (l_NewInterpolatedRoll - l_RollIncrease));
                    } else {
                        l_NewInterpolatedRoll -= l_RollIncrease;
                    }
                } else {
                    l_RollIncrease = ((RollB - RollA) / ((TimeB - TimeA) / ElapsedTime));
                    l_NewInterpolatedRoll += l_RollIncrease;
                }
            } else {
                l_RollIncrease = ((RollB - RollA) / ((TimeB - TimeA) / ElapsedTime));
                l_NewInterpolatedRoll += l_RollIncrease;
            }
        } else {
            if (RollA >= 0.0 && RollB < 0.0) {
                if (RollA > (FLOAT_PI_VALUE / 2) && RollB < (-FLOAT_PI_VALUE / 2)) {
                    l_RollIncrease = ( ((FLOAT_PI_VALUE - RollA) + (RollB + FLOAT_PI_VALUE)) / ((TimeB - TimeA) / ElapsedTime) );
                    if (l_NewInterpolatedRoll + l_RollIncrease > FLOAT_PI_VALUE) {
                        l_NewInterpolatedRoll = -FLOAT_PI_VALUE + ((l_NewInterpolatedRoll + l_RollIncrease) - FLOAT_PI_VALUE);
                    } else {
                        l_NewInterpolatedRoll += l_RollIncrease;
                    }
                } else {
                    l_RollIncrease = (RollA - RollB) / ((TimeB - TimeA) / ElapsedTime);
                    l_NewInterpolatedRoll -= l_RollIncrease;
                }
            } else {
                l_RollIncrease = ((RollA - RollB) / ((TimeB - TimeA) / ElapsedTime));
                l_NewInterpolatedRoll -= l_RollIncrease;
            }
        }
        m_RollInterpolated = l_NewInterpolatedRoll;
    } else {
        m_RollInterpolated = RollB;
    }
}

void CCinematicObject::InterpolateScale(Vect3f ScaleA, Vect3f ScaleB, float TimeA, float TimeB, float ElapsedTime)
{
    //Interpolación de la Scale
    if (ScaleA != ScaleB) {
        Vect3f l_NewInterpolatedScale = m_ScaleInterpolated;
        Vect3f l_AuxVector = Vect3f();
        l_AuxVector.x = ScaleB.x - ScaleA.x;
        l_AuxVector.y = ScaleB.y - ScaleA.y;
        l_AuxVector.z = ScaleB.z - ScaleA.z;
        float l_ScaleIncreaseX = ( sqrt(pow(l_AuxVector.x, 2)) ) / ((TimeB - TimeA) / ElapsedTime);
        float l_ScaleIncreaseY = ( sqrt(pow(l_AuxVector.y, 2)) ) / ((TimeB - TimeA) / ElapsedTime);
        float l_ScaleIncreaseZ = ( sqrt(pow(l_AuxVector.z, 2)) ) / ((TimeB - TimeA) / ElapsedTime);
        if (ScaleB.x != l_NewInterpolatedScale.x) {
            if (ScaleB.x < l_NewInterpolatedScale.x) {
                l_NewInterpolatedScale.x -= l_ScaleIncreaseX;
            } else {
                l_NewInterpolatedScale.x += l_ScaleIncreaseX;
            }
        }
        if (ScaleB.y != l_NewInterpolatedScale.y) {
            if (ScaleB.y < l_NewInterpolatedScale.y) {
                l_NewInterpolatedScale.y -= l_ScaleIncreaseY;
            } else {
                l_NewInterpolatedScale.y += l_ScaleIncreaseY;
            }
        }
        if (ScaleB.z != l_NewInterpolatedScale.z) {
            if (ScaleB.z < l_NewInterpolatedScale.z) {
                l_NewInterpolatedScale.z -= l_ScaleIncreaseZ;
            } else {
                l_NewInterpolatedScale.z += l_ScaleIncreaseZ;
            }
        }
        m_ScaleInterpolated = l_NewInterpolatedScale;
    } else {
        m_ScaleInterpolated = ScaleB;
    }
}


void CCinematicObject::AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame)
{
    m_CinematicObjectKeyFrames.push_back(CinematicObjectKeyFrame);
}

void CCinematicObject::Update(float ElapsedTime)
{
    Vect3f l_Position;
    float l_Yaw;
    float l_Pitch;
    float l_Roll;
    Vect3f l_Scale;
    if (m_Playing == true) {
        float diff = m_CurrentTime - m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime();
        m_CurrentTime += ElapsedTime;
        if ( ((diff < ElapsedTime) && (diff > -ElapsedTime)) ) {
            l_Position = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition();
            l_Yaw = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetYaw();
            l_Pitch = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch();
            l_Roll = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll();
            l_Scale = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale();
            m_RenderableObject->SetPosition(l_Position);
            m_RenderableObject->SetYaw(l_Yaw);
            m_RenderableObject->SetPitch(l_Pitch);
            m_RenderableObject->SetRoll(l_Roll);
            m_RenderableObject->SetScale(l_Scale);
            if (m_CurrentKeyFrame == (m_CinematicObjectKeyFrames.size() - 1)) {
                if (m_Cycle == true) {
                    OnRestartCycle();
                } else {
                    l_Position = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition();
                    l_Yaw = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetYaw();
                    l_Pitch = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch();
                    l_Roll = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll();
                    l_Scale = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale();
                    m_RenderableObject->SetPosition(l_Position);
                    m_RenderableObject->SetYaw(l_Yaw);
                    m_RenderableObject->SetPitch(l_Pitch);
                    m_RenderableObject->SetRoll(l_Roll);
                    m_RenderableObject->SetScale(l_Scale);
                    m_Playing = false;
                    m_CurrentTime = 0.0;
                    m_CurrentKeyFrame = 0;
                    m_PosInterpolated = m_CinematicObjectKeyFrames[0]->GetPosition();
                    m_YawInterpolated = m_CinematicObjectKeyFrames[0]->GetYaw();
                    m_PitchInterpolated = m_CinematicObjectKeyFrames[0]->GetPitch();
                    m_RollInterpolated = m_CinematicObjectKeyFrames[0]->GetRoll();
                    m_ScaleInterpolated = m_CinematicObjectKeyFrames[0]->GetScale();
                }
            } else {
                m_CurrentKeyFrame += 1;
            }
        } else {
            if ( ( (m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime()) - (m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetKeyFrameTime()) ) > ( 0.034 ) ) {
                //Interpolación entre frames
                InterpolatePosition(m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetPosition(),
                                    m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition(),
                                    m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetKeyFrameTime(),
                                    m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime(),
                                    ElapsedTime);
                InterpolateYaw(m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetYaw(),
                               m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetYaw(),
                               m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetKeyFrameTime(),
                               m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime(),
                               ElapsedTime);
                InterpolatePitch(m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetPitch(),
                                 m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch(),
                                 m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetKeyFrameTime(),
                                 m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime(),
                                 ElapsedTime);
                InterpolateRoll(m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetRoll(),
                                m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll(),
                                m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetKeyFrameTime(),
                                m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime(),
                                ElapsedTime);
                InterpolateScale(m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetScale(),
                                 m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale(),
                                 m_CinematicObjectKeyFrames[m_CurrentKeyFrame - 1]->GetKeyFrameTime(),
                                 m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime(),
                                 ElapsedTime);
                m_RenderableObject->SetPosition(m_PosInterpolated);
                m_RenderableObject->SetYaw(m_YawInterpolated);
                m_RenderableObject->SetPitch(m_PitchInterpolated);
                m_RenderableObject->SetRoll(m_RollInterpolated);
                m_RenderableObject->SetScale(m_ScaleInterpolated);
            }
        }
    }
}

void CCinematicObject::Stop()
{
    m_Playing = false;
    m_CurrentTime = 0.0;
    m_CurrentKeyFrame = 0;
    m_PosInterpolated = m_CinematicObjectKeyFrames[0]->GetPosition();
    m_YawInterpolated = m_CinematicObjectKeyFrames[0]->GetYaw();
    m_PitchInterpolated = m_CinematicObjectKeyFrames[0]->GetPitch();
    m_RollInterpolated = m_CinematicObjectKeyFrames[0]->GetRoll();
    m_ScaleInterpolated = m_CinematicObjectKeyFrames[0]->GetScale();
    Vect3f l_Position;
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
    m_RenderableObject->SetScale(l_Scale);
}

void CCinematicObject::OnRestartCycle()
{
    m_CurrentTime = 0.0;
    m_CurrentKeyFrame = 0;
    m_PosInterpolated = m_CinematicObjectKeyFrames[0]->GetPosition();
    m_YawInterpolated = m_CinematicObjectKeyFrames[0]->GetYaw();
    m_PitchInterpolated = m_CinematicObjectKeyFrames[0]->GetPitch();
    m_RollInterpolated = m_CinematicObjectKeyFrames[0]->GetRoll();
    m_ScaleInterpolated = m_CinematicObjectKeyFrames[0]->GetScale();
}