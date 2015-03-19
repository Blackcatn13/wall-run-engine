#include "RenderableCommands\ParticleRendererCommand.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"
#include "Particles\Billboard.h"
#include "Utils\Defines.h"

CParticleRendererCommand::CParticleRendererCommand(CXMLTreeNode &atts)
    : CSceneRendererCommand(atts)
{
    b = new CBillboard(3, Vect3f(0, 15, 0));
}

CParticleRendererCommand::~CParticleRendererCommand()
{
    CHECKED_DELETE(b);
}

void CParticleRendererCommand::Execute(CGraphicsManager &RM)
{
    b->Render(&RM);
}
