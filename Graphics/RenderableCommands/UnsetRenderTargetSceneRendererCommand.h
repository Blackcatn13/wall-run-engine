#pragma once
#ifndef UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H
#define UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "SetRenderTargetSceneRendererCommand.h"

class CUnsetRenderTargetSceneRendererCommand : public CSceneRendererCommand
{
private:
    CSetRenderTargetSceneRendererCommand *m_SetRenderTargetRendererCommand;
public:
    CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand
                                           *SetRenderTargetRendererCommand, CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};

#endif