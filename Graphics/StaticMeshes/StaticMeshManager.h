#pragma once
#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H

#include "Utils\MapManager.h"
#include <string>
#include "StaticMesh.h"

class CStaticMeshManager : public CMapManager<CStaticMesh>
{
protected:
    std::string m_FileName;
public:
    CStaticMeshManager();
    ~ CStaticMeshManager ();
    bool Load(const std::string &FileName);
    bool Reload();
};

#endif