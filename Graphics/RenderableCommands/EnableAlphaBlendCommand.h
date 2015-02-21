#pragma once
#ifndef ENABLE_ALPHABLEND_SCENE_RENDERER_COMMAND_H
#define ENABLE_ALPHABLEND_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include <map>
#include <string>

enum BlendType {zero = 1, one, srccolor, invsrccolor, srcalpha, invsrcalpha, destalpha, invdestalpja, destcolor, invdestcolor, srcalphasat, bothsrcalpha, bothinvsrcalpha, blendfactor, invblendfactor, srccolor2, invsrccolor2};
enum BlendOP {add = 1, substract, revsubstract, min, max};

class CEnableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{
public:
    CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
private:
    BlendOP     m_blendop;
    BlendType   m_srcblend;
    BlendType   m_destblend;
    std::map<std::string, BlendType> m_ToBlendType;
    std::map<std::string, BlendOP> m_ToBlendOp;

    void initMaps();
};

#endif