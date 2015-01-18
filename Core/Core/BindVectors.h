/*#ifndef BINDCORE_H
#define BINDCORE_H*/
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
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

void RegisterVectors()
{
	luabind::module(LUA_STATE) [
        class_<Vect3f>("Vect3f") 
        .def(constructor<float, float, float>())
        .def(constructor<>())
        .def(constructor<Vect3f>())
      
        .def(const_self + const_self)
        .def(const_self - const_self)
        .def(const_self * const_self)
        .def(const_self / float())
		.def(const_self * float())
		.def(const_self + float())
		.def(const_self - float())
        .def(const_self == const_self)
        .def(const_self ^ const_self)
        // Operadores y funciones de asignación
        .def("set",  &Vect3f::Set)
        .def("set_zero",  &Vect3f::SetZero)
        // Coordenadas polares
        .def("set_from_polar", &Vect3f::SetFromPolar)
        .def("get_polar",  &Vect3f::GetPolar)
        //funciones de comparacion
        .def("is_equal_epsilon",  &Vect3f::IsEqualEpsilon)
        .def("is_not_equal_epsilon",  &Vect3f::IsNotEqualEpsilon)
        // Producto por componentes (escalado)
        .def("scale",  &Vect3f::Scale)
        .def("get_scaled",  &Vect3f::GetScaled)
        // Establecimiento condicional
        .def("set_if_min_components",  &Vect3f::SetIfMinComponents)
        .def("set_if_max_components",  &Vect3f::SetIfMaxComponents)
        //Proyecciones
        .def("get_proj_xy",  &Vect3f::GetProjXY)
        .def("get_proj_yz",  &Vect3f::GetProjYZ)
        .def("get_proj_zx",  &Vect3f::GetProjZX)
        // Funciones de la longitud 
        .def("normalize",  &Vect3f::Normalize)
        .def("get_normalized",  &Vect3f::GetNormalized)
        .def("length",  &Vect3f::Length)
        .def("squared_length",  &Vect3f::SquaredLength)
        .def("distance",  &Vect3f::Distance)
        .def("sq_distance",  &Vect3f::SqDistance)
        // Rotaciones en los ejes principales
        .def("rotate_x",  &Vect3f::RotateX)
        .def("get_rotated_x",  &Vect3f::GetRotatedX)
        .def("rotate_y",  &Vect3f::RotateY)
        .def("get_rotated_y",  &Vect3f::GetRotatedY)
        .def("rotate_z",  &Vect3f::RotateZ)
        .def("get_rotated_z",  &Vect3f::GetRotatedZ)
        .def("get_angle_x",  &Vect3f::GetAngleX)
        .def("get_angle_y",  &Vect3f::GetAngleY)
        .def("get_angle_z",  &Vect3f::GetAngleZ)
        .def("get_angles",  &Vect3f::GetAngles)
         // Interpolación lineal
        .def("lerp",  &Vect3f::Lerp)
        .def("get_lerp",  &Vect3f::GetLerp)
        // cosas externas (funciones, constantes...) no se registran?*/
        //Variables
        .def_readwrite("x", &Vect3f::x)
        .def_readwrite("y", &Vect3f::y)
        .def_readwrite("z", &Vect3f::z)
        ];

	luabind::module(LUA_STATE) [
        class_<Vect3i>("Vect3i") 
        .def(constructor<int, int, int>())
        .def(constructor<>())
        .def(constructor<Vect3i>())
      
        .def(const_self + const_self)
        .def(const_self - const_self)
        .def(const_self * const_self)
        .def(const_self / int())
		.def(const_self * int())
		.def(const_self + int())
		.def(const_self - int())
        .def(const_self == const_self)
        .def(const_self ^ const_self)
        // Operadores y funciones de asignación
        .def("set",  &Vect3i::Set)
        .def("set_zero",  &Vect3i::SetZero)
        //// Coordenadas polares
       // .def("set_from_polar", &Vect3i::SetFromPolar)
       // .def("get_polar",  &Vect3i::GetPolar)
        ////funciones de comparacion
        .def("is_equal_epsilon",  &Vect3i::IsEqualEpsilon)
        .def("is_not_equal_epsilon",  &Vect3i::IsNotEqualEpsilon)
        //// Producto por componentes (escalado)
        .def("scale",  &Vect3i::Scale)
        .def("get_scaled",  &Vect3i::GetScaled)
        // Establecimiento condicional
        .def("set_if_min_components",  &Vect3i::SetIfMinComponents)
        .def("set_if_max_components",  &Vect3i::SetIfMaxComponents)
        ////Proyecciones
        .def("get_proj_xy",  &Vect3i::GetProjXY)
        .def("get_proj_yz",  &Vect3i::GetProjYZ)
        .def("get_proj_zx",  &Vect3i::GetProjZX)
        // Funciones de la longitud 
       // .def("normalize",  &Vect3i::Normalize)
        //.def("get_normalized",  &Vect3i::GetNormalized)
        //.def("length",  &Vect3i::Length)
        //.def("squared_length",  &Vect3i::SquaredLength)
        //.def("distance",  &Vect3i::Distance)
        //.def("sq_distance",  &Vect3i::SqDistance)*/
        //// Rotaciones en los ejes principales
        /*.def("rotate_x",  &Vect3i::RotateX)
        .def("get_rotated_x",  &Vect3i::GetRotatedX)
        .def("rotate_y",  &Vect3i::RotateY)
        .def("get_rotated_y",  &Vect3i::GetRotatedY)
        .def("rotate_z",  &Vect3i::RotateZ)
        .def("get_rotated_z",  &Vect3i::GetRotatedZ)
        .def("get_angle_x",  &Vect3i::GetAngleX)
        .def("get_angle_y",  &Vect3i::GetAngleY)
        .def("get_angle_z",  &Vect3i::GetAngleZ)
        .def("get_angles",  &Vect3i::GetAngles)*/
        // // Interpolación lineal
        .def("lerp",  &Vect3i::Lerp)
        .def("get_lerp",  &Vect3i::GetLerp)
        // cosas externas (funciones, constantes...) no se registran?*/
        //Variables
        .def_readwrite("x", &Vect3i::x)
        .def_readwrite("y", &Vect3i::y)
        .def_readwrite("z", &Vect3i::z)
        ];
}

//#endif