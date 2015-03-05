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

void RegisterCore()
{
    luabind::module(LUA_STATE) [
        class_<CCore>("CCore")
//	.def(constructor<>())
        .def("update", &CCore::Update)
        .def("de_init", &CCore::DeInit)
        .def("get_instance", &CCore::GetInstance)
        .def("get_graphics_manager", &CCore::GetGraphicsManager)
        .def("get_sound_manager", &CCore::GetSoundManager)
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
        //.def("get_renderable_object_layer_manager", &CCore::GetRenderableObjectsLayersManager)
        .def("get_phisics_manager", &CCore::GetPhysicsManager)
        //.def("get_scene_render_commands_manager", &CCore::GetSceneRendererCommandManager)
        .def("get_trigger_manager", &CCore::GetTriggerManager)
		.def("trace", &CCore::Trace)
        // .def("get_log_render", &CCore::GetLogRender)
        .property("m_CameraController", &CCore::GetCameraController, &CCore::SetCameraController)
    ];
    luabind::module(LUA_STATE) [
        class_<CCoreLuaWrapper>("CCoreLuaWrapper")
        .def(constructor<>())
        .property("m_CoreInstance", &CCoreLuaWrapper::GetCoreInstance)
    ];
}

//#endif