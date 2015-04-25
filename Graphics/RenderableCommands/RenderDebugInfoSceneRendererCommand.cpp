#include "RenderableCommands\RenderDebugInfoSceneRendererCommand.h"
#include "Core\Core.h"
#include "Core_Utils\LogRender.h"
#include "Utils\Logger.h"
#include "Core\Process.h"
#include "GUI\GUIManager.h"
#include "Texture\Texture.h"

CRenderDebugInfoSceneRendererCommand::CRenderDebugInfoSceneRendererCommand(CXMLTreeNode &atts) {
  m_LogRender = LOGRNDR;
  LOGGER->SetCapacity(500);
  m_LogRender->SetLinePerPage(30);
}

void CRenderDebugInfoSceneRendererCommand::Execute(CGraphicsManager &RM) {
#ifdef _DEBUG
  m_LogRender->Render(&RM, FONTM);
  PROCESS->RenderDebugInfo(PROCESS->getPrintInfo()/*, PROCESS->GetDt()*/);
  //CCORE->GetGuiManager()->Render(&RM, FONTM);
  //CTexture* texture = new CTexture();
  //texture->Load("./Data/GUI/Textures_LaberynthGame/f_pral.png");
  //CCORE->GetGraphicsManager()->DrawQuad2D(Vect2i(100,100),1200,700,CENTER,texture);
#endif
}
