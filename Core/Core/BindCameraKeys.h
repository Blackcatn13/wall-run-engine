#ifndef BINDCAMERAKEYS_H
#define BINDCAMERAKEYS_H

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"

#include "Math\Vector3.h"
#include "XML\XMLTreeNode.h"

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

#include "Camera/Importer/CameraInfo.h"
#include "Camera/Importer/CameraKey.h"
#include "Camera/Importer/CameraKeyController.h"


using namespace luabind;

void RegisterCameras()
{
	luabind::module(LUA_STATE) [
	class_<CCameraInfo>("CCameraInfo")
	.def(constructor<>())
	.def(constructor<const Vect3f &, const Vect3f &, const Vect3f &, float, float, float >())
	.def(constructor<CXMLTreeNode>())
	.def_readwrite("nearPlane", &CCameraInfo::m_NearPlane)
	.def_readwrite("farPlane", &CCameraInfo::m_FarPlane)
	.def_readwrite("fov", &CCameraInfo::m_FOV)
	.def_readwrite("eye", &CCameraInfo::m_Eye)
	.def_readwrite("lookAt", &CCameraInfo::m_LookAt)
	.def_readwrite("up", &CCameraInfo::m_Up)
	];

	luabind::module(LUA_STATE) [
	class_<CCameraKey>("CCameraKey")
	.def(constructor<CCameraInfo *, float>())
	.def_readwrite("cameraInfo", &CCameraKey::m_CameraInfo)
	.def_readwrite("time", &CCameraKey::m_Time)
	];

	luabind::module(LUA_STATE) [
	class_<CCameraKeyController>("CCameraKeyController")
	.def(constructor<CXMLTreeNode &>())
	.def("update", & CCameraKeyController::Update)
	.def("set_current_time", & CCameraKeyController::SetCurrentTime)
	.def("reset_time", & CCameraKeyController::ResetTime)
	.def("is_cycle", & CCameraKeyController::IsCycle)
	.def("set_cycle", & CCameraKeyController::SetCycle)
	.def("is_reverse", & CCameraKeyController::IsReverse)
	.def("set_reverse", & CCameraKeyController::SetReverse)
	];

}

#endif
