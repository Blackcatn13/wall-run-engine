#pragma once
#ifndef RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"

class CRenderDebugSceneSceneRendererCommand : public CSceneRendererCommand {
 private:
  std::string m_Layer;
  bool		m_Active;
  bool		m_PaintCookingMesh;
  std::string m_CookingMeshName;
  bool		m_PaintTriggers;
  bool		m_PaintAllPhysx;
  bool		m_DebugMode;
  bool		m_PaintCharacterControllers;
 public:
  CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
};

#endif