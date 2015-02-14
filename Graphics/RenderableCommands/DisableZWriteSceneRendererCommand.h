#pragma once
#ifndef DISABLE_ZWRITE_SCENE_RENDERER_COMMAND_H
#define DISABLE_ZWRITE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"

class CDisableZWriteSceneRendererCommand : public CSceneRendererCommand
{
public:
    CDisableZWriteSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};
#endif