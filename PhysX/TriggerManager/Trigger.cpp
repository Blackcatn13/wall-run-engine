#include "TriggerManager\Trigger.h"
#include "PhysicsManager.h"
#include "Core\ScriptManager.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Defines.h"
#include "Core\Core.h"
#include "TriggerManager\TriggerManager.h"
#include "Utils\PhysicUserData.h"
#include "Math\Color.h"

CTrigger::CTrigger(CXMLTreeNode& node, CPhysicUserData* _pUserData)
    : CPhysicActor(_pUserData)
{
    m_Time = 0;
    m_Update = false;
    std::string l_sType = node.GetPszProperty("shape", "box", false);
    //int group = node.GetIntProperty("group", 1, false);
    if (l_sType == "box")
        CreateBoxTrigger(node.GetVect3fProperty("position", v3fZERO, false), node.GetVect3fProperty("size", v3fZERO, false));
    else if (l_sType == "sphere")
        CreateSphereTrigger(node.GetVect3fProperty("position", v3fZERO, false), node.GetFloatProperty("radius", 0.f, false), ECG_TRIGGERS);
    SetTriggerGroup(ECG_TRIGGERS);
    _pUserData->SetColor(CColor(node.GetVect3fProperty("color", v3fONE, false)));
    _pUserData->SetPaint(false);
    PHYSXM->AddPhysicActor(this);
    PHYSXM->SetTriggerReport(TRIGGM->getTriggerReport());
    //Events & scripts
    std::string triggerEvent = node.GetPszISOProperty("type_event", "enter", false);
    std::string scriptFile = node.GetPszISOProperty("script_file", "", false);
    std::string scriptFunction = node.GetPszISOProperty("script_function", "", false);
    m_ScriptParam = node.GetPszISOProperty("param1", "", false);
    m_PlatformName = node.GetPszISOProperty("platform_name", "", false);
    //useElapsedTime = node.GetFloatProperty("uses_elapsed_time", false);
    m_UpdateFunction =  node.GetPszISOProperty("update_function", "", false);
    m_MaxTime =  node.GetIntProperty("max_time", 0, false);
    if (triggerEvent == "enter") {
        if (scriptFile != "")
            m_OnEnter = scriptFile;
        else {
            m_OnEnter = SetString(scriptFunction, m_ScriptParam);
        }
    } else if (triggerEvent == "stay") {
        if (scriptFile != "")
            m_OnStay = scriptFile;
        else
            m_OnStay = SetString(scriptFunction, m_ScriptParam);
    } else if (triggerEvent == "leave") {
        if (scriptFile != "")
            m_OnExit = scriptFile;
        else
            m_OnExit = SetString(scriptFunction, m_ScriptParam);
    }
}

CTrigger::~CTrigger()
{
}


void CTrigger::ExecuteOnEnter()
{
    //Executing in ENTER type
    if (m_OnEnter != "")
        // SCRIPTM->RunFile(m_OnEnter);
        SCRIPTM->RunCode(m_OnEnter);
}

void CTrigger::ExecuteOnStay()
{
    //Executing in STAY type
    if (m_OnStay != "")
        // SCRIPTM->RunFile(m_OnStay);
        SCRIPTM->RunCode(m_OnStay);
}

void CTrigger::ExecuteOnExit()
{
    //Executing in EXIT type
    if (m_OnStay != "")
        // SCRIPTM->RunFile(m_OnExit);
        SCRIPTM->RunCode(m_OnExit);
}

void CTrigger::Update(float dt)
{
    if ( m_UpdateFunction != "") {
        float l_Time = m_Time * dt;
        std::stringstream ss;
        ss << m_UpdateFunction << "(" << dt << "," << l_Time << "," << m_MaxTime << ",\"" << m_PlatformName << "\")";
        std::string toRun = ss.str();
        SCRIPTM->RunCode(toRun.c_str());
        ++m_Time;
    }
}

std::string CTrigger::SetString(std::string function, std::string param1, std::string param2)
{
    std::stringstream ss;
    ss << function << "(" << param1 << "," << param2 << " )";
    return ss.str();
}

std::string CTrigger::SetString(std::string function, std::string param1)
{
    std::stringstream ss;
    ss << function << "(\"" << param1  << "\" )";
    return ss.str();
}