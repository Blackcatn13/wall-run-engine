#pragma once
#ifndef RENDER_SCENE_SCENE_RENDERER_COMMAND_H
#define RENDER_SCENE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Renderable\RenderableObjectsManager.h"

class CRenderSceneSceneRendererCommand : public CSceneRendererCommand
{
private:
	CRenderableObjectsManager *m_Layer;
public:
	CRenderSceneSceneRendererCommand(CXMLTreeNode &atts);
	void Execute(CGraphicsManager &RM);
};

#endif