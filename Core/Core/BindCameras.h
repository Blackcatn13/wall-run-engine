#ifndef BINDCAMERAS_H
#define BINDCAMERAS_H

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

#include "Camera\Camera.h"
#include "Camera\FPSCamera.h"
#include "Camera\ThPSCamera.h"
#include "Camera\CameraController.h"

using namespace luabind;

void RegisterCameraController()
{
	//Camera
		luabind::module(LUA_STATE) [
		class_< CCamera>("CCamera")
		.enum_("ETypeCamera")
		[
			value("TC_FPS" ,0),
			value("TC_THPS", 1),
			value("TC_ESF", 2)	
		]
	//	.def(constructor<float, float, float, float, CObject3D, ETypeCamera>()) 
    //    .def(constructor<>())
		.property("m_fView_d", & CCamera::GetViewD)
		.property("m_pObject3D", & CCamera::GetObject3D, & CCamera::SetObject3D )
		.property("m_fZFar", & CCamera::GetZf, &CCamera::SetZf)
		.property("m_fZNear", & CCamera::GetZn, &CCamera::SetZn)
		.property("m_fFOV", & CCamera::GetFov )
		.property("m_fView_d", & CCamera::GetViewD)
		.property("m_fAspectRatio", & CCamera::GetAspectRatio)
		.property("m_eTypeCamera", & CCamera::GetTypeCamera, & CCamera::SetTypeCamera)
		.def("add_zf", &CCamera::AddZf)
		.def("add_zn", &CCamera::AddZn)
		.def("add_fov", &CCamera::AddFov)
		.def("add_zf", &CCamera::AddZf)
		.def("add_view_d", &CCamera::AddViewD)
		];

		luabind::module(LUA_STATE) [
		class_< CFPSCamera, CCamera>("CFPSCamera")
		.def(constructor<float, float, float, float, CObject3D *>())
        .def(constructor<>())
        .def("get_direction", & CFPSCamera::GetDirection)
        .def("get_look_at", & CFPSCamera::GetLookAt)
		.def("Get_eye", & CFPSCamera::GetEye)
        .def("Get_vec_up", & CFPSCamera::GetVecUp)
		];

		luabind::module(LUA_STATE) [
		class_< CThPSCamera, CCamera>("CThPSCamera")
		.def(constructor<float, float, float, float, CObject3D *, float>())
        .def(constructor<>())
        .def("get_direction", & CThPSCamera::GetDirection)
        .def("get_look_at", & CThPSCamera::GetLookAt)
		.def("Get_eye", & CThPSCamera::GetEye)
        .def("Get_vec_up", & CThPSCamera::GetVecUp)
		.def("set_zoom", & CThPSCamera::SetZoom)
		.def("add_zoom", & CThPSCamera::AddZoom)
        .def("get_zoom", & CThPSCamera::GetZoom)
		];

		//Camera Controller
		luabind::module(LUA_STATE) [
		class_< CCameraController>("CCameraController")
        .def(constructor<>())
        .def("add_new_camera", & CCameraController::AddNewCamera)
        .def("add_new_object", & CCameraController::AddNewObject)
		.def("update", (void (CCameraController::*)(float dt)) & CCameraController::Update)
		.def("update", (void (CCameraController::*)(std::string, float)) & CCameraController::Update)
		.def("get_active_camera", & CCameraController::getActiveCamera)
		.def("set_active_camera", (void (CCameraController::*)(CCamera *cam)) & CCameraController::setActiveCamera)
		.def("set_active_camera", (void (CCameraController::*)(std::string cam)) & CCameraController::setActiveCamera)
        ];

}

#endif
