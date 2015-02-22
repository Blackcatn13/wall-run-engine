#include "RenderableCommands\SetMatricesSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "Camera\CameraController.h"
#include "Core_Utils/MemLeaks.h"

CSetMatricesSceneRendererCommand::CSetMatricesSceneRendererCommand(CXMLTreeNode &atts)
{
}

void CSetMatricesSceneRendererCommand::Execute(CGraphicsManager &RM)
{
    RM.SetupMatrices(CAMCONTM->getActiveCamera());
}