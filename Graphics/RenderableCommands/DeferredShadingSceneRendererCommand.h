#pragma once
#ifndef DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H
#define DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "Renderable\PoolRenderableObjectTechnique.h"

class CDeferredShadingSceneRendererCommand : public CStagedTexturedRendererCommand
{
private:
    CRenderableObjectTechnique *m_RenderableObjectTechnique;
    void SetLightsData(CGraphicsManager &RM);
public:
    CDeferredShadingSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
    void Reload();
};
#endif