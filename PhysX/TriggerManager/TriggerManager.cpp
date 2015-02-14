#include "TriggerManager\TriggerManager.h"
#include "XML\XMLTreeNode.h"
#include "Math\Vector3.h"

CTriggerManager::CTriggerManager()
{
	m_TriggerMap = std::map<std::string,CTrigger*>();
}

CTriggerManager::~CTriggerManager()
{

}
void	CTriggerManager::OnEnter		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape)
{
}
void	CTriggerManager::OnLeave		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape)
{
}
void	CTriggerManager::OnStay		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape)
{
}

bool	CTriggerManager::LoadTriggers( std::string FileName)
{
	m_FileName = FileName;
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(FileName.c_str())) {
        printf("ERROR loading the file.");
    } else {
        CXMLTreeNode  m = newFile["triggers"];
        if (m.Exists()) {
            int count = m.GetNumChildren();
            for (int i = 0; i < count; ++i) {
                std::string name = m(i).GetName();
                if (name == "trigger") {
                    std::string triggerName = m(i).GetPszISOProperty("name", "");
					Vect3f triggerPos = m(i).GetVect3fProperty("position", v3fZERO);
                    std::string triggerShapeString = m(i).GetPszISOProperty("shape", "box");
					TRIGGER_SHAPE triggerShape;
					if(triggerShapeString.compare("box")==0)
					{
						triggerShape = BOX;
					}
					else if (triggerShapeString.compare("sphere"))
					{
						triggerShape = SPHERE;
					}
                    Vect3f triggerSize = m(i).GetVect3fProperty("size", v3fZERO);
					std::string triggerEvemtString = m(i).GetPszISOProperty("event", "enter");
					TRIGGER_EVENT_TYPE triggerEvent;
					if(triggerShapeString.compare("enter")==0)
					{
						triggerEvent = ENTER;
					}
					else if (triggerShapeString.compare("stay"))
					{
						triggerEvent = STAY;
					}
					else if (triggerShapeString.compare("leave"))
					{
						triggerEvent = LEAVE;
					}
                }
            }
            return true;
        }
    }
    return false;
}