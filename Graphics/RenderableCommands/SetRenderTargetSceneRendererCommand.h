#pragma once
#ifndef SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#define SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"

class CSetRenderTargetSceneRendererCommand : public CStagedTexturedRendererCommand
{
public:
	CSetRenderTargetSceneRendererCommand(){}
	CSetRenderTargetSceneRendererCommand(CXMLTreeNode &atts);
	void Execute(CGraphicsManager &RM);
	void UnsetRenderTarget();
};

#endif