#pragma once
#ifndef UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#define UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CSetRenderTargetSceneRendererCommand;
class CGraphicsManager;
class CXMLTreeNode;

class CUnsetRenderTargetSceneRendererCommand : public CSceneRendererCommand {
 private:
  CSetRenderTargetSceneRendererCommand *m_SetRenderTargetRendererCommand;
  int m_depthStencil;
 public:
  CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand
                                         *SetRenderTargetRendererCommand, CXMLTreeNode &atts);

  void Execute(CGraphicsManager &RM);
};

#endif