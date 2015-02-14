#pragma once
#ifndef RENDER_DEBUG_LIGHTS_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_LIGHTS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Renderable\RenderableObjectsManager.h"

class CRenderDebugLightsSceneRenderCommand : public CSceneRendererCommand
{
private:
    bool m_Active;
public:
    CRenderDebugLightsSceneRenderCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};

#endif