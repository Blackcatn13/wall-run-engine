//#ifndef BINDMANAGERS_H
//#define BINDMANAGERS_H
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"
#include "InputManager.h"
#include "ActionToInput.h"
//#include "SoundManager.h"
#include "Font\FontManager.h"
#include "GraphicsManager.h"
#include "Language\LanguageManager.h"
#include "TriggerManager\TriggerManager.h"
#include "Enemies\EnemyManager.h"
#include "AI\WPManager.h"
#include "AI\PuzzleManager.h"

#include "StaticMeshes\StaticMeshManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"

#include <d3dx9.h>
#include "Math\Matrix44.h"
#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Math\Vector2.h"
#include "Math\MathTypes.h"
#include "WWSoundManager.h"
#include "GUI\GUIManager.h"
#include "Collectibles\CollectibleManager.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include <luabind/scope.hpp>


using namespace luabind;


void RegisterManagers()
{
    /*  luabind::module(LUA_STATE) [
        class_<CSoundManager>("CSoundManager")
        .def(constructor<>())
        .def("update", &CSoundManager::Update)
        .def("init", &CSoundManager::Init)
        .def("de_init", &CSoundManager::DeInit)
      ];*/
    luabind::module(LUA_STATE) [
        class_<CGraphicsManager>("CGraphicsManager")
        .def(constructor<>())
        .def("update", &CGraphicsManager::Update)
        .def("init", &CGraphicsManager::Init)
        .def("de_init", &CGraphicsManager::DeInit)
        .def("render", &CGraphicsManager::Render)
        .def("get_window_rect", &CGraphicsManager::GetWindowRect)
        .def("setup_matrices", &CGraphicsManager::SetupMatrices)
        .def("get_window_rect", &CGraphicsManager::GetWindowRect)
        /*.def("begin_rendering", &CGraphicsManager::BeginRendering)
        .def("end_rendering", &CGraphicsManager::EndRendering)*/
        .def("begin_rendering", &CGraphicsManager::BeginRenderCommand)
        .def("end_rendering", &CGraphicsManager::EndRenderCommand)
        //----Transform Functions-----------------------------------------------
        .def("get_window_rect", (void (CGraphicsManager::*)(D3DXMATRIX &)) &CGraphicsManager::SetTransform)
        .def("begin_rendering", (void (CGraphicsManager::*)(Mat44f &)) &CGraphicsManager::SetTransform)
        .def("draw_line", &CGraphicsManager::DrawLine)
        .def("draw_axis", &CGraphicsManager::DrawAxis)
        .def("draw_cube", &CGraphicsManager::DrawCube)
        .def("draw_box", &CGraphicsManager::DrawBox)
        .def("draw_grid", &CGraphicsManager::DrawGrid)
        .def("draw_plane", &CGraphicsManager::DrawPlane)
        .def("get_sphere", &CGraphicsManager::DrawSphere)
        .def("get_camera", &CGraphicsManager::DrawCamera)
        .def("get_quad2D", (void (CGraphicsManager::*)(const Vect2i &, uint32, uint32, ETypeAlignment, CColor)) &CGraphicsManager::DrawQuad2D)
        .def("get_quad2DT", (void (CGraphicsManager::*)(const Vect2i &, uint32, uint32, ETypeAlignment, CTexture *, ETypeFlip)) &CGraphicsManager::DrawQuad2D)
        .def("calculate_alignment", &CGraphicsManager::CalculateAlignment)
        //.def("enable_alpha_blend", &CGraphicsManager::EnableAlphaBlend)
        .def("disable_alpha_blend", &CGraphicsManager::DisableAlphaBlend)
        .def("enable_z_buffering", &CGraphicsManager::EnableZBuffering)
        .def("disable_z_buffering", &CGraphicsManager::DisableZBuffering)
        .def("is_sphere_visible", &CGraphicsManager::isSphereVisible)
        .def("draw_rectangle_2d", &CGraphicsManager::DrawRectangle2D)
        .def("get_ray", &CGraphicsManager::GetRay)
        .def("get_width_and_height", &CGraphicsManager::GetWidthAndHeight)
        .def("draw_capsule", &CGraphicsManager::DrawCapsule)
        .def("draw_cylinder", &CGraphicsManager::DrawCylinder)
        .def("enable_z_buffering", (void (CGraphicsManager::*)(const Vect3f &, const Vect3f &, const Vect3f &, float, float, CColor)) &CGraphicsManager::DrawQuad3D)
        .def("disable_z_buffering", (void (CGraphicsManager::*)(const Vect3f &, const Vect3f &, const Vect3f &, const Vect3f &, CColor)) &CGraphicsManager::DrawQuad3D)
        .def("get_is_GUI_displayed", &CGraphicsManager::GetIsGUIDisplayed)
        .def("set_is_GUI_displayed", &CGraphicsManager::SetIsGUIDisplayed)
    ];
    luabind::module(LUA_STATE) [
        class_<CFontManager>("CFontManager")
        .def(constructor<>())
        .def("done", &CFontManager::Done)
        .def("init", (bool (CFontManager::*)(CGraphicsManager *)) &CFontManager::Init)
        //.def("init", (void (CFontManager::*)()) &CFontManager::Init)
        .def("is_ok", &CFontManager::IsOk)
        .def("reload_ttfss", &CFontManager::ReloadTTFs)
        .def("load_ttfs", &CFontManager::LoadTTFs)
        .def("get_ttf_id", &CFontManager::GetTTF_Id)
        .def("create_font", &CFontManager::CreateFont)
        .def("draw_lua_text", &CFontManager::DrawLuaText)
        //.def("draw_default_text", &CFontManager::DrawDefaultText)
        //.def("draw_text", &CFontManager::DrawText)
        /*	.def("draw_literal", &CFontManager::DrawLiteral)
        	.def("size_x", &CFontManager::SizeX)
        	.def("size_y", &CFontManager::SizeY)
        	.def("get_default_text_size", &CFontManager::GetDefaultTextSize)
        	.def("get_text_size", &CFontManager::GetTextSize)
        	.def("get_litearl_size", &CFontManager::GetLiteralSize)*/
    ];
    luabind::module(LUA_STATE) [
        class_<CLanguageManager>("CLanguageManager")
        .def(constructor<>())
        .def("load_xmls", &CLanguageManager::LoadXMLs)
        .def("set_xml_file", &CLanguageManager::SetXmlFile)
        .def("get_literal", &CLanguageManager::GetLiteral)
        .def("set_current_language", &CLanguageManager::SetCurrentLanguage)
    ];
    luabind::module(LUA_STATE) [
        class_<CStaticMeshManager>("CStaticMeshManager")
        .def(constructor<>())
        .def("load", &CStaticMeshManager::Load)
        .def("reload", &CStaticMeshManager::Reload)
    ];
    luabind::module(LUA_STATE) [
        class_<CInputManager>("CInputManager")
        .def(constructor<>())
        .def("init", &CInputManager::Init)
        .def("done", &CInputManager::Done)
        .def("is_ok", &CInputManager::IsOk)
        .def("update", &CInputManager::Update)
        .def("get_position", &CInputManager::GetPosition)
        .def("get_mouse_delta", &CInputManager::GetMouseDelta)
        .def("is_down", &CInputManager::IsDown)
        .def("is_down_up", &CInputManager::IsDownUp)
        .def("is_up_down", &CInputManager::IsUpDown)
        .def("has_game_pad", &CInputManager::HasGamePad)
        .def("scan_2_ascii", &CInputManager::Scan2ascii)
        .def("get_game_pad_left_thumb_deflection", &CInputManager::GetGamePadLeftThumbDeflection)
        .def("get_game_pad_right_thumb_deflection", &CInputManager::GetGamePadRightThumbDeflection)
        .def("get_game_pad_delta_triggers", &CInputManager::GetGamePadDeltaTriggers)
        .def("set_game_pad_left_motor_speed", &CInputManager::SetGamePadLeftMotorSpeed)
        .def("set_game_pad_right_motor_speed", &CInputManager::SetGamePadRightMotorSpeed)
    ];
    luabind::module(LUA_STATE) [
        class_<CTemplatedVectorMapManager<CTrigger>>("CTemplatedVectorMapManagerTriggerManager")
        .def(constructor<>())
        .scope
        [
            class_<CTemplatedVectorMapManager<CTrigger>::CMapResourceValue>("CMapResourceValue")
            .def(constructor<>())
            .def_readwrite("m_Value", & CTemplatedVectorMapManager<CTrigger>::CMapResourceValue::m_Value)
            .def_readwrite("m_Id", & CTemplatedVectorMapManager<CTrigger>::CMapResourceValue::m_Id)
        ]
        //.def("remove_resource", &CTemplatedVectorMapManager<CRenderableObject>::RemoveResource) // <= m_Id identificador no declarado xq está en clase interna
        .def("get_resource", &CTemplatedVectorMapManager<CTrigger>::GetResource)
        .def("get_resource_by_id", &CTemplatedVectorMapManager<CTrigger>:: GetResourceById)
        .def("add_resource", &CTemplatedVectorMapManager<CTrigger>::AddResource)
        .def("destroy", &CTemplatedVectorMapManager<CTrigger>::Destroy)
        .def("get_resource_map", &CTemplatedVectorMapManager<CTrigger>::GetResourcesMap)
        .def("get_resource_vector", &CTemplatedVectorMapManager<CTrigger>::GetResourcesVector)
    ];
    luabind::module(LUA_STATE) [
        class_<CTriggerManager, bases<CTemplatedVectorMapManager<CTrigger>, CPhysicTriggerReport>>("CTriggerManager")
        .def(constructor<>())
        .def("clear", &CTriggerManager::Clear)
        .def("load_triggers", &CTriggerManager::LoadTriggers)
        .def("on_enter", &CTriggerManager::OnEnter)
        .def("on_leave", &CTriggerManager::OnLeave)
        .def("on_stay", &CTriggerManager::OnStay)
        .def("get_trigger_report", &CTriggerManager::getTriggerReport)
    ];
    luabind::module(LUA_STATE) [
        class_<CTemplatedVectorMapManager<CRenderableObjectsManager>>("CTemplatedVectorMapManagerROM")
        .def(constructor<>())
        .scope
        [
            class_<CTemplatedVectorMapManager<CRenderableObjectsManager>::CMapResourceValue>("CMapResourceValue")
            .def(constructor<>())
            .def_readwrite("m_Value", & CTemplatedVectorMapManager<CRenderableObjectsManager>::CMapResourceValue::m_Value)
            .def_readwrite("m_Id", & CTemplatedVectorMapManager<CRenderableObjectsManager>::CMapResourceValue::m_Id)
        ]
        //.def("remove_resource", &CTemplatedVectorMapManager<CRenderableObject>::RemoveResource) // <= m_Id identificador no declarado xq está en clase interna
        .def("get_resource", &CTemplatedVectorMapManager<CRenderableObjectsManager>::GetResource)
        .def("get_resource_by_id", &CTemplatedVectorMapManager<CRenderableObjectsManager>:: GetResourceById)
        .def("add_resource", &CTemplatedVectorMapManager<CRenderableObjectsManager>::AddResource)
        .def("destroy", &CTemplatedVectorMapManager<CRenderableObjectsManager>::Destroy)
        .def("get_resource_map", &CTemplatedVectorMapManager<CRenderableObjectsManager>::GetResourcesMap)
        .def("get_resource_vector", &CTemplatedVectorMapManager<CRenderableObjectsManager>::GetResourcesVector)
    ];
    luabind::module(LUA_STATE) [
        class_<CRenderableObjectsLayersManager, CTemplatedVectorMapManager<CRenderableObjectsManager>>("CRenderableObjectsLayersManager")
        .def(constructor<>())
        .def("get_default_renderable_object_manager", &CRenderableObjectsLayersManager::GetDefaultRenderableObjectManager)
        .def("load", &CRenderableObjectsLayersManager::Load)
        .def("get_renderable_objects_manager_by_str", &CRenderableObjectsLayersManager::GetRenderableObjectsManagerByStr)
        .def("deactivate_objects_by_layer", &CRenderableObjectsLayersManager::DeactivateObjectsByStr)
		.property("m_CurrentLayer", &CRenderableObjectsLayersManager::getCurrentLayer, &CRenderableObjectsLayersManager::setCurrentLayer)
    ];
    luabind::module(LUA_STATE) [
        class_<ZONE>("ZONE")
        .def_readwrite("m_Waypoints", &ZONE::m_Waypoints)
    ];
    luabind::module(LUA_STATE) [
        class_<CMapManager<ZONE>>("CMapManagerZONE")
        .def("get_resource", &CMapManager< ZONE >::GetResource)
        .def("existe_resource", &CMapManager< ZONE >::ExisteResource)
        .def("add_resource", &CMapManager< ZONE >::AddResource)
        .def("destroy", &CMapManager< ZONE >::Destroy)
        .property("m_Resources",  &CMapManager< ZONE >::GetResources)
    ];
    luabind::module(LUA_STATE) [
        class_<CWPManager, CMapManager<ZONE>>("CWPManager")
        .def(constructor<>())
        .def("find_closest_waypoint", &CWPManager::FindClosestWaypoint)
        .def("get_distance", &CWPManager::GetDistance)
        .def("get_waypoint_position", &CWPManager::GetWaypointPosition)
        .def("calcular_siguiente_waypoint", &CWPManager::CalcularSiguienteWaypoint)
    ];
    luabind::module(LUA_STATE) [
        class_<CEnemyManager>("CEnemyManager")
        .def(constructor<>())
        .def("get_instance", &CEnemyManager::GetInstance)
        .def("insert_enemy", &CEnemyManager::InsertEnemy)
        .def("get_enemy", &CEnemyManager::GetEnemy)
        .def("reload_enemies",  &CEnemyManager::ReloadEnemies)
		.def("get_direction_enemy", &CEnemyManager::GetClosestEnemyVector)
    ];
    luabind::module(LUA_STATE) [
        class_<CMapManager<CPuzzle>>("CMapManagerPuzzle")
        .def("get_resource", &CMapManager< CPuzzle >::GetResource)
        .def("existe_resource", &CMapManager< CPuzzle >::ExisteResource)
        .def("add_resource", &CMapManager< CPuzzle >::AddResource)
        .def("destroy", &CMapManager< CPuzzle >::Destroy)
        .property("m_Resources",  &CMapManager< CPuzzle >::GetResources)
    ];
    luabind::module(LUA_STATE) [
        class_<CPuzzleManager, CMapManager<CPuzzle>>("CPuzzleManager")
        .def(constructor<>())
    ];
    luabind::module(LUA_STATE) [
        class_<CWWSoundManager>("CWWSoundManager")
        .def(constructor<>())
        .def("SetListenerPosition", &CWWSoundManager::SetListenerPosition)
        .def("PlayEvent", &CWWSoundManager::PlayEvent)
        .def("SetSwitch", &CWWSoundManager::SetSwitch)
        .def("SetTrigger", &CWWSoundManager::SetTrigger)
        .def("RegisterGameObject", &CWWSoundManager::RegisterGameObject)
        .def("UnregisterGameObject", &CWWSoundManager::UnregisterGameObject)
        .def("SetState", &CWWSoundManager::SetState)
    ];
    luabind::module(LUA_STATE) [
        class_<CGUIManager>("CGUIManager")
        .def("push_windows", &CGUIManager::PushWindows)
        .def("pop_windows", &CGUIManager::PopWindows)
        .def("set_message_box", &CGUIManager::SetMessageBox)
        .def("set_image", &CGUIManager::SetImage)
        .def("get_active_image", &CGUIManager::GetImage)
        .def("play_image", &CGUIManager::PlayImage)
        .def("set_visible_mouse", &CGUIManager::SetVisiblePointerMouse)
        .def("get_is_displayed_console", &CGUIManager::GetIsDisplayedConsole)
        .def("set_is_displayed_console", &CGUIManager::SetIsDisplayedConsole)
        .def("get_is_displayed_heart", &CGUIManager::GetIsDisplayedHeart)
        .def("set_is_displayed_heart", &CGUIManager::SetIsDisplayedHeart)
        .def("get_is_displayed_pixelite", &CGUIManager::GetIsDisplayedPixelite)
        .def("set_is_displayed_pixelite", &CGUIManager::SetIsDisplayedPixelite)
        .def("set_visibility_message", &CGUIManager::SetVisibilityMessage)
        .def("get_is_displayed_poly_message", &CGUIManager::GetIsDisplayedPolyMessage)
        .def("set_is_displayed_poly_message", &CGUIManager::SetIsDisplayedPolyMessage)
        .def("get_first_poly_message_displayed", &CGUIManager::GetFirstPolyMessageDisplayed)
        .def("set_first_poly_message_displayed", &CGUIManager::SetFirstPolyMessageDisplayed)
        .def("get_is_displayed_unlock_message", &CGUIManager::GetIsDisplayedUnlock)
        .def("set_is_displayed_unlock_message", &CGUIManager::SetIsDisplayedUnlock)
        .def("get_position_x_element", &CGUIManager::GetPositionXPercentElement)
        .def("get_position_y_element", &CGUIManager::GetPositionYPercentElement)
        .def("set_position_element", &CGUIManager::SetPositionPercentElement)
        .def("get_count_heart", &CGUIManager::GetGUICountHeart)
        .def("set_count_heart", &CGUIManager::SetGUICountHeart)
        .def("get_count_pixelite", &CGUIManager::GetGUICountPixelite)
        .def("set_count_pixelite", &CGUIManager::SetGUICountPixelite)
        .def("get_num_pixelite", &CGUIManager::GetGUIPixeliteNumber)
        .def("set_num_pixelite", &CGUIManager::SetGUIPixeliteNumber)
		.def("get_is_gameover", &CGUIManager::GetIsGameOver)
        .def("set_is_gameover", &CGUIManager::SetIsGameOver)
        .def("get_dt", &CGUIManager::GetElapsedTime)
        .def("getSliderState", &CGUIManager::GetStateSlider)
        .def("getEditableTextBox", &CGUIManager::GetEditableTextBox)
        .def("setEditableTextBox", &CGUIManager::SetEditableTextBox)
        .def("getButtonCheckInRadioBox", &CGUIManager::GetButtonCheckInRadioBox)
        .property("m_sCurrentWindows", &CGUIManager::GetCurrentWindow)
    ];
    luabind::module(LUA_STATE) [
        class_<CCollectibleManager>("CCollectibleManager")
        .def("reset_collectibles", &CCollectibleManager::ResetCollectibles)
    ];
}

//#endif