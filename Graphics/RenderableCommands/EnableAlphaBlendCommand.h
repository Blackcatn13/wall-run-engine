#pragma once
#ifndef ENABLE_ALPHABLEND_SCENE_RENDERER_COMMAND_H
#define ENABLE_ALPHABLEND_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CEnableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{
public:
    CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};

#endif