#ifndef WP_MANAGER_H
#define WP_MANAGER_H

#include <string>
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
private:
	void Load();
	std::string m_fileName;

};

#endif