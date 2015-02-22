#include "RenderableCommands\DeferredShadingSceneRendererCommand.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Renderable\RenderableObjectTechnique.h"
#include "Core\Core.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Texture\Texture.h"
#include "Texture\TextureManager.h"
#include "Effects\Effect.h"
#include "RenderableVertex\VertexTypes.h"
#include "Core_Utils/MemLeaks.h"

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &atts): CStagedTexturedRendererCommand(atts)
{
    //std::string l_Technique = atts.GetPszProperty("technique_name", "");
    //m_RenderableObjectTechnique = RENDTECHM->GetResource(l_Technique);
    for (int i = 0; i < atts.GetNumChildren(); ++i) {
        std::string name = atts(i).GetName();
        if (name == "texture") {
            CTexture* l_Texture;
            int l_StageId = atts(i).GetIntProperty("stage_id");
            std::string l_TextureName = atts(i).GetPszProperty("file", "");
            uint32 l_Width, l_Height;
            GRAPHM->GetWidthAndHeight(l_Width, l_Height);
            if (atts(i).GetBoolProperty("load_file", false, false))
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
    m_RenderableObjectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(VERTEX_TYPE_SCREEN));
}
void CDeferredShadingSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    //recorrer todas las luces y aplicar el shader de deferred
    ActivateTextures();
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    RM.GetWidthAndHeight((uint32 &)rect.right, (uint32 &)rect.bottom);
    CColor m_Color = CColor(0, 0, 0);
    CLightManager::TMapResource::iterator it = LIGHTM->GetResources().begin();
    CEffectTechnique* l_EffectTechnique = m_RenderableObjectTechnique->GetEffectTechnique();
    while (it != LIGHTM->GetResources().end()) {
        if (m_RenderableObjectTechnique->GetEffectTechnique() != NULL && m_RenderableObjectTechnique->GetEffectTechnique()->GetEffect() != NULL) {
            m_RenderableObjectTechnique->GetEffectTechnique()->GetEffect()->SetLight(it->second);
            DrawColoredQuad2DTexturedInPixelsByEffectTechnique(&RM, l_EffectTechnique, rect, m_Color, m_StageTextures[0].m_Texture);
        }
        ++it;
    }
}

void CDeferredShadingSceneRendererCommand::Reload()
{
    m_RenderableObjectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(VERTEX_TYPE_SCREEN));
}