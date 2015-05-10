#pragma once
#ifndef RENDER_SCENE_SCENE_RENDERER_COMMAND_H
#define RENDER_SCENE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include <string>

class CGraphicsManager;
class CXMLTreeNode;
class CRenderableObjectsManager;

class CRenderSceneSceneRendererCommand : public CSceneRendererCommand {
 private:
  //CRenderableObjectsManager *m_Layer;
  std::string m_Layer;
  bool m_Active;
 public:
  CRenderSceneSceneRendererCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
};

#endif