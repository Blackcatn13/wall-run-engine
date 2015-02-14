#pragma once
#ifndef SET_AXIS_RENDERER_COMMAND_H
#define SET_AXIS_RENDERER_COMMAND_H

#include "RenderableCommands\SceneRendererCommand.h"

class CXMLTreeNode;

class CGraphicsManager;

class CSetAxisRendererCommand : public CSceneRendererCommand
{
public:
    CSetAxisRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};

#endif