#pragma once
#ifndef ENABLE_ZTEST_SCENE_RENDERER_COMMAND_H
#define ENABLE_ZTEST_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CEnableZTestSceneRendererCommand : public CSceneRendererCommand
{
public:
    CEnableZTestSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};

#endif