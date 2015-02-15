#ifndef TRIGGER_H
#define TRIGGER_H

#include "Reports\PhysicTriggerReport.h"
#include "Actor\PhysicActor.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include <map>
#include "Math\Vector3.h"

class CPhysicUserData;

class CTrigger: public CPhysicActor
{
public:
	CTrigger(CXMLTreeNode& node, CPhysicUserData* _pUserData);
	~CTrigger(void);

	void ExecuteOnEnter	();
	void ExecuteOnStay	();
	void ExecuteOnExit	();
private:
    std::string		m_OnEnter;
	std::string		m_OnStay;
	std::string		m_OnExit;
};

#endif