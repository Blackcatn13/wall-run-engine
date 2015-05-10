#pragma once
#ifndef RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H
#define RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"

class CRenderDebugShadowMapsSceneRendererCommand : public CSceneRendererCommand {
 protected:
  float m_ScreenWidth, m_ScreenHeight;
  void AdvanceTexturePosition(Vect2f &Position);
 public:
  CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
};
//pagina 26 métodos Render en CPROC y métodos CTexture

#endif