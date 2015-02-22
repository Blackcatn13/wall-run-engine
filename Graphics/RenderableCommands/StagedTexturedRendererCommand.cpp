#include "RenderableCommands\StagedTexturedRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Texture\Texture.h"
#include "Utils\Defines.h"
#include "Effects\EffectTechnique.h"
#include "Effects\Effect.h"
#include "Core_Utils/MemLeaks.h"


CStagedTexturedRendererCommand::CStagedTexturedRendererCommand(CXMLTreeNode &atts)
{
}
void CStagedTexturedRendererCommand::ActivateTextures()
{
    for (int i = 0; i < m_StageTextures.size(); ++i) {
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