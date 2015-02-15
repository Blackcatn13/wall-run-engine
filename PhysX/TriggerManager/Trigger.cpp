#include "TriggerManager\Trigger.h"
#include "PhysicsManager.h"
#include "Core\ScriptManager.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Defines.h"
#include "Core\Core.h"
#include "TriggerManager\TriggerManager.h"

CTrigger::CTrigger(CXMLTreeNode& node, CPhysicUserData* _pUserData)
    : CPhysicActor(_pUserData)
{
    std::string l_sType = node.GetPszProperty("shape", "box");
    int group = node.GetIntProperty("group", 1);
    if (l_sType == "box")
        CreateBoxTrigger(node.GetVect3fProperty("position", v3fZERO), node.GetVect3fProperty("size", v3fZERO));
    else if (l_sType == "sphere")
        CreateSphereTrigger(node.GetVect3fProperty("position", v3fZERO), node.GetFloatProperty("radius", 0.f), group);
    SetTriggerGroup(ECG_TRIGGERS);
    PHYSXM->AddPhysicActor(this);
    PHYSXM->SetTriggerReport(TRIGGM->getTriggerReport());
    //Events & scripts
    std::string triggerEvent = node.GetPszISOProperty("type_event", "enter");
    std::string script = node.GetPszISOProperty("script", "");
    if (triggerEvent == "enter") {
        m_OnEnter = script;
    } else if (triggerEvent == "stay") {
        m_OnStay = script;
    } else if (triggerEvent == "leave") {
        m_OnExit = script;
    }
}

CTrigger::~CTrigger()
{
}

void CTrigger::ExecuteOnEnter()
{
    //Executing in ENTER type
    SCRIPTM->RunFile(m_OnEnter);
}

void CTrigger::ExecuteOnStay()
{
    //Executing in STAY type
    SCRIPTM->RunFile(m_OnStay);
}

void CTrigger::ExecuteOnExit()
{
    //Executing in EXIT type
    SCRIPTM->RunFile(m_OnExit);
}
