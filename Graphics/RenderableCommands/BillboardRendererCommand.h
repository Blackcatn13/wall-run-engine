#pragma once
#ifndef BILLBOARD_RENDERER_COMMAND_H_
#define BILLBOARD_RENDERER_COMMAND_H_

#include "RenderableCommands\SceneRendererCommand.h"

class CXMLTreeNode;
class CGraphicsManager;

class CBillboardRendererCommand : public CSceneRendererCommand {
 public:
  CBillboardRendererCommand(CXMLTreeNode &atts);
  ~CBillboardRendererCommand();
  void Execute(CGraphicsManager &RM);
};

#endif