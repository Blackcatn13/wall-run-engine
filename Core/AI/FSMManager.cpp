#include "AI\FSMManager.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Core\ScriptManager.h"
#include "Utils\LuaGlobals.h"

CFSMManager::CFSMManager()
    : m_fileName ("")
{
}

CFSMManager::~CFSMManager()
{
}

void CFSMManager::Load(std::string file)
{
    m_fileName = file;
    Load();
}

void CFSMManager::Reload()
{
    Destroy();
    Load();
}

void CFSMManager::Load()
{
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(m_fileName.c_str())) {
        printf("ERROR loading the file.");
    } else {
        CXMLTreeNode  m = newFile["fsm"];
        if (m.Exists()) {
            FSM* newFSM = new FSM();
            int count = m.GetNumChildren();
            std::string FSMName = m.GetPszISOProperty("name", "");
            for (int i = 0; i < count; ++i) {
                std::string name = m(i).GetName();
                if (name == "initialState") {
                    newFSM->m_currentState = m(i).GetPszISOProperty("name", "");
                } else if (name == "state") {
                    STATE* s = new STATE();
                    int states = m(i).GetNumChildren();
                    std::string StateName = m(i).GetPszISOProperty("name", "");
                    for (int j = 0; j < states; ++j) {
                        std::string StateType = m(i)(j).GetName();
                        if (StateType == "onEnter") {
                            s->onEnter = m(i)(j).GetPszISOProperty("lua_funtion", "");
                        } else if (StateType == "onExit") {
                            s->onExit = m(i)(j).GetPszISOProperty("lua_funtion", "");
                        } else if (StateType == "Update") {
                            s->onUpdate = m(i)(j).GetPszISOProperty("lua_funtion", "");
                            s->m_UpdateTime = m(i)(j).GetFloatProperty("time");
                        }
                        s->m_ElapsedTime = 0;
                        s->m_onEnter = false;
                    }
                    newFSM->m_States.AddResource(StateName, s);
                }
            }
            AddResource(FSMName, newFSM);
        }
    }
}

void CFSMManager::Update(float dt)
{
    for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
        STATE *s = it->second->m_States.GetResource(it->second->m_currentState);
        if (s->m_onEnter == false) {
            SCRIPTM->RunCode(s->onEnter.c_str());
			s->m_onEnter = true;
		}
        s->m_ElapsedTime += dt;
        if (s->m_ElapsedTime >= s->m_UpdateTime) {
			char l_InitLevelText[256];
			_snprintf_s(l_InitLevelText, 256, 256, "%s(%f)", s->onUpdate.c_str(), dt);
            SCRIPTM->RunCode(l_InitLevelText);
			s->m_ElapsedTime = 0;
		}
        bool change = CLuaGlobals::getInstance()->ValueChanged();
        if (change) {
            SCRIPTM->RunCode(s->onExit.c_str());
            it->second->m_previousState = it->second->m_currentState;
            it->second->m_currentState = CLuaGlobals::getInstance()->getString();
        }
    }
}