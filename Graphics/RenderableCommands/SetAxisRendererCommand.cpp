#include "RenderableCommands\SetAxisRendererCommand.h"
#include "GraphicsManager.h"
#include "Math\Matrix44.h"

CSetAxisRendererCommand::CSetAxisRendererCommand(CXMLTreeNode &atts)
    : CSceneRendererCommand(atts)
{
}

void CSetAxisRendererCommand::Execute(CGraphicsManager &RM)
{
    Mat44f t;
    t.SetIdentity();
    RM.SetTransform(t);
    RM.DrawGrid(20);
    RM.DrawAxis(10);
}
