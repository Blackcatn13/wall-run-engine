#include "RenderableCommands\RenderDebugInfoSceneRendererCommand.h"
#include "Core\Core.h"
#include "Core_Utils\LogRender.h"
#include "Utils\Logger.h"
#include "Core\Process.h"

CRenderDebugInfoSceneRendererCommand::CRenderDebugInfoSceneRendererCommand(CXMLTreeNode &atts) {
  m_LogRender = LOGRNDR;
  LOGGER->SetCapacity(500);
  m_LogRender->SetLinePerPage(30);
}

void CRenderDebugInfoSceneRendererCommand::Execute(CGraphicsManager &RM) {
  m_LogRender->Render(&RM, FONTM);
  PROCESS->RenderDebugInfo(PROCESS->getPrintInfo(), PROCESS->GetDt());
}
