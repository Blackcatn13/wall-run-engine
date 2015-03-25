//#ifndef BINDMANAGERS_H
//#define BINDMANAGERS_H
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"
#include "Math\Color.h"

#include <d3dx9.h>

#include "Math\Vector4.h"

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


void RegisterColor() {
  luabind::module(LUA_STATE) [
    class_<CColor>("CColor")
    .def(constructor<>())
    .def(constructor<float, float, float, float>())
    //.def(constructor<Vect4f>()) todo: registrara Vect4f
    .def("set_color", &CColor::Set)
    //TODO Registrar el resto si hace falta
  ];

}

//#endif