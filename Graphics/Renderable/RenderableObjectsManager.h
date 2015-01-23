#pragma once
#ifndef RENDERABLE_OBJECT_MANAGER_H
#define RENDERABLE_OBJECT_MANAGER_H

#include <string>
#include "Utils\TemplatedVectorMapManager.h"
#include "Renderable\RenderableObject.h"
#include "Math\Vector3.h"

class CXMLTreeNode;
class CGraphicsManager;

class CRenderableObjectsManager : public CTemplatedVectorMapManager<CRenderableObject>
{
private:
    std::string m_FileName, m_FileName2;
public:
    CRenderableObjectsManager();
    virtual ~CRenderableObjectsManager();
    void Update(float ElapsedTime);
    void Render(CGraphicsManager *RM);
    CRenderableObject* AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position);
    CRenderableObject* AddMeshInstance(CXMLTreeNode &Node);
    CRenderableObject* AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position);
    CRenderableObject* AddAnimatedInstanceModel(CXMLTreeNode &Node);
    void Load(const std::string &FileName);
    void Reload();
};

#endif