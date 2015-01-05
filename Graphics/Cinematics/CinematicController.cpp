#include "CinematicController.h"
#include "Cinematic.h"


CCinematicController::CCinematicController()
    : m_FileName("")
{
}

CCinematicController::~CCinematicController()
{
}

bool CCinematicController::Load(const std::string &FileName)
{
    m_FileName = FileName;
    CCinematic *l_Cinematic = new CCinematic();
    AddResource("a", l_Cinematic);
    l_Cinematic->LoadXML(FileName);
    return true;
}

bool CCinematicController::Reload()
{
    Destroy();
    return Load(m_FileName);
}

bool CCinematicController::Reload(const std::string &FileName)
{
    Destroy();
    return Load(FileName);
}

void CCinematicController::Update(float ElapsedTime)
{
    TMapResource::iterator it = m_Resources.begin();
    for (it; it != m_Resources.end(); ++it) {
        (* it).second->Update(ElapsedTime);
    }
}