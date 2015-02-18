#include "RenderableCommands\CaptureFrameBufferSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Texture\Texture.h"
#include "Core\Core.h"
#include "Utils\Defines.h"
#include "Texture\TextureManager.h"

CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &atts)
{
    for (int i = 0; i < atts.GetNumChildren(); ++i) {
        std::string name = atts(i).GetName();
        if (name == "dynamic_texture") {
            int l_StageId = atts(i).GetIntProperty("stage_id", 0);
            std::string l_TextureName = atts(i).GetPszProperty("name", "");
            std::string l_FormatType = atts(i).GetPszProperty("format_type", "");
            int l_Width = atts(i).GetIntProperty("width", 0);
            int l_Height = atts(i).GetIntProperty("heigth", 0);
            bool l_WidthAsFrameBuffer = atts(i).GetBoolProperty("texture_width_as_frame_buffer", false);//Este para que sirve?
            if ((l_Width == 0 || l_Height == 0) && l_WidthAsFrameBuffer) {
                D3DVIEWPORT9 * l_viewPort = new D3DVIEWPORT9();
                GRAPHM->GetDevice()->GetViewport(l_viewPort);
                l_Width = l_viewPort->Width;
                l_Height = l_viewPort->Height;
                CHECKED_DELETE(l_viewPort);
            }
            CTexture * l_Texture = new CTexture();
            l_Texture->Create(l_TextureName, l_Width, l_Height, 1, CTexture::TUsageType::RENDERTARGET, CTexture::TPoolType::DEFAULT, l_Texture->GetFormatTypeFromString(l_FormatType));
            AddStageTexture(l_StageId, l_Texture);
            TEXTM->AddResource(l_TextureName, l_Texture);
        }
    }
}
void CCaptureFrameBufferSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    for (int i = 0; i < m_StageTextures.size(); ++i) {
        m_StageTextures[i].m_Texture->CaptureFrameBuffer(m_StageTextures[i].m_StageId);
		bool l_Done=false;
		if(l_Done)
          m_StageTextures[i].m_Texture->SaveToFile("d:\\wololo.jpg");
    }
}