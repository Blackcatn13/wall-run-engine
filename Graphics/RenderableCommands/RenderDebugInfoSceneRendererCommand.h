#pragma once
#ifndef RENDER_DEBUG_INFO_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_INFO_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"

class CLogRender;

class CRenderDebugInfoSceneRendererCommand : public CSceneRendererCommand {
 protected:
  CLogRender *m_LogRender;
 public:
  CRenderDebugInfoSceneRendererCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
};


#endif