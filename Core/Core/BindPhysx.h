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
//#include "NxActor.h"
#include "Utils\PhysicUserData.h"
#include "Utils\Named.h"
#include <string>
#include "PhysicsDefs.h"


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
        .enum_("EUserDataFlag")
        [
            value("UD_IS_HW_ACTOR" , 0),
            value("UD_IS_HW_CONTROLLER", 1),
            value("UD_IS_TRIGGER", 2),
            value("UD_IS_INSIDE_TRIGGER", 3),
            value("UD_PASSES_INTERSECTION_TEST", 4),
            value("UD_HIT_BY_RAYCAST", 5),
            value("UD_NO_RENDER", 6),
            value("UD_IS_DRAIN", 7),
            value("UD_IS_ASLEEP", 8),
            value("UD_RENDER_USING_LIGHT1", 9)
        ]
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
        .property("m_PhysicController", &CPlayerController::getPhysicController, &CPlayerController::setPhysicController )
        .property("m_PhysicUserData", &CPlayerController::getPhysicUserData, &CPlayerController::setPhysicUserData )
        .property("m_Speed", &CPlayerController::getSpeed, &CPlayerController::setSpeed )
    ];
}

//#endif