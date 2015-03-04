#include "AI\FSMManager.h"
#include "XML\XMLTreeNode.h"

CFSMManager::CFSMManager()
	: m_fileName (NULL)
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
							s->onEnter = m(i)(j).GetPszISOProperty("name", "");
						} else if (StateType == "onExit") {
							s->onExit = m(i)(j).GetPszISOProperty("name", "");
						} else if (StateType == "Update") {
							s->onUpdate = m(i)(j).GetPszISOProperty("name", "");
							s->m_UpdateTime = m(i)(j).GetFloatProperty("time");
						}
					}
					newFSM->m_States.AddResource(StateName, s);
				}
			}
			AddResource(FSMName, newFSM);
		}
    }
}