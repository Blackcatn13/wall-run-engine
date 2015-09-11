#pragma once
#ifndef BILLBOARD_RENDERER_COMMAND_H_
#define BILLBOARD_RENDERER_COMMAND_H_

#include "RenderableCommands\SceneRendererCommand.h"
#include <string>

class CXMLTreeNode;
class CGraphicsManager;

class CBillboardRendererCommand : public CSceneRendererCommand {
 public:
  CBillboardRendererCommand(CXMLTreeNode &atts);
  ~CBillboardRendererCommand();
  void Execute(CGraphicsManager &RM);
 private:
  std::string name;
};

#endif