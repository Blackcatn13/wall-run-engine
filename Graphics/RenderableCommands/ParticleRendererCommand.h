#pragma once
#ifndef PARTICLE_RENDERER_COMMAND_H_
#define PARTICLE_RENDERER_COMMAND_H_

#include "RenderableCommands\SceneRendererCommand.h"

class CXMLTreeNode;
class CGraphicsManager;

class CParticleRendererCommand : public CSceneRendererCommand {
 public:
  CParticleRendererCommand(CXMLTreeNode &atts);
  ~CParticleRendererCommand();
  void Execute(CGraphicsManager &RM);
};

#endif