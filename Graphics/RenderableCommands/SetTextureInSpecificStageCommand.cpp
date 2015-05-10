#include "RenderableCommands\SetTextureInSpecificStageCommand.h"
#include "Texture\Texture.h"
#include "Effects\EffectTechnique.h"
#include <d3dx9.h>
#include "Effects\Effect.h"
#include "GraphicsManager.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Texture\TextureManager.h"
#include "RenderableVertex\VertexTypes.h"
#include "Core_Utils/MemLeaks.h"
#include "Core\Core.h"

CSetTextureInSpecificStageCommand::CSetTextureInSpecificStageCommand(CXMLTreeNode &atts) {
  m_TextureName = atts.GetPszProperty("texture_name", "", false);
  m_Stage = atts.GetIntProperty("stage_id", 0, false);

}



void CSetTextureInSpecificStageCommand::Execute(CGraphicsManager &RM) {
  TEXTM->GetResource(m_TextureName)->Activate(m_Stage);
}