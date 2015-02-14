#include "RenderableCommands\SetMatricesSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "Camera\CameraController.h"

CSetMatricesSceneRendererCommand::CSetMatricesSceneRendererCommand(CXMLTreeNode &atts)
{
}

void CSetMatricesSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    RM.SetupMatrices(CAMCONTM->getActiveCamera());
}