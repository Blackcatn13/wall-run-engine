#pragma once
#ifndef BEGIN_RENDER_SCENE_RENDERER_COMMAND_H
#define BEGIN_RENDER_SCENE_RENDERER_COMMAND_H

#include "RenderableCommands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CBeginRenderSceneRendererCommand : public CSceneRendererCommand
{
public:
    CBeginRenderSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};

#endif