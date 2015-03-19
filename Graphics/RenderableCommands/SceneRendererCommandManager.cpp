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
#include "Core_Utils/MemLeaks.h"


CSceneRendererCommandManager::CSceneRendererCommandManager()
    : m_FileName(""),
      m_needReload(false)
{
    CleanUp();
}
CSceneRendererCommandManager::~CSceneRendererCommandManager()
{
    CleanUp();
}

void CSceneRendererCommandManager::CleanUp()
{
    //if(!m_SceneRendererCommands.GetResourcesMap().empty())
    m_SceneRendererCommands.Destroy();
}

std::string CSceneRendererCommandManager::GetNextName()
{
    std::ostringstream ss;
    ss << m_SceneRendererCommands.GetResourcesVector().size();
    std::string l_Name = "SCeneRenderCommand_" + ss.str();
    return l_Name;
}

void CSceneRendererCommandManager::Load(const std::string &FileName)
{
    m_FileName = FileName;
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(FileName.c_str())) {
        printf("ERROR loading the file.");
    } else {
        CXMLTreeNode  m = newFile["scene_renderer_commands"];
        if (m.Exists()) {
            int count = m.GetNumChildren();
            for (int i = 0; i < count; ++i) {
                std::string l_Name = m(i).GetPszProperty("name", "", false);
                if (l_Name.empty())
                    l_Name = GetNextName();
                std::string name = m(i).GetName();
                if (name == "begin_scene") {
                    CBeginRenderSceneRendererCommand *l_Command = new CBeginRenderSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "clear_scene") {
                    CClearSceneRendererCommand *l_Command = new CClearSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "capture_frame_buffer") {
                    CCaptureFrameBufferSceneRendererCommand *l_Command = new CCaptureFrameBufferSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "render_deferred_shading") {
                    CDeferredShadingSceneRendererCommand *l_Command = new CDeferredShadingSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "disable_z_write") {
                    CDisableZWriteSceneRendererCommand *l_Command = new CDisableZWriteSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "render_draw_quad") {
                    CDrawQuadRendererCommand *l_Command = new CDrawQuadRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "gaussian_blur") {
                    CGaussianBlueSceneRendererCommand *l_Command = new CGaussianBlueSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "enable_z_write") {
                    CEnableZWriteSceneRendererCommand *l_Command = new CEnableZWriteSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "enable_alpha_blend") {
                    CEnableAlphaBlendSceneRendererCommand *l_Command = new CEnableAlphaBlendSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "disable_alpha_blend") {
                    CDisableAlphaBlendSceneRendererCommand *l_Command = new CDisableAlphaBlendSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "end_scene") {
                    CEndRenderSceneRendererCommand *l_Command = new CEndRenderSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "generate_shadow_maps") {
                    CGenerateShadowMapsSceneRendererCommand *l_Command = new CGenerateShadowMapsSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "present") {
                    CPresentSceneRendererCommand *l_Command = new CPresentSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "set_pool_renderable_objects_technique") {
                    CRenderableObjectTechniquesSceneRendererCommand *l_Command = new CRenderableObjectTechniquesSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "render_debug_lights") {
                    CRenderDebugLightsSceneRenderCommand *l_Command = new CRenderDebugLightsSceneRenderCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                //if (name == "render_debug_scene") {
                //	CRenderDebugSceneSceneRendererCommand *l_Command = new CRenderDebugSceneSceneRendererCommand(m(i));
                //	m_SceneRendererCommands.AddResource(l_Name,l_Command);
                //}
                //if (name == "render_debug_shadow_maps") {
                //	CRenderDebugShadowMapsSceneRendererCommand *l_Command = new CRenderDebugShadowMapsSceneRendererCommand(m(i));
                //	m_SceneRendererCommands.AddResource(l_Name,l_Command);
                //}
                //if (name == "render_gui") {
                //	CRenderGUISceneRendererCommand *l_Command = new CRenderGUISceneRendererCommand(m(i));
                //	m_SceneRendererCommands.AddResource(l_Name,l_Command);
                //}
                if (name == "render_scene") {
                    CRenderSceneSceneRendererCommand *l_Command = new CRenderSceneSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "set_matrices") {
                    CSetMatricesSceneRendererCommand *l_Command = new CSetMatricesSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "set_axis") {
                    CSetAxisRendererCommand *l_Command = new CSetAxisRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "set_render_target") {
                    CSetRenderTargetSceneRendererCommand *l_Command = new CSetRenderTargetSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                ///*if (name == "texture") {
                //	CStagedTexturedRendererCommand *l_Command = new CStagedTexturedRendererCommand(m(i));
                //	m_SceneRendererCommands.AddResource(l_Name,l_Command);
                //}*/
                if (name == "unset_render_target") {
                    std::string l_CommandName = m(i).GetPszProperty("render_target", "");
                    CSetRenderTargetSceneRendererCommand *l_UnsetRenderTarget = (CSetRenderTargetSceneRendererCommand *)m_SceneRendererCommands.GetResourcesMap().find(l_CommandName)->second.m_Value;
                    //	GetSetRenderTargetSceneRendererCommand(l_UnsetRenderTarget);
                    if (l_UnsetRenderTarget != NULL) {
                        CUnsetRenderTargetSceneRendererCommand *l_Command = new CUnsetRenderTargetSceneRendererCommand(l_UnsetRenderTarget, m(i));
                        m_SceneRendererCommands.AddResource(l_Name, l_Command);
                    }
                }
                if (name == "enable_z_test") {
                    CEnableZTestSceneRendererCommand *l_Command = new CEnableZTestSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "disable_z_test") {
                    CDisableZTestSceneRendererCommand *l_Command = new CDisableZTestSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
                if (name == "render_debug_info") {
                    CRenderDebugInfoSceneRendererCommand *l_Command = new CRenderDebugInfoSceneRendererCommand(m(i));
                    m_SceneRendererCommands.AddResource(l_Name, l_Command);
                }
				if (name == "particle_render") {
					CParticleRendererCommand *l_Command = new CParticleRendererCommand(m(i));
					m_SceneRendererCommands.AddResource(l_Name, l_Command);
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
void CSceneRendererCommandManager::Execute(CGraphicsManager& RM)
{
    for (int i = 0; i < m_SceneRendererCommands.GetResourcesVector().size(); ++i) {
        if (m_needReload) {
            m_SceneRendererCommands.GetResourcesVector().at(i)->Reload();
        }
        m_SceneRendererCommands.GetResourcesVector().at(i)->Execute(RM);
    }
    if (m_needReload)
        m_needReload = false;
}

void CSceneRendererCommandManager::Reload()
{
    CleanUp();
    Load(m_FileName);
}