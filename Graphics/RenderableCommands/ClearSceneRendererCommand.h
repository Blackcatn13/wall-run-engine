#pragma once
#ifndef CLEAR_SCENE_RENDERER_COMMAND_H
#define CLEAR_SCENE_RENDERER_COMMAND_H

#include "RenderableCommands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class  CGraphicsManager;

class CClearSceneRendererCommand : public CSceneRendererCommand
{
protected:
    bool m_Color;
    bool m_Depth;
    bool m_Stencil;
public:
    CClearSceneRendererCommand(CXMLTreeNode &atts);
    virtual void Execute(CGraphicsManager &RM);
};
#endif