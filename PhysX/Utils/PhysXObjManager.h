
#pragma once


#ifndef _PHYSX_OBJ_MANAGER_H
#define _PHYSX_OBJ_MANAGER_H

#include <string>
#include "Utils\MapManager.h"
#include "PhysXObjDefs.h"

class CPhysXObjManager : public CMapManager<TPhysXObj>
{
private:
	std::string		m_Filename;

public:
	CPhysXObjManager();
	virtual ~CPhysXObjManager();
	
	bool			Load					(const std::string& filename);
	bool			Reload					();
	void			CleanUp					();
};

#endif