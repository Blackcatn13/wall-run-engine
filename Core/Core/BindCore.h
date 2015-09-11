/*#ifndef BINDCORE_H
#define BINDCORE_H*/
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"
#include "Core\CoreLuaWrapper.h"
#include "PlayerController.h"
#include "Object\Object3D.h"


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

void RegisterCore() {
  luabind::module(LUA_STATE) [
    class_<CCore>("CCore")
//	.def(constructor<>())
    .def("update", &CCore::Update)
    .def("de_init", &CCore::DeInit)
    .def("get_instance", &CCore::GetInstance)
    .def("get_graphics_manager", &CCore::GetGraphicsManager)
    //.def("get_sound_manager", &CCore::GetSoundManager)
    .def("get_input_manager", &CCore::GetInputManager)
    .def("get_language_manager", &CCore::GetLanguageManager)
    .def("get_font_manager", &CCore::GetFontManager)
    .def("get_handler", &CCore::getHandler)
    .def("get_config", &CCore::getConfig)
    .def("get_action_to_input", &CCore::GetActionToInput)
    .def("get_static_mesh_manager", &CCore::GetStaticMeshManager)
    .def("get_renderable_manager", &CCore::GetRenderableManager)
    .def("get_animanted_model_manager", &CCore::GetAnimatedModelManager)
    .def("get_texture_manager", &CCore::GetTextureManager)
    .def("get_script_manager", &CCore::GetScriptManager)
    .def("get_light_manager", &CCore::GetLightManager)
    .def("get_cinematic_controller", &CCore::GetCinematicController)
    .def("get_player_controller", &CCore::GetPlayerController)
    .def("get_renderable_object_layer_manager", &CCore::GetRenderableObjectsLayersManager)
    .def("get_phisics_manager", &CCore::GetPhysicsManager)
    .def("get_scene_render_commands_manager", &CCore::GetSceneRendererCommandManager)
    .def("get_trigger_manager", &CCore::GetTriggerManager)
    .def("get_enemy_manager", &CCore::GetEnemyManager)
    .def("get_puzzle_manager",  &CCore::GetPuzzleManager)
//        .def("get_wp_manager", &CCore::GetWPManager)
    .def("trace", &CCore::Trace)
    .def("set_lights_to_platforms", &CCore::SetLightsToPlatforms)
    .def("getWWSoundManager", &CCore::GetWWSoundManager)
    .def("getGUIManager", &CCore::GetGuiManager)
    .def("getCollectibleManager", &CCore::GetCollectibleManager)
    .def("getParticleManager", &CCore::GetParticleManager)
    // .def("get_log_render", &CCore::GetLogRender)
    .def("exit_game", &CCore::ExitGame)
    .def("get_billboard_manager", &CCore::GetBillboardManager)
    .def("getTickCounter", &CCore::GetTickCounter)
    .property("m_CameraController", &CCore::GetCameraController, &CCore::SetCameraController)
    .property("m_ElapsedTime", &CCore::GetElapsedTime)
    //.property("m_PlatformsMap", &CCore::GetPlatformsMap)
//#ifdef _PARTICLEVIEWER
    .def("copy_to_clipboard", &CCore::CopyToClipboard)
//#endif
  ];
  luabind::module(LUA_STATE) [
    class_<CCoreLuaWrapper>("CCoreLuaWrapper")
    .def(constructor<>())
    .property("m_CoreInstance", &CCoreLuaWrapper::GetCoreInstance)
  ];
}

//#endif