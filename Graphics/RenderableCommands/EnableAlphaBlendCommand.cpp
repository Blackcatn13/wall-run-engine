#include "RenderableCommands\EnableAlphaBlendCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Core_Utils/MemLeaks.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &atts)
{
    initMaps();
    m_blendop = m_ToBlendOp[atts.GetPszISOProperty("blendOP", "add", false)];
    m_srcblend = m_ToBlendType[atts.GetPszISOProperty("blendSrc", "one", false)];
    m_destblend = m_ToBlendType[atts.GetPszISOProperty("blendDest", "one", false)];
}

void CEnableAlphaBlendSceneRendererCommand::Execute(CGraphicsManager &RM)

{
    RM.EnableAlphaBlend(m_blendop, m_srcblend, m_destblend);
}

void CEnableAlphaBlendSceneRendererCommand::initMaps()
{
    m_ToBlendOp["add"] = add;
    m_ToBlendOp["substract"] = substract;
    m_ToBlendOp["revsubstract"] = revsubstract;
    m_ToBlendOp["min"] = min;
    m_ToBlendOp["max"] = max;
    m_ToBlendType["zero"] = zero;
    m_ToBlendType["one"] = one;
    m_ToBlendType["srccolor"] = srccolor;
    m_ToBlendType["invsrccolor"] = invsrccolor;
    m_ToBlendType["srcalpha"] = srcalpha;
    m_ToBlendType["invsrcalpha"] = invsrcalpha;
    m_ToBlendType["destalpha"] = destalpha;
    m_ToBlendType["invdestalpja"] = invdestalpja;
    m_ToBlendType["destcolor"] = destcolor;
    m_ToBlendType["invdestcolor"] = invdestcolor;
    m_ToBlendType["srcalphasat"] = srcalphasat;
    m_ToBlendType["bothsrcalpha"] = bothsrcalpha;
    m_ToBlendType["bothinvsrcalpha"] = bothinvsrcalpha;
    m_ToBlendType["blendfactor"] = blendfactor;
    m_ToBlendType["invblendfactor"] = invblendfactor;
    m_ToBlendType["srccolor2"] = srccolor2;
    m_ToBlendType["invsrccolor2"] = invsrccolor2;
}

