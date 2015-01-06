#ifndef BINDEFFECTS_H
#define BINDEFFECTS_H

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"

#include "Utils\MapManager.h"

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

#include "Effects/Effect.h"
#include "Effects/EffectTechnique.h"
#include "Effects/EffectManager.h"


using namespace luabind;

void RegisterEffects()
{
	luabind::module(LUA_STATE) [
	class_<CEffect>("CEffect")
	.def(constructor<>())
	.def("set_lights", & CEffect::SetLights)
	.def("load", & CEffect::Load)
	.def("reload", & CEffect::Reload)
	.def("get_d3d_effect", & CEffect::GetD3DEffect)
	.def("get_technique_by_name", & CEffect::GetTechniqueByName)
	];

	luabind::module(LUA_STATE) [
	class_<CEffectTechnique>("CEffectTechnique")
	.def(constructor<>())
	.def("get_effect", & CEffectTechnique::GetEffect)
	.def("begin_render", & CEffectTechnique::BeginRender)
	.def("refresh", & CEffectTechnique::Refresh)
	.def("get_d3d_technique", & CEffectTechnique::GetD3DTechnique)
	];

	luabind::module(LUA_STATE) [
    class_<CMapManager<CEffectTechnique>>("CMapManagerEffectTechnique")
    .def("get_resource", &CMapManager< CEffectTechnique >::GetResource)
    .def("existe_resource", &CMapManager< CEffectTechnique >::ExisteResource)
    .def("add_resource", &CMapManager< CEffectTechnique >::AddResource)
    .def("destroy", &CMapManager< CEffectTechnique >::Destroy)
    ];

	luabind::module(LUA_STATE) [
	class_<CEffectManager, CMapManager<CEffectTechnique>>("CEffectManager") 
	.def(constructor<>())
	.def("get_world_matrix", & CEffectManager::GetWorldMatrix)
	.def("get_projection_matrix", & CEffectManager::GetProjectionMatrix)
	.def("get_view_matrix", & CEffectManager::GetViewMatrix)
	.def("get_view_projection_matrix", & CEffectManager::GetViewProjectionMatrix)
	.def("get_camera_eye", & CEffectManager::GetCameraEye)
	.def("get_light_view_matrix", & CEffectManager::GetLightViewMatrix)
	.def("get_shadow_projection_matrix", & CEffectManager::GetShadowProjectionMatrix)
	.def("set_world_matrix", & CEffectManager::SetWorldMatrix)
	.def("set_projection_matrix", & CEffectManager::SetProjectionMatrix)
	.def("set_view_matrix", & CEffectManager::SetViewMatrix)
	.def("set_view_projection_matrix", & CEffectManager::SetViewProjectionMatrix)
	.def("set_light_view_matrix", & CEffectManager::SetLightViewMatrix)
	.def("set_shadow_projection_matrix", & CEffectManager::SetShadowProjectionMatrix)
	.def("set_camera_eye", & CEffectManager::SetCameraEye)
	.def("load", & CEffectManager::Load)
	.def("reload", & CEffectManager::Reload)
	.def("get_technique_effect_name_by_vertex_default", & CEffectManager::GetTechniqueEffectNameByVertexDefault)
	.def("get_max_lights", & CEffectManager::GetMaxLights)
	.def("get_effect", & CEffectManager::GetEffect)
	.def("get_effect_technique", & CEffectManager::GetEffectTechnique)
	.def("get_static_mesh_technique", & CEffectManager::GetStaticMeshTechnique)
	.def("get_static_mesh_mechnique", & CEffectManager::SetStaticMeshTechnique)
	.def("get_animated_model_technique", & CEffectManager::GetAnimatedModelTechnique)
	.def("set_animated_model_technique", & CEffectManager::SetAnimatedModelTechnique)
	.def("cleanUp", & CEffectManager::CleanUp)
	];

}

#endif
