#pragma once
#ifndef SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#define SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"

class CXMLTreeNode;
class CGraphicsManager;

class CSetRenderTargetSceneRendererCommand : public CStagedTexturedRendererCommand {
 public:
  CSetRenderTargetSceneRendererCommand() {}
  ~CSetRenderTargetSceneRendererCommand();
  CSetRenderTargetSceneRendererCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
  void UnsetRenderTarget(int depthStencil);
};

#endif