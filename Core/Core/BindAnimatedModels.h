//#ifndef BINDMANAGERS_H
//#define BINDMANAGERS_H
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"
#include "AnimatedModels\AnimatedModelManager.h"
#include "AnimatedModels\AnimatedInstanceModel.h"


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


void RegisterAnimatedModels() {
  luabind::module(LUA_STATE) [
    class_<CMapManager<CAnimatedCoreModel>>("CMapManagerAnimatedCoreModel")
    .def("get_resource", &CMapManager< CTexture >::GetResource)
    .def("existe_resource", &CMapManager< CTexture >::ExisteResource)
    .def("add_resource", &CMapManager< CTexture >::AddResource)
    .def("destroy", &CMapManager< CTexture >::Destroy)
  ];
  luabind::module(LUA_STATE) [
    class_<CAnimatedInstanceModel, CRenderableObject>("CAnimatedInstanceModel")
    .def(constructor<>())
    .def("render", &CAnimatedInstanceModel::Render)
    .def("render_model_by_software", &CAnimatedInstanceModel::RenderModelBySoftware)
    .def("initialize", &CAnimatedInstanceModel::Initialize)
    .def("destroy", &CAnimatedInstanceModel::Destroy)
    .def("execute_action", &CAnimatedInstanceModel::ExecuteAction)
    .def("blend_cycle", &CAnimatedInstanceModel::BlendCycle)
    .def("clear_cycle", &CAnimatedInstanceModel::ClearCycle)
    .def("remove_action", &CAnimatedInstanceModel::RemoveAction)
    .def("getAnimationBonePosition", &CAnimatedInstanceModel::GetAnimationBonePosition)
    .def("getBonePosition", &CAnimatedInstanceModel::GetBonePosition)
    .def("getBoneMovement", &CAnimatedInstanceModel::GetBoneMovement)
    .def("getBonesMovement", &CAnimatedInstanceModel::GetBonesMovement)
    .def("updateSkeleton", &CAnimatedInstanceModel::UpdateSkeleton)
    .def("restartBonesPosition", &CAnimatedInstanceModel::RestartBonesOldPosition)
    .def("is_cycle_animation_active", &CAnimatedInstanceModel::IsCycleAnimationActive)
    .def("is_action_animation_active", &CAnimatedInstanceModel::IsActionAnimationActive)
  ];
  luabind::module(LUA_STATE) [
    class_<CAnimatedModelManager, CMapManager<CAnimatedCoreModel>>("CAnimatedModelManager")
    .def(constructor<>())
    .def("get_core", &CAnimatedModelManager::GetCore)
    .def("get_instance", &CAnimatedModelManager::GetInstance)
    .def("load", &CAnimatedModelManager::Load)
  ];
}

//#endif