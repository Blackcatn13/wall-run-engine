#pragma once
#ifndef CHECK_ENABLED_POLY_RENDERER_COMMAND_H
#define CHECK_ENABLED_POLY_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CGraphicsManager;
class CXMLTreeNode;
class CEffect;
class CRenderableObjectsManager;

class CCheckEnabledPolyRenderCommand : public CSceneRendererCommand {
 private:
  CEffect 	*m_Effect;
 public:
  CCheckEnabledPolyRenderCommand(CXMLTreeNode &atts);
  void CheckLayerChange(CRenderableObjectsManager *rolm1, CRenderableObjectsManager *rolm2, bool polyEnabled);
  void Execute(CGraphicsManager &RM);
};
#endif