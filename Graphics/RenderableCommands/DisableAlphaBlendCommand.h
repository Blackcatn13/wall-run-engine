#pragma once
#ifndef DISABLE_ALPHABLEND_SCENE_RENDERER_COMMAND_H
#define DISABLE_ALPHABLEND_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CGraphicsManager;
class CXMLTreeNode;

class CDisableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{
public:
    CDisableAlphaBlendSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};
#endif