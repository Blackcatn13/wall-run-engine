#include "RenderableCommands\SceneRendererCommandManager.h"
#include "GraphicsManager.h"
#include <sstream>
#include <map>
#include <vector>
#include "Core\Core.h"
//Includes de los Commands
#include "RenderableCommands\BeginRenderSceneRendererCommand.h"
#include "RenderableCommands\ClearSceneRendererCommand.h"
#include "RenderableCommands\CaptureFrameBufferSceneRendererCommand.h"
#include "RenderableCommands\DeferredShadingSceneRendererCommand.h"
#include "RenderableCommands\DisableZTestSceneRendererCommand.h"
#include "RenderableCommands\DisableZWriteSceneRendererCommand.h"
#include "RenderableCommands\DrawQuadRendererCommand.h"
#include "RenderableCommands\EnableZTestSceneRendererCommand.h"
#include "RenderableCommands\EnableZWriteSceneRendererCommand.h"
#include "RenderableCommands\EndRenderSceneRendererCommand.h"
#include "RenderableCommands\GenerateShadowMapsSceneRendererCommand.h"
#include "RenderableCommands\PresentSceneRenderCommand.h"
#include "RenderableCommands\RenderableObjectTechniquesSceneRendererCommand.h"
#include "RenderableCommands\RenderDebugLightsSceneRendererCommand.h"
#include "RenderableCommands\RenderDebugSceneSceneRendererCommand.h"
#include "RenderableCommands\RenderDebugShadowMapsSceneRendererCommand.h"
#include "RenderableCommands\RenderGUISceneRendererCommand.h"
#include "RenderableCommands\RenderSceneSceneRendererCommand.h"
#include "RenderableCommands\SetMatricesSceneRendererCommand.h"
#include "RenderableCommands\SetRenderTargetSceneRendererCommand.h"
//#include "RenderableCommands\StagedTexturedRendererCommand.h"
#include "RenderableCommands\UnsetRenderTargetSceneRendererCommand.h"
#include "RenderableCommands\SetAxisRendererCommand.h"
#include "RenderableCommands\EnableAlphaBlendCommand.h"
#include "RenderableCommands\DisableAlphaBlendCommand.h"
#include "RenderableCommands\RenderDebugInfoSceneRendererCommand.h"
#include "RenderableCommands\GaussianBlurSceneRendererCommand.h"
#include "RenderableCommands\ParticleRendererCommand.h"
#include "RenderableCommands\CheckEnabledPolyRenderCommand.h"
#include "RenderableCommands\SetTextureInSpecificStageCommand.h"
#include "RenderableCommands\BillboardRendererCommand.h"


#include "Core_Utils/MemLeaks.h"


CSceneRendererCommandManager::CSceneRendererCommandManager()
  : m_FileName(""),
    m_needReload(false),
    m_needReloadGUI(false),
    m_CommandNumber(0) {
  CleanUp();
}
CSceneRendererCommandManager::~CSceneRendererCommandManager() {
  CleanUp();
}

void CSceneRendererCommandManager::CleanUp() {
  for (auto it = m_commandsMaps.begin(); it != m_commandsMaps.end(); ++it) {
    (*it).second.Destroy();
  }
}

std::string CSceneRendererCommandManager::GetNextName() {
  std::ostringstream ss;
  ss << m_CommandNumber++;
  std::string l_Name = "SCeneRenderCommand_" + ss.str();
  return l_Name;
}

void CSceneRendererCommandManager::Load(const std::string &FileName) {
  m_FileName = FileName;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(FileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    int scene_commands_count = newFile.GetNumChildren();
    for (int count = 0; count < scene_commands_count; count++) {
      CXMLTreeNode m = newFile.getNextChild();
      CTemplatedVectorMapManager<CSceneRendererCommand> *toInsert;
      std::string treeName = m.GetName();
      if (treeName == "renderer_list") {
        int numRenderers = m.GetNumChildren();
        for (int renderers = 0; renderers < numRenderers; ++renderers) {
          m_commandsMaps[m.getNextChild().GetPszISOProperty("name", "")] = CTemplatedVectorMapManager<CSceneRendererCommand>();
        }
      } else {
        toInsert = &m_commandsMaps[treeName];
        if (m.GetBoolProperty("active"))
          m_activeRenderer = treeName;
      }
      /*#ifdef _PARTICLEVIEWER
            if (treeName == "particle_viewer") {
              toInsert = &m_SceneRendererCommandsParticle;
            }
      #else
            if (treeName == "normal_render") {

            } else if (treeName == "gui_render") {
              toInsert = &m_SceneRendererCommandsGUI;
            }
      #endif*/
      //}
      //CXMLTreeNode  m = newFile["scene_renderer_commands"];
      if (m.Exists()) {
        int count = m.GetNumChildren();
        for (int i = 0; i < count; ++i) {
          CXMLTreeNode child = m.getNextChild();
          std::string l_Name = child.GetPszProperty("name", "", false);
          if (l_Name.empty())
            l_Name = GetNextName();
          std::string name = child.GetName();
          if (name == "begin_scene") {
            CBeginRenderSceneRendererCommand *l_Command = new CBeginRenderSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "clear_scene") {
            CClearSceneRendererCommand *l_Command = new CClearSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "capture_frame_buffer") {
            CCaptureFrameBufferSceneRendererCommand *l_Command = new CCaptureFrameBufferSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "render_deferred_shading") {
            CDeferredShadingSceneRendererCommand *l_Command = new CDeferredShadingSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "disable_z_write") {
            CDisableZWriteSceneRendererCommand *l_Command = new CDisableZWriteSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "render_draw_quad") {
            CDrawQuadRendererCommand *l_Command = new CDrawQuadRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "gaussian_blur") {
            CGaussianBlueSceneRendererCommand *l_Command = new CGaussianBlueSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "enable_z_write") {
            CEnableZWriteSceneRendererCommand *l_Command = new CEnableZWriteSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "enable_alpha_blend") {
            CEnableAlphaBlendSceneRendererCommand *l_Command = new CEnableAlphaBlendSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "disable_alpha_blend") {
            CDisableAlphaBlendSceneRendererCommand *l_Command = new CDisableAlphaBlendSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "end_scene") {
            CEndRenderSceneRendererCommand *l_Command = new CEndRenderSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "generate_shadow_maps") {
            CGenerateShadowMapsSceneRendererCommand *l_Command = new CGenerateShadowMapsSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "present") {
            CPresentSceneRendererCommand *l_Command = new CPresentSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "set_pool_renderable_objects_technique") {
            CRenderableObjectTechniquesSceneRendererCommand *l_Command = new CRenderableObjectTechniquesSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "render_debug_lights") {
            CRenderDebugLightsSceneRenderCommand *l_Command = new CRenderDebugLightsSceneRenderCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "render_debug_scene") {
            CRenderDebugSceneSceneRendererCommand *l_Command = new CRenderDebugSceneSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "set_texture_in_stage") {
            CSetTextureInSpecificStageCommand *l_Command = new CSetTextureInSpecificStageCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          //if (name == "render_debug_shadow_maps") {
          //	CRenderDebugShadowMapsSceneRendererCommand *l_Command = new CRenderDebugShadowMapsSceneRendererCommand(child);
          //	toInsert->AddResource(l_Name,l_Command);
          //}
          //if (name == "render_gui") {
          //	CRenderGUISceneRendererCommand *l_Command = new CRenderGUISceneRendererCommand(child);
          //	toInsert->AddResource(l_Name,l_Command);
          //}
          if (name == "render_scene") {
            CRenderSceneSceneRendererCommand *l_Command = new CRenderSceneSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "set_matrices") {
            CSetMatricesSceneRendererCommand *l_Command = new CSetMatricesSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "set_axis") {
            CSetAxisRendererCommand *l_Command = new CSetAxisRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "set_render_target") {
            CSetRenderTargetSceneRendererCommand *l_Command = new CSetRenderTargetSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          ///*if (name == "texture") {
          //	CStagedTexturedRendererCommand *l_Command = new CStagedTexturedRendererCommand(child);
          //	toInsert->AddResource(l_Name,l_Command);
          //}*/
          if (name == "unset_render_target") {
            std::string l_CommandName = child.GetPszProperty("render_target", "");
            CSetRenderTargetSceneRendererCommand *l_UnsetRenderTarget = (CSetRenderTargetSceneRendererCommand *)toInsert->GetResourcesMap().find(l_CommandName)->second.m_Value;
            //	GetSetRenderTargetSceneRendererCommand(l_UnsetRenderTarget);
            if (l_UnsetRenderTarget != NULL) {
              CUnsetRenderTargetSceneRendererCommand *l_Command = new CUnsetRenderTargetSceneRendererCommand(l_UnsetRenderTarget, child);
              toInsert->AddResource(l_Name, l_Command);
            }
          }
          if (name == "enable_z_test") {
            CEnableZTestSceneRendererCommand *l_Command = new CEnableZTestSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "disable_z_test") {
            CDisableZTestSceneRendererCommand *l_Command = new CDisableZTestSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "render_debug_info") {
            CRenderDebugInfoSceneRendererCommand *l_Command = new CRenderDebugInfoSceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "render_gui") {
            CRenderGUISceneRendererCommand *l_Command = new CRenderGUISceneRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "particle_render") {
            CParticleRendererCommand *l_Command = new CParticleRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "check_active_poly") {
            CCheckEnabledPolyRenderCommand *l_Command = new CCheckEnabledPolyRenderCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
          if (name == "billboard_render") {
            CBillboardRendererCommand *l_Command = new CBillboardRendererCommand(child);
            toInsert->AddResource(l_Name, l_Command);
          }
        }
      }
    }
  }
}
//
//void CSceneRendererCommandManager::GetSetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *TargetSceneRendererCommand)
//{
//    for (int i = 0; i < m_SceneRendererCommands.GetResourcesVector().size(); ++i) {
//        TargetSceneRendererCommand = (CSetRenderTargetSceneRendererCommand *)m_SceneRendererCommands.GetResourcesVector().at(i);
//    }
//}
void CSceneRendererCommandManager::Execute(CGraphicsManager &RM) {
  for (size_t i = 0; i < m_commandsMaps[m_activeRenderer].GetResourcesVector().size(); ++i) {
    if (m_needReload) {
      m_commandsMaps[m_activeRenderer].GetResourcesVector().at(i)->Reload();
    }
    m_commandsMaps[m_activeRenderer].GetResourcesVector().at(i)->Execute(RM);
  }
  if (m_needReload)
    m_needReload = false;
}

void CSceneRendererCommandManager::Reload() {
  CleanUp();
  Load(m_FileName);
}