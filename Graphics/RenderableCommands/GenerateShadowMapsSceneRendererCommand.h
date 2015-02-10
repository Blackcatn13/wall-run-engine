#pragma once
#ifndef GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H
#define GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"

class CGenerateShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &atts);
	void Execute(CGraphicsManager &RM);
};

#endif