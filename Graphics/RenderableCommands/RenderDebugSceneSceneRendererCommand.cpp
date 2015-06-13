#include "RenderableCommands\RenderDebugSceneSceneRendererCommand.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Core\Core.h"
#include "PhysicsManager.h"
#include <vector>
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "Cooking Mesh\PhysicCookingMesh.h"
#include "ActionToInput.h"
#include "TriggerManager\Trigger.h"
#include "TriggerManager\TriggerManager.h"
#include "Enemies\EnemyManager.h"
#include "Core\PlayerController.h"
#include "Core\Process.h"


CRenderDebugSceneSceneRendererCommand::CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts):
  m_PaintTriggers(false),
  m_PaintAllPhysx(false),
  m_DebugMode(true),
  m_PaintCharacterControllers(false) {
  m_Layer = atts.GetPszProperty("layer", "solid", false);
  m_Active = atts.GetBoolProperty("active", true, false);
  m_PaintCookingMesh = atts.GetBoolProperty("paint_cooking_mesh", false, false);
  m_CookingMeshName = atts.GetPszProperty("cooking_mesh_name", "", false);
}
void CRenderDebugSceneSceneRendererCommand::Execute(CGraphicsManager &RM) {
#ifdef _DEBUG
  {
    RENDLM->setCurrentLayer(m_Layer);
    if (m_Active) {
      //Poner todo el pintado de físics a true
      //Pintar Físic controllers
      if (ACT2IN->DoAction("ToggleDebugMode")) {
        m_DebugMode = !m_DebugMode;
        PHYSXM->SetDebugRenderMode(m_DebugMode);
        PROCESS->setDebugPhisics(m_DebugMode);
      }
      if (ACT2IN->DoAction("ToggleDebugAllPhysx")) {
        m_PaintAllPhysx = !m_PaintAllPhysx;
        PHYSXM->SetPaintAllActors(m_PaintAllPhysx,  m_CookingMeshName,  m_PaintCookingMesh);
        PROCESS->setPaintPhisics(m_PaintAllPhysx);
      }
      if (ACT2IN->DoAction("ToggleDebugCharacters")) {
        m_PaintCharacterControllers = !m_PaintCharacterControllers;
        // CCORE->GetEnemyManager()->SetPaintEnemies(m_PaintCharacterControllers);
        //CCORE->GetPlayerController()->getPhysicUserData()->SetPaint(m_PaintCharacterControllers);

        PHYSXM->SetPaintAllPhysicControllers(m_PaintCharacterControllers);
        PROCESS->setPaintPhisicActors(m_PaintCharacterControllers);
      }
      if (ACT2IN->DoAction("ToggleDebugTriggers")) {
        m_PaintTriggers = !m_PaintTriggers;
        TRIGGM->GetResourcesVector();
        for (size_t i = 0; i < TRIGGM->GetResourcesVector().size(); ++i) {
          TRIGGM->GetResourcesVector().at(i)->GetUserData()->SetPaint(m_PaintTriggers);
        }
        PROCESS->setPaintPhisicTriggers(m_PaintTriggers);
      }
      PHYSXM->DebugRender(&RM);
      PHYSXM->SetPaintByActorName(m_CookingMeshName, m_PaintCookingMesh);
      //Render de la layer
      CCORE->Render();
      RENDLM->Render(GRAPHM, RENDLM->getCurrentLayer());
    }
  }
#endif
}
