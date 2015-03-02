#include "RenderableCommands\StagedTexturedRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Texture\Texture.h"
#include "Utils\Defines.h"
#include "Effects\EffectTechnique.h"
#include "Effects\Effect.h"
#include "Core_Utils/MemLeaks.h"
#include "Core\Core.h"
#include "Texture\TextureManager.h"


CStagedTexturedRendererCommand::CStagedTexturedRendererCommand(CXMLTreeNode &atts)
{
	
    for (int i = 0; i < atts.GetNumChildren(); ++i) {
        std::string name = atts(i).GetName();
        if (name == "dynamic_texture") {
            int l_StageId = atts(i).GetIntProperty("stage_id", 0, false);
            std::string l_TextureName = atts(i).GetPszProperty("name", "", false);
            std::string l_FormatType = atts(i).GetPszProperty("format_type", "", false);
            int l_Width = atts(i).GetIntProperty("width", 0, false);
            int l_Height = atts(i).GetIntProperty("height", 0, false);
            bool l_WidthAsFrameBuffer = atts(i).GetBoolProperty("texture_width_as_frame_buffer", false, false);
            if ((l_Width == 0 || l_Height == 0) && l_WidthAsFrameBuffer) {
                D3DVIEWPORT9 * l_viewPort = new D3DVIEWPORT9();
                GRAPHM->GetDevice()->GetViewport(l_viewPort);
                l_Width = l_viewPort->Width;
                l_Height = l_viewPort->Height;
                CHECKED_DELETE(l_viewPort);
            }
            CTexture * l_Texture;
            if (TEXTM->ExisteResource(l_TextureName)) {
                l_Texture = TEXTM->GetResource(l_TextureName);
            } else {
                l_Texture = new CTexture();
                l_Texture->Create(l_TextureName, l_Width, l_Height, 1, CTexture::TUsageType::RENDERTARGET, CTexture::TPoolType::DEFAULT, l_Texture->GetFormatTypeFromString(l_FormatType));
                AddStageTexture(l_StageId, l_Texture);
				TEXTM->AddResource(l_TextureName, l_Texture);
            }
            
			//AddStageTexture(l_StageId, l_Texture);
        } else  if (name == "texture") {
            CTexture* l_Texture;
            Vect3f color = atts.GetVect3fProperty("color", v3fZERO, false);
            m_Color.Set(color.x, color.y, color.z);
            m_TechniqueName = atts.GetPszProperty("technique_name", "", false);
            int l_StageId = atts(i).GetIntProperty("stage_id");
            std::string l_TextureName = atts(i).GetPszProperty("file", "");
            uint32 l_Width, l_Height;
            GRAPHM->GetWidthAndHeight(l_Width, l_Height);
            if (atts(i).GetBoolProperty("load_file", false, false)) {
                l_Texture = new CTexture();
                l_Texture->Load(l_TextureName);
                TEXTM->AddResource(l_TextureName, l_Texture);
            } else {
                // bool Exist = TEXTM->ExisteResource(l_TextureName);
                //  l_Texture = TEXTM->GetResource(l_TextureName);
                //int width = l_Texture->GetWidth();
                /*if (!TEXTM->ExisteResource(l_TextureName)) {
                    l_Texture = new CTexture();
                    l_Texture->Create(l_TextureName, l_Width, l_Height, 0, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::A8R8G8B8);
                    TEXTM->AddResource(l_TextureName, l_Texture);
                }*/
                if (TEXTM->ExisteResource(l_TextureName)) {
                    l_Texture = TEXTM->GetResource(l_TextureName);
                    AddStageTexture(l_StageId, l_Texture);
                }
            }
            //AddStageTexture(l_StageId, l_Texture);
        }
    }
}
void CStagedTexturedRendererCommand::ActivateTextures()
{
    for (int i = 0; i < m_StageTextures.size(); ++i) {
        if (TEXTM->ExisteResource(m_StageTextures[i].m_Texture->getName()))
            m_StageTextures[i].m_Texture->Activate(m_StageTextures[i].m_StageId);
    }
}

CStagedTexturedRendererCommand::~CStagedTexturedRendererCommand()
{
    /*for (int i = 0; i < m_StageTextures.size(); ++i) {
        CHECKED_DELETE (m_StageTextures[i].m_Texture);
    }*/
    m_StageTextures.clear();
}
void CStagedTexturedRendererCommand::AddStageTexture(int StageId, CTexture *Texture)
{
    if (Texture != NULL)	{
        //  CKGStageTexture l_CKGStageTexture = new CKGStageTexture(StageId, Texture);
        m_StageTextures.push_back(CKGStageTexture (StageId, Texture));
    }
}

void CStagedTexturedRendererCommand::CKGStageTexture::Activate()
{
    m_Texture->Activate(m_StageId);
}

void CStagedTexturedRendererCommand::DrawColoredQuad2DTexturedInPixelsByEffectTechnique(CGraphicsManager *RM, CEffectTechnique *EffectTechnique, RECT Rect, CColor Color, CTexture *Texture, float U0, float V0, float U1, float V1)
{
    if (EffectTechnique == NULL)
        return;
    EffectTechnique->BeginRender();
    LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetD3DEffect();
    if (l_Effect != NULL) {
        l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());
        UINT l_NumPasses;
        l_Effect->Begin(&l_NumPasses, 0);
        for (UINT iPass = 0; iPass < l_NumPasses; iPass++) {
            l_Effect->BeginPass(iPass);
            RM->DrawColoredQuad2DTexturedInPixels(Rect, Color, Texture, U0, V0, U1, V1);
            l_Effect->EndPass();
        }
        l_Effect->End();
    }
}