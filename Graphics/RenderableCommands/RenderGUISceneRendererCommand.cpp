#include "RenderableCommands\RenderGUISceneRendererCommand.h"
#include "Core\Core.h"
#include "GUI\GUIManager.h"

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(CXMLTreeNode &atts) {}

void CRenderGUISceneRendererCommand::Execute(CGraphicsManager &RM)
{
    CCORE->GetGuiManager()->Render(&RM, FONTM);
}
