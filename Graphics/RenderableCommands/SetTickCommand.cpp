#include "RenderableCommands\SetTickCommand.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"
#include "Core\Core.h"
#include "Utils\Defines.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Core_Utils\TickCounter.h"

CSetTickCommand::CSetTickCommand(CXMLTreeNode &atts)
  : CSceneRendererCommand(atts)
  , name(atts.GetPszISOProperty("tick", ""))
  , m_technique(atts.GetPszISOProperty("technique", "", false)) {
  m_effectTechnique = EFFECTM->GetEffectTechnique(m_technique);
}

CSetTickCommand::~CSetTickCommand() {
}

void CSetTickCommand::Execute(CGraphicsManager &RM) {
  m_effectTechnique->SetTick(CCORE->GetTickCounter()->getValue(name));
}
