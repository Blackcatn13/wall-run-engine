#pragma once
#ifndef SET_MATRICES_SCENE_RENDERER_COMMAND_H
#define SET_MATRICES_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"

class CSetMatricesSceneRendererCommand : public CSceneRendererCommand {
protected:
	bool m_Color;
	bool m_Depth;
	bool m_Stencil;
public:
	CSetMatricesSceneRendererCommand(CXMLTreeNode &atts);
	virtual void Execute(CGraphicsManager &RM);
};
#endif