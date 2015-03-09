#include "AI\WPManager.h"
#include "XML\XMLTreeNode.h"

CWPManager::CWPManager()
	: m_fileName ("")
{
}

CWPManager::~CWPManager()
{
}

void CWPManager::Load(std::string file)
{
	m_fileName = file;
	Load();
}

void CWPManager::Reload()
{
	Destroy();
	Load();
}


int CWPManager::FindClosestWaypoint(std::string ZONEName, Vect3f Position)
{
	int l_closestId = 0;
	float l_closestDistance = 500;
	ZONE * l_currentZone = GetResource("ZONEName");
	for (std::vector<WP *>::iterator it = l_currentZone->m_Waypoints.begin(); it != l_currentZone->m_Waypoints.end(); ++it) {
		//(*it)->m_id
		int l_WaypointDistance = GetDistance((*it)->m_pos, Position);
		if (l_WaypointDistance < l_closestDistance)
		{
			l_closestId = (*it)->m_id;
			l_closestDistance = l_WaypointDistance;
		}
	}
	return l_closestId;
}

Vect3f CWPManager::GetWaypointPosition(int id, std::string ZONEName)
{
	ZONE * l_currentZone = GetResource("ZONEName");
	return l_currentZone->m_Waypoints[id]->m_pos;
}

float CWPManager::GetDistance(Vect3f pos1, Vect3f pos2)
{
	float l_distance = pow((pos1.x + pos2.x),2) + pow((pos1.y + pos2.y),2) + pow((pos1.z + pos2.z),2);
	return l_distance;
}

void CWPManager::Load()
{
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(m_fileName.c_str())) {
        printf("ERROR loading the file.");
    } else {
		CXMLTreeNode  m = newFile["wp"];
		if (m.Exists()) {
			int count = m.GetNumChildren();
			for (int i = 0; i < count; ++i) {

				ZONE* newZONE = new ZONE();
				int count2 = m(i).GetNumChildren();
				std::string ZONEName = m(i).GetPszISOProperty("name", "");
				for (int j = 0; j < count2; ++j) {

					std::string name = m(i)(j).GetName();
					if (name == "waypoint") {
						WP* newWP = new WP();
						newWP->m_id = m(i)(j).GetIntProperty("id", 0);
						newWP->m_pos = m(i)(j).GetVect3fProperty("pos",Vect3f(0,0,0));
						int count3 = m(i)(j).GetNumChildren();
						for (int k = 0; k < count3; ++k) {
							Link* newLink = new Link();
							newLink->id = m(i)(j)(k).GetIntProperty("id", 0);
							newLink->weight = m(i)(j)(k).GetIntProperty("weight", 0);
							newWP->m_LinkList.push_back(newLink);
						}
						newZONE->m_Waypoints.push_back(newWP);
					}
				}
				AddResource(ZONEName, newZONE);
			}
		}
    }
}