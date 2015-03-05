#include "AI\WPManager.h"
#include "XML\XMLTreeNode.h"

CWPManager::CWPManager()
	: m_fileName (NULL)
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

void CWPManager::Load()
{
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(m_fileName.c_str())) {
        printf("ERROR loading the file.");
    } else {
		CXMLTreeNode  m = newFile["Waypoints"];
		if (m.Exists()) {
			int count = m.GetNumChildren();
			for (int i = 0; i < count; ++i) {

				ZONE* newZONE = new ZONE();
				int count2 = m.GetNumChildren();
				std::string ZONEName = m.GetPszISOProperty("name", "");
				for (int j = 0; j < count2; ++j) {

					std::string name = m(i).GetName();
					if (name == "waypoint") {
						WP* newWP = new WP();
						newWP->m_id = m(i)(j).GetIntProperty("id", 0);
						newWP->m_pos = m(i)(j).GetVect3fProperty("pos",Vect3f(0,0,0));
						int count3 = m.GetNumChildren();
						for (int k = 0; k < count2; ++k) {
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