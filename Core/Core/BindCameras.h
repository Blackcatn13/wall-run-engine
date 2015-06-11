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
#include "Utils\MapManager.h"

using namespace luabind;

void RegisterCameraController() {
  //Camera
  luabind::module(LUA_STATE) [
    class_< CCamera>("CCamera")
    .enum_("ETypeCamera")
    [
      value("TC_FPS" , 0),
      value("TC_THPS", 1),
      value("TC_ESF", 2),
      value("TC_CIN", 3),
      value("TC_ORTHFIX", 4),
      value("TC_2DCAM", 5),
      value("TC_3DCAM", 6)
    ]
    //	.def(constructor<float, float, float, float, CObject3D, ETypeCamera>())
    //    .def(constructor<>())
    .property("m_fView_d", & CCamera::GetViewD)
    .property("m_pObject3D", & CCamera::GetObject3D, & CCamera::SetObject3D )
    .property("m_fZFar", & CCamera::GetZf, &CCamera::SetZf)
    .property("m_fZNear", & CCamera::GetZn, &CCamera::SetZn)
    .property("m_fFOV", & CCamera::GetFov, & CCamera::SetFov )
    .property("m_fAspectRatio", & CCamera::GetAspectRatio, & CCamera::SetAspectRatio)
    .property("m_eTypeCamera", & CCamera::GetTypeCamera, & CCamera::SetTypeCamera)
    .def("add_zf", &CCamera::AddZf)
    .def("add_zn", &CCamera::AddZn)
    .def("add_fov", &CCamera::AddFov)
    .def("add_view_d", &CCamera::AddViewD)
    //.def("set_type_camera", &CCamera::SetTypeCamera)
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
    .property("m_path", & CThPSCamera::GetMPath)
	.property("m_pathSpecs", & CThPSCamera::GetMPathSpecs)
    .property("m_lastPlayerPos", & CThPSCamera::GetLastPlayerPos, & CThPSCamera::SetLastPlayerPos)
    .property("m_currentWaypoint", & CThPSCamera::GetCurrentWaypoint, & CThPSCamera::SetCurrentWaypoint)
    .property("m_nextWaypoint", & CThPSCamera::GetNextWaypoint, & CThPSCamera::SetNextWaypoint)
    .def("get_path_size", & CThPSCamera::GetPathSize)
    .def("get_path_point", & CThPSCamera::GetPathPoint)
	.def("get_path_point_specs", & CThPSCamera::GetPathPointSpecs)
    .def("get_direction", & CThPSCamera::GetDirection)
    .def("get_look_at", & CThPSCamera::GetLookAt)
    .def("Get_eye", & CThPSCamera::GetEye)
    .def("Get_vec_up", & CThPSCamera::GetVecUp)
    .def("set_zoom", & CThPSCamera::SetZoom)
    .def("add_zoom", & CThPSCamera::AddZoom)
    .def("get_zoom", & CThPSCamera::GetZoom)
  ];
  luabind::module(LUA_STATE) [
    class_<CMapManager<CCamera>>("CMapManagerCamera")
    .def("get_resource", &CMapManager< CCamera >::GetResource)
    .def("existe_resource", &CMapManager< CCamera >::ExisteResource)
    .def("add_resource", &CMapManager< CCamera >::AddResource)
    .def("destroy", &CMapManager< CCamera >::Destroy)
    .property("m_Resources",  &CMapManager< CCamera >::GetResources)
  ];
  //Camera Controller
  luabind::module(LUA_STATE) [
    class_< CCameraController, CMapManager<CCamera>>("CCameraController")
    .def(constructor<>())
    .property("m_FlyMode", & CCameraController::GetFlyMode, & CCameraController::SetFlyMode)
    .property("m_Speed", & CCameraController::GetSpeed, & CCameraController::SetSpeed)
    .property("m_LittleZoom", & CCameraController::GetLittleZoom, & CCameraController::SetLittleZoom)
    .property("m_BigZoom", & CCameraController::GetBigZoom, & CCameraController::SetBigZoom)
    .property("m_PanSpeed", & CCameraController::GetPanSpeed, & CCameraController::SetPanSpeed)
    .def("add_new_camera", & CCameraController::AddNewCamera)
    .def("add_new_object", & CCameraController::AddNewObject)
    .def("update", (void (CCameraController::*)(float dt)) & CCameraController::Update)
    .def("update", (void (CCameraController::*)(std::string, float)) & CCameraController::Update)
    .def("get_active_camera", & CCameraController::getActiveCamera)
    .def("get_camera_name", & CCameraController::GetCameraName)
    .def("set_active_camera", (void (CCameraController::*)(CCamera * cam)) & CCameraController::setActiveCamera)
    .def("set_active_camera", (void (CCameraController::*)(std::string cam)) & CCameraController::setActiveCamera)
    //CINEMATICS
    .def("load", & CCameraController::Load)
    .def("reload", (bool (CCameraController::*)()) & CCameraController::Reload)
    .def("reload", (bool (CCameraController::*)(const std::string &)) & CCameraController::Reload)
    .def("is_any_cinematic-playing", & CCameraController::IsAnyCinematicPlaying)
    .def("play", & CCameraController::Play)
    .def("pause", & CCameraController::Pause)
    .def("stop", & CCameraController::Stop)
    .def("cam_updates", & CCameraController::CamUpdates)
  ];
    luabind::module(LUA_STATE) [
    class_< PathPointSpec>("PathPointSpec")
		.def(constructor<>())
		.def_readwrite("can_rotate", &PathPointSpec::canRotate)
		.def_readwrite("can_go_back", &PathPointSpec::canGoBack)
	];
	

}

#endif
