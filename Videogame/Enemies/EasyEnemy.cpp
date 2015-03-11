#include "EasyEnemy.h"
#include "XML\XMLTreeNode.h"
#include "Utils\PhysicUserData.h"
#include "Core\Core.h"
#include "PhysicsManager.h"

#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"


CEasyEnemy::CEasyEnemy(CXMLTreeNode &info1) : CEnemy(info1)
{
    Init();
}


CEasyEnemy::~CEasyEnemy()
{
}

void CEasyEnemy::Init()
{
    std::stringstream ss;
    ss << m_Name << "UserData";
    std::string l_UserDataName = ss.str();
    m_PhysicUserData = new CPhysicUserData(l_UserDataName);
    m_PhysicUserData->SetPaint(true);
    m_PhysicUserData->SetColor(colRED);
    m_PhysicController = new CPhysicController(0.5, 0.25, 0.87, 0.1, 0.3, ECG_ESCENE, m_PhysicUserData, m_Position, -9.8f);
    PHYSXM->AddPhysicController(m_PhysicController);
    // SetMesh(m_Mesh);
    CRenderableObject* malla = RENDLM->GetDefaultRenderableObjectManager()->GetResource(m_Mesh);
    malla->SetYaw(m_fYaw);
    Vect3f position = Vect3f(m_PhysicController->GetPosition());
    bool visible = malla->getVisible();
    malla->SetPosition(m_Position);
}

void CEasyEnemy::Update(float elapsedTime)
{
}

void CEasyEnemy::Render()
{
}