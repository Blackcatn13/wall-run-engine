#ifndef BINDCINEMATICS_H
#define BINDCINEMATICS_H

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"

#include "Object\Object3D.h"
#include "Renderable\RenderableObject.h"
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

//#include "Cinematics/CCinematicPlayer"
//#include "Cinematics/CCinematicObjectKeyFrame"
//#include "Cinematics/CCinematicObject"
//#include "Cinematics/CCinematic

using namespace luabind;

void RegisterCinematics()
{
//	luabind::module(LUA_STATE) [
//	class_<CCinematicPlayer>("CCinematicPlayer")
//	.def(constructor<>())
//	.def("init", & CCinematicPlayer::Init)
//	.def("update", & CCameraKeyController::Update)
//	.def("stop", & CCinematicPlayer::Stop)
//	.def("play", & CCinematicPlayer::Play)
//	.def("pause", & CCinematicPlayer::Pause)
//	.def("is_finished", & CCinematicPlayer::IsFinished)
//	.def("get_duration", & CCinematicPlayer::GetDuration)
//	.def("get_current_time", & CCinematicPlayer::GetCurrentTimes) //Cambiar por GetCurrentTime
//	.def("on_restart_cycle", & CCinematicPlayer::OnRestartCycle)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CCinematicObjectKeyFrame, CObject3D>("CCinematicObjectKeyFrame")
//	.def(constructor<CXMLTreeNode &>())
//	.def("get_key_frame_time", & CCinematicObjectKeyFrame::GetKeyFrameTime)
//	.def("set_key_frame_time", & CCinematicObjectKeyFrame::SetKeyFrameTime)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CCinematicObject, CCinematicPlayer>("CCinematicObject")
//	.def(constructor<CXMLTreeNode &>())
//	.def("is_ok", & CCinematicObject::IsOk)
//	.def("add_cinematic_object_key_frame", & CCinematicObject::AddCinematicObjectKeyFrame)
//	.def("update", & CCinematicObject::Update)
//	.def("stop", & CCinematicObject::Stop)
//	.def("on_restart_cycle", & CCinematicObject::OnRestartCycle)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CCinematic, bases<CRenderableObject, CCinematicPlayer>>("CCinematic")
//	.def(constructor<CXMLTreeNode &>())
//	.def("stop", & CCinematic::Stop)
//	.def("play", & CCinematic::Play)
//	.def("pause", & CCinematic::Pause)
//	.def("load_xml", & CCinematic::LoadXML)
//	.def("add_cinematic_object", & CCinematic::AddCinematicObject)
//	.def("update", & CCinematic::Update)
//	.def("render", & CCinematic::Render)
//	];
}
#endif
