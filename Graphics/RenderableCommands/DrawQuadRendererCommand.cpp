#include "RenderableCommands\DrawQuadRendererCommand.h"
#include "Texture\Texture.h"
#include "Effects\EffectTechnique.h"
#include <d3dx9.h>
#include "Effects\Effect.h"
#include "GraphicsManager.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Texture\TextureManager.h"
#include "RenderableVertex\VertexTypes.h"
#include "Core_Utils/MemLeaks.h"

CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &atts) : CStagedTexturedRendererCommand(atts) {
  /* Vect3f color = atts.GetVect3fProperty("color", v3fZERO, false);
   m_Color.Set(color.x, color.y, color.z);
   m_TechniqueName = atts.GetPszProperty("technique_name", "", false);
   for (int i = 0; i < atts.GetNumChildren(); ++i) {
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
   }*/
}



void CDrawQuadRendererCommand::Execute(CGraphicsManager &RM) {
  CEffectTechnique *l_EffectTechnique = RENDTECHM->GetResource(RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(VERTEX_TYPE_SCREEN))->GetEffectTechnique(); //"DrawQuadSolidTechnique"
  RECT rect;
  rect.top = 0;
  rect.left = 0;
  ActivateTextures();
  RM.GetWidthAndHeight((uint32 &)rect.right, (uint32 &)rect.bottom);
  DrawColoredQuad2DTexturedInPixelsByEffectTechnique(&RM, l_EffectTechnique, rect, m_Color, m_StageTextures[0].m_Texture);
}