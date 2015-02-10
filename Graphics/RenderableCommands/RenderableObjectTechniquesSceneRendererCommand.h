#pragma once
#ifndef RENDERABLE_OBJECT_TECHNIQUES_SCENE_RENDERER_COMMAND_H
#define RENDERABLE_OBJECT_TECHNIQUES_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Renderable\PoolRenderableObjectTechnique.h"

class CRenderableObjectTechniquesSceneRendererCommand : public CSceneRendererCommand
{
private:
	CPoolRenderableObjectTechnique *m_PoolRenderableObjectTechnique;
public:
	CRenderableObjectTechniquesSceneRendererCommand(CXMLTreeNode &atts);
	void Execute(CGraphicsManager &RM);
};
#endif