#include "RenderableCommands\DeferredShadingSceneRendererCommand.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Renderable\RenderableObjectTechnique.h"
#include "Core\Core.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"


CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &atts)
{
    std::string l_Technique = atts.GetPszProperty("technique_name");
    m_RenderableObjectTechnique = RENDTECHM->GetResource(l_Technique);
}
void CDeferredShadingSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    //recorrer todas las luces y aplicar el shader de deferred
}