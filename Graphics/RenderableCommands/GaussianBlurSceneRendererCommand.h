#pragma once
#ifndef GAUSSIAN_BLUR_SCENE_RENDERER_COMMAND_H
#define GAUSSIAN_BLUR_SCENE_RENDERER_COMMAND_H


#include "XML\XMLTreeNode.h"
#include "StagedTexturedRendererCommand.h"
#include <string>

class CEffectTechnique;
class CTexture;
class CGraphicsManager;

class CGaussianBlueSceneRendererCommand : public CStagedTexturedRendererCommand
{
protected:
	int m_GaussianLoops;
	CColor m_Color;
	std::string m_TechniqueName;
public:
    CGaussianBlueSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};


#endif