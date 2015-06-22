#include "RenderableCommands\UnsetRenderTargetSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "SetRenderTargetSceneRendererCommand.h"
#include "Utils\Defines.h"
#include "Core_Utils/MemLeaks.h"

CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SetRenderTargetRendererCommand, CXMLTreeNode &atts)
  : m_depthStencil(atts.GetIntProperty("stencil_buffer", -1)) {
  m_SetRenderTargetRendererCommand = SetRenderTargetRendererCommand;
}
void CUnsetRenderTargetSceneRendererCommand::Execute(CGraphicsManager &RM) {
  m_SetRenderTargetRendererCommand->UnsetRenderTarget(m_depthStencil);
}

