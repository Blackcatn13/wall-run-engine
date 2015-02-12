#pragma once
#ifndef RENDERABLE_OBJECT_TECHNIQUE_MANAGER_H
#define RENDERABLE_OBJECT_TECHNIQUE_MANAGER_H

#include <string>
#include "Utils\MapManager.h"
#include "Renderable\RenderableObjectTechnique.h"
#include "Renderable\PoolRenderableObjectTechnique.h"


//class CRenderableObjectTechniqueManager : public CTemplatedMapManager<CRenderableObjectTechnique> {
class CRenderableObjectTechniqueManager : public CMapManager<CRenderableObjectTechnique>
{
private:
    std::string m_FileName;
    CMapManager<CPoolRenderableObjectTechnique> m_PoolRenderableObjectTechniques;
    void InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName);
public :
    CRenderableObjectTechniqueManager();
    virtual ~CRenderableObjectTechniqueManager();
    void Destroy();
    void Load(const std::string &FileName);
    void Reload();
    std::string GetRenderableObjectTechniqueNameByVertexType(unsigned int VertexType);
    CMapManager<CPoolRenderableObjectTechnique> & GetPoolRenderableObjectTechniques();
};

#endif