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
	float l_closestDistance = 500.0;
	ZONE * l_currentZone = GetResource("ZONEName");
	for (std::vector<WP *>::iterator it = l_currentZone->m_Waypoints.begin(); it != l_currentZone->m_Waypoints.end(); ++it) {
		//(*it)->m_id
		float l_WaypointDistance = GetDistance((*it)->m_pos, Position);
		if (l_WaypointDistance < l_closestDistance)
		{
			l_closestId = (*it)->m_id;
			l_closestDistance = l_WaypointDistance;
		}
	}
	return l_closestId;
}

int CWPManager::FindOptimalPath(std::string ZONEName, Vect3f PositionStart, Vect3f PositionDestiny)
{
	int l_idWaypointStart = FindClosestWaypoint(ZONEName, PositionStart);
	int l_idWaypointDestiny = FindClosestWaypoint(ZONEName, PositionDestiny);
	std::string CurrentPath = "";
	std::string l_nextWaypoint = FindNExtWaypointOptimal(ZONEName, l_idWaypointStart, l_idWaypointDestiny, CurrentPath);
	int l_intNextWaypoint = atoi(l_nextWaypoint.substr(0,1).c_str()); //cogemos el primer waypoint del path óptimo (solo funciona de 0 a 9 waypoints)
	return l_intNextWaypoint;
	
}
std::string CWPManager::FindNExtWaypointOptimal(std::string ZONEName, int WaypointStart, int WaypointDestiny, std::string CurrentPath)
{

	ZONE * l_currentZone = GetResource(ZONEName);
	for (std::vector<Link *>::iterator it = l_currentZone->m_Waypoints[WaypointStart]->m_LinkList.begin(); it != l_currentZone->m_Waypoints[WaypointStart]->m_LinkList.end(); ++it) 
	{
		std::string l_ThisWaypointToString = boost::lexical_cast<std::string>(WaypointStart);
		std::string l_CurrentPath = CurrentPath + l_ThisWaypointToString;
		(*it)->id;
	}
	return "";
}

int CWPManager::TestFunction(int parametro1)
{
	//inputs de Test
	ZONE * l_currentZone = GetResource("sala_principal");
	int id_origen = 1;
	int id_destino = 4;
	Vect3f posicionDestino = Vect3f(7.000, 0.000, 7.000);

	//creamos vector de nodos abiertos
	std::vector<WP *> l_openVector;

	//añadimos a vectores abiertos el nodo por defecto
	l_openVector.push_back(l_currentZone->m_Waypoints[id_origen]);

	//calculamos G H y F para nodo abierto inicial
	float temp_H = CalcularH(0, id_origen, id_destino, l_currentZone);
	float temp_G = 0;
	float temp_F = temp_H + temp_G;

	//calculamos todos los vecinos de nodo inicial
	for (std::vector<Link *>::iterator it = l_currentZone->m_Waypoints[id_origen]->m_LinkList.begin(); it != l_currentZone->m_Waypoints[id_origen]->m_LinkList.end(); ++it) 
	{
		l_openVector.push_back(l_currentZone->m_Waypoints[(*it)->id]);
	}
	//cerramos el vector actual
	l_currentZone->m_Waypoints[id_origen]->m_cerrado = true;

	return 0;
}

float CWPManager::CalcularH(int pesoEntrada, int id_origen, int id_destino, ZONE * currentZone)
{
	float l_H = pesoEntrada + GetDistance(currentZone->m_Waypoints[id_origen]->m_pos, currentZone->m_Waypoints[id_destino]->m_pos);
	l_H = sqrt(l_H);
	return l_H;
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
						newWP->m_cerrado = false;
						newWP->m_F = 999.0;
						newWP->m_G = 999.0;
						newWP->m_H = 999.0;
						newWP->m_Padre = NULL;
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

void CWPManager::ResetWPStruct(std::string ZONEName)
{
	ZONE * l_currentZone = GetResource(ZONEName);
	for (std::vector<WP *>::iterator it = l_currentZone->m_Waypoints.begin(); it != l_currentZone->m_Waypoints.end(); ++it) 
	{
		(*it)->m_cerrado = false;
		(*it)->m_F = 999.0;
		(*it)->m_G = 999.0;
		(*it)->m_H = 999.0;
		(*it)->m_Padre = NULL;
	}

}




