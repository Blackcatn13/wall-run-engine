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
#include "SceneElements\Switch.h"
#include "SceneElements\SceneElement.h"
#include "SceneElements\Door.h"
#include "SceneElements\WallTrap.h"


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

void RegisterSceneElements() {
  luabind::module(LUA_STATE) [
    class_<CSceneElement, CMeshInstance>("CSceneElement")
    .def(constructor < std::string , std::string, bool>())
    .def("insert_phisic", &CSceneElement::InsertPhisic)
    .property("m_Actor", &CSceneElement::GetActor)
    .property("m_PhysicsSize", &CSceneElement::GetPhysicsSize)
    .property("m_FirstTime", &CSceneElement::getFirstTime, &CSceneElement::setFirstTime)
    .def("activate_phisic", &CSceneElement::ActivatePhisic)
    .def("is_around", &CSceneElement::isAround)
    .def("get_fsm", &CSceneElement::getFsm)
    .def("is_inside", &CSceneElement::isInside)

  ];

  luabind::module(LUA_STATE) [
    class_<CStaticPlatform, CSceneElement>("CStaticPlatform")
    .def(constructor<std::string, std::string>())
    /* .def("insert_platform", &CStaticPlatform::InsertPlatform)
     .property("m_PlatformActor", &CStaticPlatform::GetPlatformActor )*/
  ];
  luabind::module(LUA_STATE) [
    class_<CBreakablePlatform, CStaticPlatform>("CBreakablePlatform")
    .def(constructor<std::string, std::string, std::string>())
    .def("disable_platform", &CBreakablePlatform::DisablePlatform)
    .def("enable_platform", &CBreakablePlatform::EnablePlatform)
    .property("m_EmittersSet", &CBreakablePlatform::getEmittersSet, &CBreakablePlatform::setEmittersSet)
  ];
  luabind::module(LUA_STATE) [
    class_<CMovingPlatform, CStaticPlatform>("CMovingPlatform")
    .def(constructor<std::string, std::string, float>())
    .def("move_to_point", &CMovingPlatform::MoveToPoint)
    .def("get_next_wp", &CMovingPlatform::GetNextWP)
    .def("add_box_controller", &CMovingPlatform::AddBoxController)
    .property("m_NextWP",  &CMovingPlatform::GetNextWPVector, &CMovingPlatform::SetNextWPVector)
    .property("m_Activated",  &CMovingPlatform::getActivated, &CMovingPlatform::setActivated)
    .property("m_Direction",  &CMovingPlatform::getDirection, &CMovingPlatform::setDirection)
    .property("m_Speed",  &CMovingPlatform::getSpeed, &CMovingPlatform::setSpeed)
  ];

  luabind::module(LUA_STATE) [
    class_<CPinchosPlatform, CBreakablePlatform>("CPinchosPlatform")
    .def(constructor<std::string, std::string, std::string/*, Vect3f, Vect3f, bool, bool*/>())
    .def("falling_into_platform", &CPinchosPlatform::FallingIntoPlatform)
    /*  .property("m_BackPos",  &CPinchosPlatform::getBackPos, &CPinchosPlatform::setBackPos)
      .property("m_FrontPos",  &CPinchosPlatform::getFrontPos, &CPinchosPlatform::setFrontPos)
      .property("m_FromZ",  &CPinchosPlatform::getFromZ, &CPinchosPlatform::setFromZ)
      .property("m_FromX",  &CPinchosPlatform::getFromX, &CPinchosPlatform::setFromX)*/
  ];
  luabind::module(LUA_STATE) [
    class_<CPolyPlatform, CStaticPlatform>("CPolyPlatform")
    .def(constructor<std::string , std::string ,  Vect3f , Vect3f,  float, float, float>())
    .def("activate_poly", &CPolyPlatform::ActivatePoly)
    .def("deactivate_poly", &CPolyPlatform::DeactivatePoly)
    .def("apply_physics_to_player", &CPolyPlatform::ApplyPhysicsToPlayer)
    .property("m_Activated", &CPolyPlatform::IsActivated, &CPolyPlatform::SetActivated )
    .property("m_Direction", &CPolyPlatform::getDirection, &CPolyPlatform::setDirection )
    .property("m_OriginalPosition", &CPolyPlatform::getOriginalPosition, &CPolyPlatform::setOriginalPosition )
    .property("m_FinalPosition", &CPolyPlatform::getFinalPosition, &CPolyPlatform::setFinalPosition)
    .property("m_Speed", &CPolyPlatform::getSpeed, &CPolyPlatform::setSpeed)
    // .property("m_RedimScale", &CPolyPlatform::getRedimScale, &CPolyPlatform::setRedimScale )
    .property("m_Enabled", &CPolyPlatform::getEnabled, &CPolyPlatform::setEnabled )
    .property("m_ActivationDistance", &CPolyPlatform::getActivationDistance, &CPolyPlatform::setActivationDistance )
    .property("m_TimeOut", &CPolyPlatform::getTimeOut, &CPolyPlatform::setTimeOut )
    //.property("m_Collission", &CPolyPlatform::getCollission, &CPolyPlatform::setCollission )
    .property("m_IsMoving", &CPolyPlatform::getIsMoving, &CPolyPlatform::setIsMoving )
    .property("m_ActiveTime", &CPolyPlatform::getActiveTime, &CPolyPlatform::setActiveTime )
    .property("m_LightName", &CPolyPlatform::getLightName, &CPolyPlatform::setLightName)
    .property("m_Light", &CPolyPlatform::GetLight, &CPolyPlatform::SetLight)
    .property("m_LightOriginalPosition", &CPolyPlatform::getLightOriginalPosition, &CPolyPlatform::setLightOriginalPosition)
    // .property("m_RedimAxis", &CPolyPlatform::getRedimAxis, &CPolyPlatform::setRedimAxis )
  ];


  luabind::module(LUA_STATE) [
    class_<CSwitch, CSceneElement>("CSwitch")
    .def(constructor < std::string , std::string>())
    .property("m_Switched" , &CSwitch::getSwitched, &CSwitch::setSwitched)
  ];

  luabind::module(LUA_STATE) [
    class_<CDoor, CSceneElement>("CDoor")
    .def(constructor < std::string , std::string, std::string, Vect3f>())
    .property("m_AnimationDone", &CDoor::getAnimationDone, &CDoor::setAnimationDone)
    .property("m_FinalPosition", &CDoor::getFinalPosition, &CDoor::setFinalPosition)
    .property("m_OriginalPosition", &CDoor::getOriginalPosition, &CDoor::setOriginalPosition)
    .property("m_IsPlayingAnimation", &CDoor::getIsPlayingAnimation, &CDoor::setIsPlayingAnimation)
    .property("m_IsOpening", &CDoor::getIsOpening, &CDoor::setIsOpening)
  ];

  luabind::module(LUA_STATE) [
    class_<CWallTrap, CStaticPlatform>("CWallTrap")
    .def(constructor<std::string, std::string, float, float, std::string>())
    .def("move_to_point", &CWallTrap::MoveToPoint)
    .property("m_Activated",  &CWallTrap::getActivated, &CWallTrap::setActivated)
    .property("m_SpeedClosing",  &CWallTrap::getSpeedClosing, &CWallTrap::setSpeedClosing)
    .property("m_SpeedOpening",  &CWallTrap::getSpeedOpening, &CWallTrap::setSpeedOpening)
    .property("m_Side",  &CWallTrap::getSide, &CWallTrap::setSide)
    .property("m_TimeOut",  &CWallTrap::getTimeOut, &CWallTrap::setTimeOut)
    .property("m_CurrentTime",  &CWallTrap::getCurrentTime, &CWallTrap::setCurrentTime)
    .property("m_FinalPosition",  &CWallTrap::getFinalPosition, &CWallTrap::setFinalPosition)
    .property("m_OriginalPosition",  &CWallTrap::getOriginalPosition, &CWallTrap::setOriginalPosition)

  ];
}

//#endif