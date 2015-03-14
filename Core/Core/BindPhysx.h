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
#include "Utils\PhysicUserData.h"
#include "Utils\Named.h"
#include <string>
#include "PhysicsDefs.h"
#include "Reports\PhysicTriggerReport.h"
#include "NxPhysics.h"


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

void RegisterPhysX()
{
    luabind::module(LUA_STATE) [
        class_<CPhysicUserData, CNamed>("CPhysicUserData")
        .def(constructor<std::string>())
        .property("m_bPaintPhysicObject", &CPhysicUserData::GetPaint,  &CPhysicUserData::SetPaint )
        .property("m_ColorPhysicObject", &CPhysicUserData::GetColor,  &CPhysicUserData::SetColor )
        .property("_pEntity", &CPhysicUserData::GetEntity,  &CPhysicUserData::SetEntity )
        .property("m_pActor", &CPhysicUserData::GetActor,  &CPhysicUserData::SetActor )
        .property("m_pController", &CPhysicUserData::GetController,  &CPhysicUserData::SetController )
        .property("m_fRadius", &CPhysicUserData::GetRadius,  &CPhysicUserData::SetRadius )
        .property("m_uiCollisionGroup", &CPhysicUserData::GetGroup,  &CPhysicUserData::SetGroup )
        .property("m_myCollisionGroup", &CPhysicUserData::GetMyCollisionGroup,  &CPhysicUserData::SetMyCollisionGroup )
        .property("m_bPaintPhysicObject", &CPhysicUserData::SetPaint,  &CPhysicUserData::GetPaint )
        .property("m_ColorPhysicObject", &CPhysicUserData::GetColor,  &CPhysicUserData::SetColor )
        .property("m_pEntity", &CPhysicUserData::GetEntity,  &CPhysicUserData::SetEntity )
        .property("m_pActor", &CPhysicUserData::GetActor,  &CPhysicUserData::SetActor )
        .property("m_pController", &CPhysicUserData::SetController,  &CPhysicUserData::GetController )
        .property("m_ColorPhysicObject", &CPhysicUserData::GetColor,  &CPhysicUserData::SetColor )
        .property("m_pEntity", &CPhysicUserData::GetEntity,  &CPhysicUserData::SetEntity )
        .property("m_pActor", &CPhysicUserData::GetActor,  &CPhysicUserData::SetActor )
        .property("m_uiCollisionGroup",  &CPhysicUserData::GetGroup,  &CPhysicUserData::SetGroup )
        .property("m_myCollisionGroup",  &CPhysicUserData::GetMyCollisionGroup,  &CPhysicUserData::SetMyCollisionGroup )
    ];
    luabind::module(LUA_STATE) [
        class_<CPhysicActor>("CPhysicActor")
        //.def(constructor<>())
        .def(constructor<CPhysicUserData*>())
        //.def(constructor<NxActor>())
        .def("create_body", &CPhysicActor::CreateBody)
        .def("create_box_trigger", &CPhysicActor::CreateBoxTrigger)
        .def("create_sphere_trigger", &CPhysicActor::CreateSphereTrigger)
        .def("activate_all_triggers", &CPhysicActor::ActivateAllTriggers)
        //  .def("create_actor", &CPhysicActor::CreateActor)
        //---- Functions ---------------------------------------
        .def("add_impulse_at_pos", &CPhysicActor::AddImpulseAtPos)
        .def("add_velocity_at_pos", &CPhysicActor::AddVelocityAtPos)
        .def("add_aceleration_at_pos", &CPhysicActor::AddAcelerationAtPos)
        .def("add_force_at_pos", (void (CPhysicActor::*)(const Vect3f &, const Vect3f &, float, bool))  &CPhysicActor::AddForceAtPos)
        .def("add_torque", &CPhysicActor::AddTorque)
        //---AddShape Functions-----
        .def("add_shpere_shape", &CPhysicActor::AddSphereShape)
        .def("add_box_shape", &CPhysicActor::AddBoxSphape)
        .def("add_capsule_shape", &CPhysicActor::AddCapsuleShape)
        .def("add_mesh_shape", &CPhysicActor::AddMeshShape)
        .def("add_planet_shape", &CPhysicActor::AddPlaneShape)
        //---Activate
        .def("activate", &CPhysicActor::Activate)
        .def("set_kinematic", &CPhysicActor::SetKinematic)
        //---- Properties ( get & Set )----
        .def("get_user_data", &CPhysicActor::GetUserData)
        .property("m_pUserData", &CPhysicActor::GetUserData )
        .def("set_linear_velocity", &CPhysicActor::SetLinearVelocity)
        .def("get_linear_velocity", &CPhysicActor::GetLinearVelocity)
        .def("get_position", &CPhysicActor::GetPosition)
        .def("get_rotation", &CPhysicActor::GetRotation)
        .def("set_rotation", (void (CPhysicActor::*)(const Vect3f&)) &CPhysicActor::SetRotation)
        .def("set_rotation", (void (CPhysicActor::*)(const Mat33f&)) &CPhysicActor::SetRotation)
        .def("set_angular_velocity", &CPhysicActor::SetAngularVelocity)
        .def("get_angular_velocity", &CPhysicActor::GetAngularVelocity)
        .def("se_global_position", &CPhysicActor::SetGlobalPosition)
        .def("move_global_position", &CPhysicActor::MoveGlobalPosition)
        .def("get_angular_momentum", &CPhysicActor::GetAngularMomentum)
        .def("get_inertia_tensor", &CPhysicActor::GetInertiaTensor)
        .property("m_uCollisionGroups", &CPhysicActor::GetColisionGroup, &CPhysicActor::SetCollisionGroup )
        .def("get_phx_colision_group", &CPhysicActor::GetPhXColisionGroup)
        .property("m_TriggerGroup", &CPhysicActor::GetTriggerGroup, &CPhysicActor::SetTriggerGroup )
        //---Get Info-------
        .def("get_mat44", &CPhysicActor::GetMat44)
        .def("set_mat44", &CPhysicActor::SetMat44)
        .def("move_global_pose_mat44", &CPhysicActor::MoveGlobalPoseMat44)
        //---Get PhsX Info---
        //   .property("m_pPhXActor",  &CPhysicActor::GetPhXActor)
        // .property("m_pPhXActorDesc",  &CPhysicActor::GetActorDesc)
        //---Get Info-------
        .def("set_actor_solver_iteration_count", &CPhysicActor::SetActorSolverIterationCount)
        .def("set_contact_report_flags", &CPhysicActor::SetContactReportFlags)
        .def("set_contact_report_threshold", &CPhysicActor::SetContactReportThreshold)
        .def("destroy", &CPhysicActor::Destroy)
        //---Set Material -------
        .def("set_actor_material", &CPhysicActor::SetActorMaterial)
    ];
    luabind::module(LUA_STATE) [
        class_<CPhysicController, CObject3D>("CPhysicController")
        .def(constructor<float, float, float, float, float, ECollisionGroup, CPhysicUserData*, Vect3f&, float >())
        .def(constructor<Vect3f, float, float, float, ECollisionGroup, CPhysicUserData*, Vect3f&, float >())
        .def("move", &CPhysicController::Move)
		.def("movePlayer", &CPhysicController::MovePlayer)
        .def("jump", &CPhysicController::Jump)
        .def("create_controller", &CPhysicController::CreateController)
        //---- Functions ------
        .def("update_character_extents", &CPhysicController::UpdateCharacterExtents)
        //---- Properties ( get & Set )--------
        .def("get_user_data", &CPhysicController::GetUserData)
        .def("set_collision", &CPhysicController::SetCollision)
        .def("get_position", &CPhysicController::GetPosition)
        .def("set_position", &CPhysicController::SetPosition)
        .property("m_fGravity", &CPhysicController::GetGravity, &CPhysicController::SetGravity)
        .def("set_group", &CPhysicController::SetGroup)
        .def("set_height", &CPhysicController::SetHeight)
        .def("get_height", &CPhysicController::GetHeight)
        .def("set_active", &CPhysicController::SetActive)
        .property("m_fRadiusControler", &CPhysicController::GetRadius, &CPhysicController::SetRadius)
        .property("m_uCollisionGroups", &CPhysicController::GetColisionGroup)
        // .property("m_phx_Controller", &CPhysicController::GetPhXController)
        // .def("get_phx_controller_desc", &CPhysicController::GetPhXControllerDesc)
        .def("use_gravity", &CPhysicController::UseGravity)
        //---PhsX Info---
        .property("m_Type", &CPhysicController::GetType, &CPhysicController::SetType)
    ];
    luabind::module(LUA_STATE) [
        class_<CPlayerController, CObject3D>("CPlayerController")
        .def(constructor<>())
        .def("move", &CPlayerController::Move)
		.def("is_grounded", &CPlayerController::IsGrounded)
        .property("m_PhysicController", &CPlayerController::getPhysicController, &CPlayerController::setPhysicController )
        .property("m_PhysicUserData", &CPlayerController::getPhysicUserData, &CPlayerController::setPhysicUserData )
        .property("m_isGrounded", &CPlayerController::getisGrounded, &CPlayerController::setisGrounded)
		.property("m_Gravity", &CPlayerController::getGravity, &CPlayerController::setGravity )
		.property("m_GravityJump", &CPlayerController::getGravityJump, &CPlayerController::setGravityJump )
        .property("m_Speed", &CPlayerController::getSpeed, &CPlayerController::setSpeed )
        .property("m_JumpForce", &CPlayerController::getJumpForce, &CPlayerController::setJumpForce )
        .property("m_isJumping", &CPlayerController::getisJumping, &CPlayerController::setisJumping )
		.property("m_is3D", &CPlayerController::getis3D, &CPlayerController::setis3D )
        .property("m_CurrentJumpForce", &CPlayerController::getCurrentJumpForce, &CPlayerController::setCurrentJumpForce )
    ];
    luabind::module(LUA_STATE) [
        class_<CPhysicsManager>("CPhysicsManager")
        .def(constructor<>())
        .def("add_physic_actor", &CPhysicsManager::AddPhysicActor )
        .def("set_trigger_report", &CPhysicsManager::SetTriggerReport )
        // .def("get_physics_sdk", &CPhysicsManager::GetPhysicsSDK )
        //TODO afegir la resta a mesura que calgui / hi hagi temps
    ];
    luabind::module(LUA_STATE) [
        class_<CTrigger, CPhysicActor >("CTrigger")
        .def(constructor<CXMLTreeNode, CPhysicUserData*>())
        .def("execute_on_enter", &CTrigger::ExecuteOnEnter )
        .def("execute_on_stay", &CTrigger::ExecuteOnStay )
        .def("execute_on_exit", &CTrigger::ExecuteOnExit )
        .property("m_Update",  &CTrigger::getUpdate,  &CTrigger::setUpdate)
    ];
    luabind::module(LUA_STATE) [
        class_<CPhysicCookingMesh >("CPhysicCookingMesh")
        .def(constructor<>())
        .def("init", (void (CPhysicCookingMesh::*)(CPhysicsManager*, CPhysicUserAllocator*)) &CPhysicCookingMesh::Init )
        .def("done", &CPhysicCookingMesh::Done)
        .def("is_ok", &CPhysicCookingMesh::IsOk)
        .def("create_physic_mesh", (bool (CPhysicCookingMesh::*)(const std::string &, const std::string &)) &CPhysicCookingMesh::CreatePhysicMesh)
        .def("create_physic_mesh", (bool (CPhysicCookingMesh::*)(const std::vector<Vect3f>&, const std::vector<uint32>&, const std::string &)) &CPhysicCookingMesh::CreatePhysicMesh)
        .def("save_physic_mesh",  &CPhysicCookingMesh::SavePhysicMesh)
        .def("create_mesh_fromASE",  &CPhysicCookingMesh::CreateMeshFromASE)
        .def("cook_cloth_mesh",  &CPhysicCookingMesh::CookClothMesh)
        .def("release",  &CPhysicCookingMesh::Release)
        .def("release_physic_mesh",  &CPhysicCookingMesh::ReleasePhysicMesh)
    ];
    luabind::module(LUA_STATE) [
        class_<NxUserTriggerReport >("NxUserTriggerReport")
        .def("on_Trigger", &NxUserTriggerReport::onTrigger)
    ];
    luabind::module(LUA_STATE) [
        class_<CPhysicTriggerReport, NxUserTriggerReport >("CPhysicTriggerReport")
        //.def(constructor<>())
        .def("on_enter", &CPhysicTriggerReport::OnEnter)
        .def("on_leave", &CPhysicTriggerReport::OnLeave)
        .def("on_stay", &CPhysicTriggerReport::OnStay)
        .def("on_Trigger", &CPhysicTriggerReport::onTrigger)
    ];
}

//#endif