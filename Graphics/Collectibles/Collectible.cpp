#include "Collectible.h"
#include "Core\ScriptManager.h"
#include "Core\Core.h"
#include "PhysicsManager.h"
#include <sstream>

CCollectible::CCollectible(std::string coreName, std::string name,std::string layerName,  std::string userDataName, std::string luaCode):
CMeshInstance(name, coreName), 
	m_LayerName(layerName),
	m_LuaCode(luaCode),
	m_UserDataName(userDataName){
}

CCollectible::~CCollectible(){}

void CCollectible::update(float dt)
{
	std::stringstream ss;
	ss << m_LuaCode <<  dt << ")";
	std::string toRun = ss.str();
	SCRIPTM->RunCode(toRun.c_str());
	//SCRIPTM->RunCode(m_LuaCode);
}