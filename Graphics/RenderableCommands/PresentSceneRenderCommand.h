#pragma once
#ifndef PRESENT_SCENE_RENDERER_COMMAND_H
#define PRESENT_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CPresentSceneRendererCommand : public CSceneRendererCommand {
 public:
  CPresentSceneRendererCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
};

#endif