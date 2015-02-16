#pragma once
#ifndef DISABLE_ZTEST_SCENE_RENDERER_COMMAND_H
#define DISABLE_ZTEST_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CGraphicsManager;
class CXMLTreeNode;

class CDisableZTestSceneRendererCommand : public CSceneRendererCommand
{
public:
    CDisableZTestSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};
#endif