#include "RenderableCommands\BillboardRendererCommand.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"
#include "Core\Core.h"
#include "Particles\ParticleManager.h"
#include "Particles\ParticleEmitter.h"
#include "Particles\BillboardManager.h"
#include "Utils\Defines.h"

CBillboardRendererCommand::CBillboardRendererCommand(CXMLTreeNode &atts)
  : CSceneRendererCommand(atts) {
}

CBillboardRendererCommand::~CBillboardRendererCommand() {
}

void CBillboardRendererCommand::Execute(CGraphicsManager &RM) {
  BILLBM->Render(&RM);
}
