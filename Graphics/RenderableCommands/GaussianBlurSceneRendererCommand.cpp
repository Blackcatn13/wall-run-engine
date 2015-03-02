#include "RenderableCommands\GaussianBlurSceneRendererCommand.h"
#include "Texture\Texture.h"
#include "Effects\EffectTechnique.h"
#include <d3dx9.h>
#include "Effects\Effect.h"
#include "GraphicsManager.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Texture\TextureManager.h"
#include "RenderableVertex\VertexTypes.h"
#include "Core_Utils/MemLeaks.h"

CGaussianBlueSceneRendererCommand::CGaussianBlueSceneRendererCommand(CXMLTreeNode &atts) : CStagedTexturedRendererCommand(atts)
{
    Vect3f color = atts.GetVect3fProperty("color", v3fZERO, false);
    m_Color.Set(color.x, color.y, color.z);
    m_TechniqueName = atts.GetPszProperty("technique_name", "", false);
	m_GaussianLoops = atts.GetIntProperty("gaussian_loops", 2, false);
	int l_numDynamicTexture = 0;
	if ((m_GaussianLoops % 2) != 0) ++m_GaussianLoops;
    /*for (int i = 0; i < atts.GetNumChildren(); ++i) {
        std::string name = atts(i).GetName();
        if (name == "texture") {
            CTexture* l_Texture; 
            int l_StageId = atts(i).GetIntProperty("stage_id");
            std::string l_TextureName = atts(i).GetPszProperty("file", "");
            uint32 l_Width, l_Height;
            GRAPHM->GetWidthAndHeight(l_Width, l_Height);
            if (atts(i).GetBoolProperty("load_file", false, false)) {
                l_Texture = new CTexture();
                l_Texture->Load(l_TextureName);
                TEXTM->AddResource(l_TextureName, l_Texture);
            } else {
                l_Texture = TEXTM->GetResource(l_TextureName);
                int width = l_Texture->GetWidth();
                if (l_Texture == NULL) {
                    l_Texture = new CTexture();
                    l_Texture->Create(l_TextureName, l_Width, l_Height, 0, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::A8R8G8B8);
                    TEXTM->AddResource(l_TextureName, l_Texture);
                }
            }
            AddStageTexture(l_StageId, l_Texture);
        }		
		else if (name == "dynamic_texture") {
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
		
	}*/
}

void CGaussianBlueSceneRendererCommand::Execute(CGraphicsManager &RM)
{
	//un pasada
	//int l_numLoops = 0;
    CEffectTechnique * l_EffectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(VERTEX_TYPE_SCREEN))->GetEffectTechnique();//"DrawQuadSolidTechnique"
	RECT rect;
    rect.top = 0;
    rect.left = 0;
	/*
	
	m_StageTextures[2].m_Texture->SetAsRenderTarget(0);
	
	RM.GetWidthAndHeight((uint32 &)rect.right, (uint32 &)rect.bottom);

	DrawColoredQuad2DTexturedInPixelsByEffectTechnique(&RM, l_EffectTechnique, rect, m_Color, m_StageTextures[0].m_Texture);
	m_StageTextures[2].m_Texture->UnsetAsRenderTarget();

	m_StageTextures[1].m_Texture->SetAsRenderTarget(0);
	
	RM.GetWidthAndHeight((uint32 &)rect.right, (uint32 &)rect.bottom);

	DrawColoredQuad2DTexturedInPixelsByEffectTechnique(&RM, l_EffectTechnique, rect, m_Color, m_StageTextures[2].m_Texture);
	m_StageTextures[1].m_Texture->UnsetAsRenderTarget();
	// hasta aqui una pasada
	*/
	
	for(int i=0;i<m_GaussianLoops;++i)
	{
		CTexture *l_SourceTexture=i==0 ? m_StageTextures[0].m_Texture : ((i%2)==0 ? m_StageTextures[1].m_Texture : m_StageTextures[2].m_Texture);
		CTexture *l_RenderTargetTexture=(i%2)==0 ? m_StageTextures[2].m_Texture: m_StageTextures[1].m_Texture;
		
		//CEffectTechnique *l_EffectTechnique=i==0 ? DrawQuadTechnique : GaussianTechnique;
		
		l_RenderTargetTexture->SetAsRenderTarget(0);
		RM.GetWidthAndHeight((uint32 &)rect.right, (uint32 &)rect.bottom);
		rect.right=l_RenderTargetTexture->GetWidth();
		rect.bottom=l_RenderTargetTexture->GetHeight();
		DrawColoredQuad2DTexturedInPixelsByEffectTechnique(&RM, l_EffectTechnique, rect, m_Color, l_SourceTexture);
		
		l_RenderTargetTexture->UnsetAsRenderTarget();
	}
	
	/*while (l_numLoops < m_GaussianLoops) {
		m_StageTextures[0].m_Texture->CaptureFrameBuffer(m_StageTextures[0].m_StageId);
        DrawColoredQuad2DTexturedInPixelsByEffectTechnique(&RM, l_EffectTechnique, rect, m_Color, m_StageTextures[0].m_Texture);
		++l_numLoops;
    }
	*/

	/*
	for(int i=0;i<Passes;++i)
	{
		CTexture *l_SourceTexture=i==0 ? SourceTexture : ((i%2)==0 ? OkTexture : TempTexture);
		CTexture *l_RenderTargetTexture=(i%2)==0 ? TempTexture : OkTexture;
		
		CEffectTechnique *l_Technique=i==0 ? DrawQuadTechnique : GaussianTechnique;
		
		l_RenderTargetTexture->SetRenderTarget(0);
		DrawQuad(fullrectDelRenderTarget, l_SourceTexture, technique);
		
		RenderTargetTexture->UnsetRenderTarget();
	}*/

}