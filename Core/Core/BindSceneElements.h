/*#ifndef BINDCORE_H
#define BINDCORE_H*/
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"

#include "SceneElements\StaticPlatform.h"
#include "SceneElements\BreakablePlatform.h"
#include "SceneElements\MovingPlatform.h"

#include "Mesh\MeshInstance.h"
#include <string>
#include "Math\Vector3.h"

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

void RegisterSceneElements()
{
    luabind::module(LUA_STATE) [
        class_<CStaticPlatform, CMeshInstance>("CStaticPlatform")
        .def(constructor<std::string, std::string>())
        .def("insert_platform", &CStaticPlatform::InsertPlatform)
    ];
    luabind::module(LUA_STATE) [
        class_<CBreakablePlatform, CStaticPlatform>("CBreakablePlatform")
        .def(constructor<std::string, std::string, std::string>())
        .def("disable_platform", &CBreakablePlatform::DisablePlatform)
    ];
    luabind::module(LUA_STATE) [
        class_<CMovingPlatform, CStaticPlatform>("CMovingPlatform")
        .def(constructor<std::string, std::string, float>())
        .def("move_to_point", &CMovingPlatform::MoveToPoint)
        .def("get_next_wp", &CMovingPlatform::GetNextWP)
    ];
}

//#endif