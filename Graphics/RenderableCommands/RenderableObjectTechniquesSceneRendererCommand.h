#pragma once
#ifndef RENDERABLE_OBJECT_TECHNIQUES_SCENE_RENDERER_COMMAND_H
#define RENDERABLE_OBJECT_TECHNIQUES_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CGraphicsManager;
class CXMLTreeNode;
class CPoolRenderableObjectTechnique;

class CRenderableObjectTechniquesSceneRendererCommand : public CSceneRendererCommand
{
private:
    CPoolRenderableObjectTechnique *m_PoolRenderableObjectTechnique;
	std::string m_PoolName;
public:
    CRenderableObjectTechniquesSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
	void Reload();
};
#endif