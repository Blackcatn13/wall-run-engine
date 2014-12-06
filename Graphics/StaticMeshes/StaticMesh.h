#pragma once
#ifndef _STATIC_MESH_H
#define _STATIC_MESH_H

#include "Utils\Defines.h"
#include <string>
#include <vector>

class CTexture;
class CRenderableVertexs;
class CGraphicsManager;

class CStaticMesh
{
protected:
    std::vector<CRenderableVertexs*> m_RVs;
    std::vector<std::vector<CTexture *>> m_Textures;
    std::string m_FileName;
    unsigned int m_NumVertexs, m_NumFaces;
public:
    CStaticMesh();
    ~CStaticMesh();
    bool Load (const std::string &FileName);
    bool ReLoad ();
    void Render (CGraphicsManager *RM);
};

#endif