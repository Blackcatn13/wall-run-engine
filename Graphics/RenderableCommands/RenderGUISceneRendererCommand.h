#pragma once
#ifndef RENDER_GUI_SCENE_RENDERER_COMMAND_H
#define RENDER_GUI_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"

class CRenderGUISceneRendererCommand : public CSceneRendererCommand
{
private:
    //CGUI *m_GUI;
public:
    CRenderGUISceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};
#endif