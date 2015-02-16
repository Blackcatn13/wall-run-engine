#include "RenderableCommands\SetRenderTargetSceneRendererCommand.h"

#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "Texture\Texture.h"


CSetRenderTargetSceneRendererCommand::CSetRenderTargetSceneRendererCommand(CXMLTreeNode &atts)
{
    for (int i = 0; i < atts.GetNumChildren(); ++i) {
        std::string name = atts(i).GetName();
        if (name == "dynamic_texture") {
            int l_StageId = atts(i).GetIntProperty("stage_id", 0);
            std::string l_TextureName = atts(i).GetPszProperty("name", "");
            std::string l_FormatType = atts(i).GetPszProperty("format_type", "");
            bool l_WidthAsFrameBuffer = atts(i).GetBoolKeyword("texture_width_as_frame_buffer", false);//Este para que sirve?
            CTexture * l_Texture = new CTexture();
            //Width&Heigth ?
            l_Texture->Create(l_TextureName, 100, 100, 1, CTexture::TUsageType::RENDERTARGET, CTexture::TPoolType::DEFAULT, l_Texture->GetFormatTypeFromString(l_FormatType));
            AddStageTexture(l_StageId, l_Texture);
        }
    }
}

void CSetRenderTargetSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    for (int i = 0; i < m_StageTextures.size(); ++i) {
        m_StageTextures[i].m_Texture->SetAsRenderTarget(m_StageTextures[i].m_StageId);
    }
}
void CSetRenderTargetSceneRendererCommand::UnsetRenderTarget()
{
    for (int i = 0; i < m_StageTextures.size(); ++i) {
        m_StageTextures[i].m_Texture->UnsetAsRenderTarget(m_StageTextures[i].m_StageId);
    }
}
