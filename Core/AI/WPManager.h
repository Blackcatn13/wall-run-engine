#ifndef WP_MANAGER_H
#define WP_MANAGER_H

#include <sstream>
#include <string>
#include "boost/lexical_cast.hpp"
#include "Utils\MapManager.h"
#include "Math/Vector3.h"
typedef struct Link
{
	int id;
	int weight;
} Link;

typedef struct Waypoint
{
	int					m_id;
	Vect3f				m_pos;
	std::vector<Link *> m_LinkList;
} WP;

typedef struct Zone
{
	std::vector<WP *>	m_Waypoints;
	std::string			m_previousState;
	std::string			m_currentState;
} ZONE;

class CWPManager : public CMapManager<ZONE>
{
public:
    CWPManager();
    ~CWPManager();
	void Load(std::string file);
	void Reload();
	int FindClosestWaypoint(std::string ZONEName, Vect3f Position);
	float GetDistance(Vect3f pos1, Vect3f pos2);
	Vect3f GetWaypointPosition(int id, std::string ZONEName);
	int FindOptimalPath(std::string ZONEName, Vect3f PositionStart, Vect3f PositionDestiny);
	std::string FindNExtWaypointOptimal(std::string ZONEName, int WaypointStart, int WaypointDestiny, std::string CurrentPath);
private:
	void Load();
	std::string m_fileName;

};

#endif