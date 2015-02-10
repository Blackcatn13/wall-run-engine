#pragma once
#ifndef SCENE_RENDERER_COMMAND_H
#define SCENE_RENDERER_COMMAND_H

#include <string>
#include "Utils\MapManager.h"

#include "Utils\Named.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"

//class CSceneRendererCommand : public CUABActive, public CNamed {
class CSceneRendererCommand : public CNamed {
	public: CSceneRendererCommand(CXMLTreeNode &atts);
	virtual ~CSceneRendererCommand();
	virtual void Execute(CGraphicsManager &RM) = 0;
};


#endif