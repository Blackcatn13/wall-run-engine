/*#ifndef BINDCORE_H
#define BINDCORE_H*/
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"

#include "SceneElements\StaticPlatform.h"
#include "SceneElements\BreakablePlatform.h"
#include "SceneElements\MovingPlatform.h"
#include "SceneElements\PinchosPlatform.h"
#include "SceneElements\PolyPlatform.h"

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
        .property("m_PlatformActor", &CStaticPlatform::GetPlatformActor )
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
        .def("add_box_controller", &CMovingPlatform::AddBoxController)
    ];
    luabind::module(LUA_STATE) [
        class_<CPinchosPlatform, CBreakablePlatform>("CPinchosPlatform")
        .def(constructor<std::string, std::string, std::string, float, bool, bool>())
        .def("falling_into_platform", &CPinchosPlatform::FallingIntoPlatform)
    ];
    luabind::module(LUA_STATE) [
        class_<CPolyPlatform, CStaticPlatform>("CPolyPlatform")
        .def(constructor<std::string, std::string, std::string,  float, float>())
        .def("activate_poly", &CPolyPlatform::ActivatePoly)
        .def("deactivate_poly", &CPolyPlatform::DeactivatePoly)
        .property("m_Activated", &CPolyPlatform::IsActivated, &CPolyPlatform::SetActivated )
        .property("m_OriginalScale", &CPolyPlatform::getOriginalScale, &CPolyPlatform::setOriginalScale )
        .property("m_RedimScale", &CPolyPlatform::getRedimScale, &CPolyPlatform::setRedimScale )
        .property("m_Enabled", &CPolyPlatform::getEnabled, &CPolyPlatform::setEnabled )
        .property("m_ActivationDistance", &CPolyPlatform::getActivationDistance, &CPolyPlatform::setActivationDistance )
        .property("m_TimeOut", &CPolyPlatform::getTimeOut, &CPolyPlatform::setTimeOut )
        // .property("m_InitialCollission", &CPolyPlatform::getInitialCollission, &CPolyPlatform::setInitialCollission )
        .property("m_RedimAxis", &CPolyPlatform::getRedimAxis, &CPolyPlatform::setRedimAxis )
    ];
}

//#endif