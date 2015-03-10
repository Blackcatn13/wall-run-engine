#include "PolyPlatform.h"
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Core\PlayerController.h"
#include "Core\Core.h"
#include "Utils\PhysicUserData.h"
#include "Core\ScriptManager.h"
#include "Utils\Logger.h"


CPolyPlatform::CPolyPlatform(std::string platformName, std::string coreName, std::string redimAxis, float redimScale,  float activationDistance)
    : CStaticPlatform(platformName, coreName),
      m_RedimScale(redimScale),
      m_OriginalScale(v3fZERO),
      m_RedimAxis(redimAxis),
      m_Enabled(false),
      // m_InitialCollission(initialCollision),
      m_ActivationDistance(activationDistance),
      m_ActiveTime(.0f),
      m_Activated(false)
{
}

CPolyPlatform::~CPolyPlatform ()
{
    //PHYSXM->ReleasePhysicActor(m_PlatorformActor);
}


void CPolyPlatform:: ActivatePoly()
{
    if (!m_Activated) {
        /* Vect3f l_PlayerPosition =  CCORE->GetPlayerController()->getPhysicController()->GetPosition();
         CCORE->GetPlayerController()->getPhysicController()->SetPosition(l_PlayerPosition + m_RedimScale);*/
        float l_NewScale = 0.0f;
        /*  if (GetScale() == m_OriginalScale) {
              m_PlatorformActor->Activate(!m_InitialCollission);
              l_NewScale = GetScale() + m_RedimScale;
          } else {
              l_NewScale = GetScale() - m_RedimScale;
              m_PlatorformActor->Activate(m_InitialCollission);
          }*/
        if (m_RedimAxis == "x") {
            if (GetScale().x != m_RedimScale) {
                l_NewScale = m_RedimScale;
                m_PlatorformActor->Activate(true);
            } else {
                l_NewScale = 0;
                m_PlatorformActor->Activate(false);
            }
            SetScale(Vect3f(l_NewScale, GetScale().y, GetScale().z));
            m_Activated = true;
        } else if (m_RedimAxis == "y") {
            if (GetScale().y != m_RedimScale) {
                l_NewScale = m_RedimScale;
                m_PlatorformActor->Activate(true);
            } else {
                l_NewScale = 0;
                m_PlatorformActor->Activate(false);
            }
            SetScale(Vect3f(GetScale().x, l_NewScale , GetScale().z));
            m_Activated = true;
        } else if (m_RedimAxis == "z") {
            if (GetScale().z != m_RedimScale) {
                l_NewScale = m_RedimScale;
                m_PlatorformActor->Activate(true);
            } else {
                l_NewScale = 0;
                m_PlatorformActor->Activate(false);
            }
            SetScale(Vect3f(GetScale().x, GetScale().y , l_NewScale));
            m_Activated = true;
        } else {
            std::string msg_error = "No redimension Axis defined";
            LOGGER->AddNewLog(ELL_INFORMATION, msg_error.c_str());
        }
    }
}

void CPolyPlatform:: DeactivatePoly()
{
    if (m_Activated) {
        m_ActiveTime = 0.0f;
        m_PlatorformActor->Activate(false);
        SetScale(m_OriginalScale);
        if (GetScale() == m_OriginalScale) {
            m_Activated = false;
            if (m_RedimAxis == "y")
                CCORE->GetPlayerController()->getPhysicController()->Move(Vect3f(.0f, -1.f, .0f), m_Dt);
        }
    } else
        m_PlatorformActor->Activate(true);
}

void CPolyPlatform::Update(float ElapsedTime)
{
    //Esto para el lua de momento
    /*float l_Distance = CCORE->GetPlayerController()->GetPosition().Distance(m_Position);
    if (l_Distance < m_ActivationDistance)
        m_Enabled = true;
    else
        m_Enabled = false;
    */
    m_Dt = ElapsedTime;
    if (m_Activated)
        m_ActiveTime = m_ActiveTime + 1 * ElapsedTime;
    std::stringstream ss;
    ss << "update_poly_platform" << "(" << m_ActiveTime << ",\"" << m_Name << "\")";
    std::string toRun = ss.str();
    SCRIPTM->RunCode(toRun.c_str());
}