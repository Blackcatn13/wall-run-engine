#include "RenderableCommands\ParticleRendererCommand.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"
#include "Core\Core.h"
#include "Particles\ParticleManager.h"
#include "Particles\ParticleEmitter.h"
#include "Particles\BillboardManager.h"
#include "Utils\Defines.h"

CParticleRendererCommand::CParticleRendererCommand(CXMLTreeNode &atts)
  : CSceneRendererCommand(atts) {
}

CParticleRendererCommand::~CParticleRendererCommand() {
}

void CParticleRendererCommand::Execute(CGraphicsManager &RM) {
  //PARTIM->Render(&RM);
  //BILLBM->Render(&RM);
}
