#pragma once
#ifndef CHECK_ENABLED_POLY_RENDERER_COMMAND_H
#define CHECK_ENABLED_POLY_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CGraphicsManager;
class CXMLTreeNode;

class CEffect;

class CCheckEnabledPolyRenderCommand : public CSceneRendererCommand {
 private:
  CEffect 	*m_Effect;
 public:
  CCheckEnabledPolyRenderCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
};
#endif