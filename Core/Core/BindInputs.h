/*#ifndef BINDINPUTS_H
#define BINDINPUTS_H*/
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"
#include "InputManager.h"
#include "ActionToInput.h"

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


void RegisterInputs()
{
    luabind::module(LUA_STATE) [
        class_<CActionToInput>("CActionToInput")
        .def(constructor<CInputManager *>())
        .def("load_xml", &CActionToInput::LoadXML)
        .def("reload_xml", &CActionToInput::ReloadXML)
        .def("init_string_2_Input", &CActionToInput::InitString2Input)
        .def("do_action", (bool (CActionToInput::*)(const std::string & action_name, float & amount)) &CActionToInput::DoAction)
        .def("do_action", (bool (CActionToInput::*)(const std::string & action_name)) &CActionToInput::DoAction)
        .def("do_action_from_lua", (bool (CActionToInput::*)(const std::string action_name, float amount)) &CActionToInput::DoActionFromLua)
        .def("do_action_from_lua", (bool (CActionToInput::*)(const std::string action_name)) &CActionToInput::DoActionFromLua)
    ];
}