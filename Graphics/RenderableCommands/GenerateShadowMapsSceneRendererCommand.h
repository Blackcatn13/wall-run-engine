#pragma once
#ifndef GENERATE_CASCADE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H
#define GENERATE_CASCADE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
//#include "GraphicsManager.h"
//#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CXMLTreeNode;

class CGenerateShadowMapsSceneRendererCommand : public CSceneRendererCommand {
 public:
  CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
};

#endif