
#include "RenderableCommands\DrawQuadRendererCommand.h"
#include "Texture\Texture.h"
#include "Effects\EffectTechnique.h"
#include <d3dx9.h>
#include "Effects\Effect.h"
#include "GraphicsManager.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Texture\TextureManager.h"



void CDrawQuadRendererCommand::DrawColoredQuad2DTexturedInPixelsByEffectTechnique(CGraphicsManager *RM, CEffectTechnique *EffectTechnique, RECT Rect, CColor Color, CTexture *Texture, float U0, float V0, float U1, float V1)
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


CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &atts) : CStagedTexturedRendererCommand(atts)
{
    Vect3f color = atts.GetVect3fProperty("color", v3fZERO);
    m_Color.Set(color.x, color.y, color.z);
    for (int i = 0; i < atts.GetNumChildren(); ++i) {
        std::string name = atts(i).GetName();
        if (name == "texture") {
            CTexture* l_Texture;
            int l_StageId = atts(i).GetIntProperty("stage_id");
            std::string l_TextureName = atts(i).GetPszProperty("file", "");
            uint32 l_Width, l_Height;
            GRAPHM->GetWidthAndHeight(l_Width, l_Height);
            if (atts(i).GetBoolProperty("load_file"))
                l_Texture->Load(l_TextureName);
            else {
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
    }
}



void CDrawQuadRendererCommand::Execute(CGraphicsManager &RM)
{
    //TODO Que se pille el nombre de la technique por XML
    CEffectTechnique * l_EffectTechnique = RENDTECHM->GetResource("DrawQuadSolidTechnique")->GetEffectTechnique();
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    for (size_t i = 0; i < m_StageTextures.size(); ++i) {
        RM.GetWidthAndHeight((uint32 &)rect.right, (uint32 &)rect.bottom);
        DrawColoredQuad2DTexturedInPixelsByEffectTechnique(&RM, l_EffectTechnique, rect, m_Color, m_StageTextures[i].m_Texture);
    }
}
/*
technique DrawQuadTechnique
{
pass p0
{
AlphaBlendEnable = false;
CullMode = CCW;
PixelShader = compile ps_3_0 DrawQuadPS();
}
}*/