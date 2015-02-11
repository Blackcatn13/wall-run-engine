#pragma once
#ifndef END_RENDER_SCENE_RENDERER_COMMAND_H
#define END_RENDER_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManagers;

class CEndRenderSceneRendererCommand : public CSceneRendererCommand
{
	public: 
		CEndRenderSceneRendererCommand (CXMLTreeNode &atts);
		void Execute(CGraphicsManager &RM);
};

#endif