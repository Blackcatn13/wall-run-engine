/*#ifndef BINDCORE_H
#define BINDCORE_H*/
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"

#include "PlayerController.h"
#include "Math\Vector3.h"
#include "Math\Matrix33.h"
#include "Object\Object3D.h"
#include "Actor\PhysicActor.h"
#include "Actor\PhysicController.h"
#include "TriggerManager\Trigger.h"
#include "PhysicsManager.h"
#include "Cooking Mesh\PhysicCookingMesh.h"
#include "Utils\PhysicUserAllocator.h"
//#include "NxActor.h"
//#include "NxPhysicsSDK.h"
#include "AI\AIController.h"
#include "AI\WPManager.h"

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

void RegisterAI()
{
    luabind::module(LUA_STATE) [
        class_<CAIController, CObject3D>("CAIController")
        .def(constructor<>())
        .property("m_PhysicController", &CAIController::getPhysicController,  &CAIController::setPhysicController )
        .property("m_PhysicUserData", &CAIController::getPhysicUserData,  &CAIController::setPhysicUserData )
        .property("m_Gravity", &CAIController::getGravity,  &CAIController::setGravity )
        .property("m_Gravity", &CAIController::getGravity,  &CAIController::setGravity )
        .property("m_Speed", &CAIController::getSpeed,  &CAIController::setSpeed )
        //.property("m_TurnSpeed", &CAIController::getTurnSpeed,  &CAIController::setTurnSpeed )
        .property("m_JumpForce", &CAIController::getJumpForce,  &CAIController::setJumpForce )
        .property("m_CurrentJumpForce", &CAIController::getCurrentJumpForce,  &CAIController::setCurrentJumpForce )
        .property("m_isJumping", &CAIController::getisJumping,  &CAIController::setisJumping )
        .def("move", &CAIController::Move)
        .def("move_to", &CAIController::MoveTo)
    ];

	 luabind::module(LUA_STATE) [
		    class_<ZONE>("ZONE")
			.def_readwrite("m_Waypoints", &ZONE::m_Waypoints)
	   ];

	  luabind::module(LUA_STATE) [
        class_<CMapManager<ZONE>>("CMapManagerZONE")
        .def("get_resource", &CMapManager< ZONE >::GetResource)
        .def("existe_resource", &CMapManager< ZONE >::ExisteResource)
        .def("add_resource", &CMapManager< ZONE >::AddResource)
        .def("destroy", &CMapManager< ZONE >::Destroy)
        .property("m_Resources",  &CMapManager< ZONE >::GetResources)
    ];
	  

	  luabind::module(LUA_STATE) [
        class_<CWPManager, CMapManager<ZONE>>("CWPManager")
        .def(constructor<>())
        .def("find_closest_waypoint", &CWPManager::FindClosestWaypoint)
		.def("get_distance", &CWPManager::GetDistance)
		.def("get_waypoint_position", &CWPManager::GetWaypointPosition)
		.def("calcular_siguiente_waypoint", &CWPManager::CalcularSiguienteWaypoint)
	  ];

	
}

//#endif