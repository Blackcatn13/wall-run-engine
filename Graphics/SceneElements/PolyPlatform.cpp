#include "PolyPlatform.h"
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Core\PlayerController.h"
#include "Core\Core.h"
#include "Utils\PhysicUserData.h"
#include "Core\ScriptManager.h"
#include "Utils\Logger.h"


CPolyPlatform::CPolyPlatform(std::string platformName, std::string coreName, std::string redimAxis, float redimScale, bool initialCollision,  float activationDistance)
    : CStaticPlatform(platformName, coreName),
      m_RedimScale(redimScale),
      m_OriginalScale(GetScale()),
      m_RedimAxis(redimAxis),
      m_Enabled(false),
      m_Collission(initialCollision),
      m_ActivationDistance(activationDistance),
      m_ActiveTime(.0f),
      m_Activated(false)
{
}

CPolyPlatform::~CPolyPlatform ()
{
    //PHYSXM->ReleasePhysicActor(m_PlatorformActor);
}


void CPolyPlatform:: ActivatePoly(Vect3f direction)
{
    if (!m_Activated) {
        float l_NewScale = 0.0f;
        m_Activated = true;
        if (m_Collission) {
            l_NewScale = 0;
            m_PlatorformActor->Activate(false);
            m_Collission = false;
            if (m_RedimAxis == "y")
                CCORE->GetPlayerController()->getPhysicController()->Move(direction, m_Dt);
        } else {
            l_NewScale = m_RedimScale;
            m_PlatorformActor->Activate(true);
            m_Collission = true;
        }
        if (m_RedimAxis == "x") {
            SetScale(Vect3f(l_NewScale, GetScale().y, GetScale().z));
        } else if (m_RedimAxis == "y") {
            SetScale(Vect3f(GetScale().x, l_NewScale , GetScale().z));
        } else if (m_RedimAxis == "z") {
            SetScale(Vect3f(GetScale().x, GetScale().y , l_NewScale));
        } else {
            std::string msg_error = "No redimension Axis defined";
            LOGGER->AddNewLog(ELL_INFORMATION, msg_error.c_str());
        }
    }
}


void CPolyPlatform:: DeactivatePoly(Vect3f direction)
{
    if (m_Activated) {
        m_Activated = false;
        m_ActiveTime = 0.0f;
        m_PlatorformActor->Activate(!m_Collission);
        if (m_Collission) {
            m_Collission = false;
            // if (m_RedimAxis == "y")
            CCORE->GetPlayerController()->getPhysicController()->Move(direction, m_Dt);
        } else {
            m_Collission = true;
            //Hacer que el player suba si es necesario
        }
        SetScale(m_OriginalScale);
        // m_PlatorformActor->Activate(m_Collission);
    }
    /*if (m_Activated) {
        m_ActiveTime = 0.0f;
        m_PlatorformActor->Activate(false);
        SetScale(m_OriginalScale);
        if (m_Collission) {
            m_Activated = false;
            if (m_RedimAxis == "y")
                CCORE->GetPlayerController()->getPhysicController()->Move(Vect3f(.0f, -1.f, .0f), m_Dt);
        }
    } else
        m_PlatorformActor->Activate(true);*/
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
    ss << "update_poly_platform" << "(" << m_ActiveTime << "," << ElapsedTime << ",\"" << m_Name << "\")";
    std::string toRun = ss.str();
    SCRIPTM->RunCode(toRun.c_str());
}