#include "PhysicMaterial\PhysicMaterial.h"
#include "NxMaterialDesc.h"
#include "Utils\Defines.h"
#include "NxMaterial.h"
#include "Core\Core.h"
#include "PhysicsManager.h"

CPhysicMaterial::CPhysicMaterial(void)
{
}


CPhysicMaterial::~CPhysicMaterial(void)
{
}

void CPhysicMaterial::NewMaterial(float rest, float stfric, float dynfric)
{
    m_Desc = new NxMaterialDesc();
    m_Desc->restitution = rest;
    m_Desc->dynamicFriction = dynfric;
    m_Desc->staticFriction = stfric;
}

int CPhysicMaterial::getMaterialID()
{
    return m_Mat->getMaterialIndex();
}