#pragma once
#ifndef POOL_RENDERABLE_OBJECT_TECHNIQUE_H
#define POOL_RENDERABLE_OBJECT_TECHNIQUE_H

#include <string>
#include "Utils\Named.h"
#include <vector>
#include "XML\XMLTreeNode.h"
#include "Effects\EffectTechnique.h"
#include "Renderable\RenderableObjectTechnique.h"
#include "Utils\Defines.h"

class CPoolRenderableObjectTechnique : public CNamed
{
private:
    class CPoolRenderableObjectTechniqueElement
    {
    public:
        CRenderableObjectTechnique m_RenderableObjectTechnique;
        CRenderableObjectTechnique *m_OnRenderableObjectTechniqueManager;
        CPoolRenderableObjectTechniqueElement(const std::string &Name, CEffectTechnique *EffectTechnique, CRenderableObjectTechnique *OnRenderableObjectTechniqueManager);
    };
    std::vector<CPoolRenderableObjectTechniqueElement *> m_RenderableObjectTechniqueElements;
public:
    CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode);
    virtual ~CPoolRenderableObjectTechnique();
    void Destroy();
    void AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager);
    void Apply();
};

#endif