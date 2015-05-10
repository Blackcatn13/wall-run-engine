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
#include "Enemy.h"
#include "EasyEnemy.h"
#include "AI\Puzzle.h"

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

void RegisterAI() {
  luabind::module(LUA_STATE) [
    class_<CAIController, CObject3D>("CAIController")
    .def(constructor<>())
    .def(constructor<std::string, std::string, Vect3f, float, float, float >())
    .property("m_PhysicController", &CAIController::getPhysicController,  &CAIController::setPhysicController )
    .property("m_PhysicUserData", &CAIController::getPhysicUserData,  &CAIController::setPhysicUserData )
    .property("m_Gravity", &CAIController::getGravity,  &CAIController::setGravity )
    .property("m_Gravity", &CAIController::getGravity,  &CAIController::setGravity )
    .property("m_Speed", &CAIController::getSpeed,  &CAIController::setSpeed )
    .property("m_TurnSpeed", &CAIController::getTurnSpeed,  &CAIController::setTurnSpeed )
    .property("m_JumpForce", &CAIController::getJumpForce,  &CAIController::setJumpForce )
    .property("m_CurrentJumpForce", &CAIController::getCurrentJumpForce,  &CAIController::setCurrentJumpForce )
    .property("m_isJumping", &CAIController::getisJumping,  &CAIController::setisJumping )
    .property("m_SpeedModified", &CAIController::getSpeedModified, &CAIController::setSpeedModified )
    .property("m_RenderableObject", &CAIController::getRenderableObject,  &CAIController::setRenderableObject )
    .property("m_isAlive", &CAIController::getisAlive,  &CAIController::setisAlive )
    .def("move", &CAIController::Move)
    .def("move_to", &CAIController::MoveTo)
    .def("rotate_or_move", &CAIController::RotateOrMove)
    .def("only_rotate", &CAIController::OnlyRotate)
    .def("rotate_yaw", &CAIController::RotateYaw)
    .def("actualizar_disparo", &CAIController::ActualizarDisparo)
    .def("actualizar_hitbox", &CAIController::ActualizarHitboxEnemigo)
    .def("get_angle_diff", &CAIController::getAngleDiff)
  ];

  luabind::module(LUA_STATE) [
    class_<CEnemy, CAIController>("CEnemy")
    .def(constructor<CXMLTreeNode>())
    .def(constructor<std::string, std::string, Vect3f, float, float, float , float >())
    //.property("m_Speed", &CEnemy::getSpeed, &CEnemy::setSpeed)
    .property("m_CurrentTime", &CEnemy::getCurrentTime, &CEnemy::setCurrentTime)
    .property("m_OriginalPosition", &CEnemy::GetOriginalPosition)
    .def("get_name",  &CEnemy::GetEnemyName)

    /* .def("update", &CEnemy::Update)
     .def("render", &CEnemy::Render)*/
  ];

  luabind::module(LUA_STATE) [
    class_<CEasyEnemy, CEnemy>("CEasyEnemy")
    .def(constructor<CXMLTreeNode>())
    .def(constructor<std::string, std::string, Vect3f, float, float, float , float >())
    .def("move_enemy", &CEasyEnemy::MoveEnemy)
    .def("get_next_wp", &CEasyEnemy::GetNextWp)
    // .def("init_wp_vector", &CEasyEnemy::InitWpVector)
    .property("m_CurrentWp", &CEasyEnemy::GetCurrentWp, &CEasyEnemy::SetCurrentWp)
    .property("m_AttackSpeed", &CEasyEnemy::GetAttackSpeed )
    .property("m_OriginalSpeed", &CEasyEnemy::GetOriginalSpeed )
    .property("m_Returning", &CEasyEnemy::getReturning, &CEasyEnemy::setReturning)
    .def("get_wp_vector_size", &CEasyEnemy::GetWPVectorSize )

    // .def("init", &CEasyEnemy::Init)
    //.def("update", &CEasyEnemy::Update)
    //.def("render", &CEasyEnemy::Render)
  ];


  luabind::module(LUA_STATE) [
    class_<CPuzzle>("CPuzzle")
    .def(constructor<>())
    .def(constructor<std::string, int >())
    .property("m_MinActivatedSwitches",  &CPuzzle::getMinActivatedSwitches, &CPuzzle::setMinActivatedSwitches )
    .property("m_ActivatedSwitches",  &CPuzzle::getActivatedSwitches, &CPuzzle::setActivatedSwitches )
    .property("m_Done",  &CPuzzle::getDone, &CPuzzle::setDone)

  ];




}

//#endif