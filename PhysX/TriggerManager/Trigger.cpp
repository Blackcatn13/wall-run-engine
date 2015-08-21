#include "TriggerManager\Trigger.h"
#include "PhysicsManager.h"
#include "Core\ScriptManager.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Defines.h"
#include "Core\Core.h"
#include "TriggerManager\TriggerManager.h"
#include "Utils\PhysicUserData.h"
#include "Math\Color.h"

CTrigger::CTrigger(CXMLTreeNode &node, CPhysicUserData *_pUserData)
  : CPhysicActor(_pUserData),
    m_IsSwitched(false),
    m_UserData(_pUserData) {
  m_Time = 0;
  m_Update = false;
  std::string l_sType = node.GetPszProperty("shape", "box", false);
  //int group = node.GetIntProperty("group", 1, false);
  if (l_sType == "box")
    CreateBoxTrigger(node.GetVect3fProperty("pos", v3fZERO, false), node.GetVect3fProperty("size", v3fZERO, false));
  else if (l_sType == "sphere")
    CreateSphereTrigger(node.GetVect3fProperty("pos", v3fZERO, false), node.GetFloatProperty("radius", 0.f, false), ECG_TRIGGERS);
  SetTriggerGroup(ECG_TRIGGERS);
  _pUserData->SetColor(CColor(node.GetVect3fProperty("color", v3fONE, false)));
  _pUserData->SetPaint(false);
  PHYSXM->AddPhysicActor(this);
  PHYSXM->SetTriggerReport(TRIGGM->getTriggerReport());
  int l_CollisionGroup = node.GetIntProperty("group", 3, false);
  _pUserData->SetMyCollisionGroup(static_cast<ECollisionGroup>(l_CollisionGroup));
  //Events & scripts
  std::string triggerEvent = node.GetPszISOProperty("type_event", "enter", false);
  std::string triggerEvent2 = node.GetPszISOProperty("type_event2", "", false);
  std::string triggerEvent3 = node.GetPszISOProperty("type_event3", "", false);
  std::string scriptFile = node.GetPszISOProperty("script_file", "", false);
  std::string scriptFunction = node.GetPszISOProperty("script_function", "", false);
  std::string scriptFunction2 = node.GetPszISOProperty("script_function2", "", false);
  std::string scriptFunction3 = node.GetPszISOProperty("script_function3", "", false);
  m_ScriptParam = node.GetPszISOProperty("param1", "", false);
  std::string l_ScriptParam2 = node.GetPszISOProperty("param1_event2", "", false);
  std::string l_ScriptParam3 = node.GetPszISOProperty("param1_event3", "", false);
  std::string l_Param2 = node.GetPszISOProperty("param2", "", false);
  std::string l_Param2_event2 = node.GetPszISOProperty("param2_event2", "", false);
  std::string l_Param2_event3 = node.GetPszISOProperty("param2_event3", "", false);
  m_PlatformName = node.GetPszISOProperty("platform_name", "", false);
  //useElapsedTime = node.GetFloatProperty("uses_elapsed_time", false);
  m_UpdateFunction =  node.GetPszISOProperty("update_function", "", false);
  m_MaxTime =  node.GetIntProperty("max_time", 0, false);
  SetTriggerFunctions(triggerEvent, scriptFile, m_ScriptParam, l_Param2,  scriptFunction);
  if (triggerEvent2 != "")
    SetTriggerFunctions(triggerEvent2, scriptFile, l_ScriptParam2, l_Param2_event2,  scriptFunction2);
  if (triggerEvent3 != "")
    SetTriggerFunctions(triggerEvent3, scriptFile, l_ScriptParam3, l_Param2_event3,  scriptFunction3);
}

void CTrigger::SetTriggerFunctions(std::string triggerEvent, std::string scriptFile, std::string param1, std::string l_Param2, std::string scriptFunction) {
  if (triggerEvent == "enter") {
    if (scriptFile != "")
      m_OnEnter = scriptFile;
    else if (l_Param2 == "") {
      m_OnEnter = SetString(scriptFunction, param1);
    } else {
      m_OnEnter = SetString(scriptFunction, param1, l_Param2);
    }
  } else if (triggerEvent == "stay") {
    if (scriptFile != "")
      m_OnStay = scriptFile;
    else if (l_Param2 == "")
      m_OnStay = SetString(scriptFunction, param1);
    else
      m_OnStay = SetString(scriptFunction, param1, l_Param2);
  } else if (triggerEvent == "leave") {
    if (scriptFile != "")
      m_OnExit = scriptFile;
    else if (l_Param2 == "")
      m_OnExit = SetString(scriptFunction, param1);
    else
      m_OnExit = SetString(scriptFunction, param1, l_Param2);
  }
}

CTrigger::CTrigger(std::string type, Vect3f position, Vect3f size, float radius, Vect3f color, std::string triggerEvent, std::string scriptFunction, std::string param1, std::string param2, CPhysicUserData *_pUserData)
  : CPhysicActor(_pUserData),
    m_UserData(_pUserData),
    m_IsSwitched(false),
    m_ScriptParam(param1),
    //  m_ScriptFile(""),
    m_PlatformName(""),
    m_UpdateFunction(""),
    m_MaxTime(0) {
  m_Time = 0;
  m_Update = false;
  std::string l_sType = type;
  //int group = node.GetIntProperty("group", 1, false);
  if (l_sType == "box")
    CreateBoxTrigger(position, size);
  else if (l_sType == "sphere")
    CreateSphereTrigger(position, radius, ECG_TRIGGERS);
  SetTriggerGroup(ECG_TRIGGERS);
  _pUserData->SetColor(CColor(color));
  _pUserData->SetPaint(false);
  PHYSXM->AddPhysicActor(this);
  PHYSXM->SetTriggerReport(TRIGGM->getTriggerReport());
  //Events & scripts
// std::string triggerEvent = triggerEvent;
  //std::string scriptFile = node.GetPszISOProperty("script_file", "", false);
// std::string l_scriptFunction = scriptFunction;
// m_ScriptParam = param1;
// std::string l_Param2 = node.GetPszISOProperty("param2", "", false);
// m_PlatformName = node.GetPszISOProperty("platform_name", "", false);
  //useElapsedTime = node.GetFloatProperty("uses_elapsed_time", false);
// m_UpdateFunction =  node.GetPszISOProperty("update_function", "", false);
// m_MaxTime =  node.GetIntProperty("max_time", 0, false);
  if (triggerEvent == "enter") {
    /* if (scriptFile != "")
       m_OnEnter = scriptFile;
     else*/ if (param2 == "") {
      m_OnEnter = SetString(scriptFunction, m_ScriptParam);
    } else {
      m_OnEnter = SetString(scriptFunction, m_ScriptParam, param2);
    }
  } else if (triggerEvent == "stay") {
    /* if (scriptFile != "")
       m_OnStay = scriptFile;
     else*/ if (param2 == "")
      m_OnStay = SetString(scriptFunction, m_ScriptParam);
    else
      m_OnStay = SetString(scriptFunction, m_ScriptParam, param2);
  } else if (triggerEvent == "leave") {
    /*if (scriptFile != "")
      m_OnExit = scriptFile;
    else*/ if (param2 == "")
      m_OnExit = SetString(scriptFunction, m_ScriptParam);
    else
      m_OnExit = SetString(scriptFunction, m_ScriptParam, param2);
  }
}

CTrigger::~CTrigger() {
  CHECKED_DELETE(m_UserData);
  PHYSXM->ReleasePhysicActor(this);
}


void CTrigger::ExecuteOnEnter() {
  //Executing in ENTER type
  if (m_OnEnter != "")
    // SCRIPTM->RunFile(m_OnEnter);
    SCRIPTM->RunCode(m_OnEnter);
}

void CTrigger::ExecuteOnStay() {
  //Executing in STAY type
  if (m_OnStay != "")
    // SCRIPTM->RunFile(m_OnStay);
    SCRIPTM->RunCode(m_OnStay);
}

void CTrigger::ExecuteOnExit() {
  //Executing in EXIT type
  if (m_OnExit != "")
    // SCRIPTM->RunFile(m_OnExit);
    SCRIPTM->RunCode(m_OnExit);
}

void CTrigger::Update(float dt) {
  if ( m_UpdateFunction != "") {
    float l_Time = m_Time * dt;
    std::stringstream ss;
    ss << m_UpdateFunction << "(" << dt << "," << l_Time << "," << m_MaxTime << ",\"" << m_PlatformName << "\",\"" << getName() << "\")";
    std::string toRun = ss.str();
    SCRIPTM->RunCode(toRun.c_str());
    ++m_Time;
  }
}

std::string CTrigger::SetString(std::string function, std::string param1, std::string param2) {
  std::stringstream ss;
  ss << function << "(\"" << param1 << "\", \"" << param2 << "\" )";
  return ss.str();
}

std::string CTrigger::SetString(std::string function, std::string param1) {
  std::stringstream ss;
  ss << function << "(\"" << param1  << "\" )";
  return ss.str();
}

void CTrigger::setUpdate(bool update) {
  m_Update = update;
  if (update == true)
    TRIGGM->GetUpdateTriggersVector().push_back(this);
  else {
    for (size_t i = 0; i <  TRIGGM->GetUpdateTriggersVector().size(); ++ i) {
      TRIGGM->GetUpdateTriggersVector().erase(TRIGGM->GetUpdateTriggersVector().begin() + i);
    }
  }
}