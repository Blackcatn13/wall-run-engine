#pragma once
#ifndef GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H
#define GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
//#include "GraphicsManager.h"
//#include "XML\XMLTreeNode.h"

class CGraphicsManager;
class CXMLTreeNode;

class CGenerateCascadeShadowMapsSceneRendererCommand : public CSceneRendererCommand {
 protected:
  float m_orthoScale;
  float m_textureScale;
  int m_loops;
 public:
  CGenerateCascadeShadowMapsSceneRendererCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
};

#endif