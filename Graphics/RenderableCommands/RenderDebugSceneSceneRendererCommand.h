#pragma once
#ifndef RENDER_DEBUG_LIGHTS_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_LIGHTS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"

class CRenderDebugLightsSceneSceneRendererCommand : public CSceneRendererCommand
{
private:
public:
	CRenderDebugLightsSceneSceneRendererCommand(CXMLTreeNode &atts);
	void Execute(CGraphicsManager &RM);
};

#endif