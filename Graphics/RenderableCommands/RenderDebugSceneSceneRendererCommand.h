#pragma once
#ifndef RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"

class CRenderDebugSceneSceneRendererCommand : public CSceneRendererCommand
{
private:
public:
	CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts);
	void Execute(CGraphicsManager &RM);
};

#endif