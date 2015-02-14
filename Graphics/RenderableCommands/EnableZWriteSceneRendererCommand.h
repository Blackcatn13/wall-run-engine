#pragma once
#ifndef ENABLE_ZWRITE_SCENE_RENDERER_COMMAND_H
#define ENABLE_ZWRITE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"

class CEnableZWriteSceneRendererCommand : public CSceneRendererCommand
{
public:
    CEnableZWriteSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};

#endif