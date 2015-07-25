#ifndef BINDLIGHTS_H
#define BINDLIGHTS_H

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"

#include "Object\Object3D.h"
#include "Lights\Light.h"
#include "Lights\LightManager.h"
#include "Lights\DirectionalLight.h"
#include "Lights\OmniLight.h"
#include "Lights\SpotLight.h"

#include "Utils\MapManager.h"
#include "Utils\TemplatedVectorMapManager.h"

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

void RegisterLights()
{
    luabind::module(LUA_STATE) [
        class_<CLight, CObject3D>("CLight")
        .enum_("TLightType")
        [
            value("OMNI" , 0),
            value("DIRECTIONAL", 1),
            value("SPOT", 2)
        ]
        //.def(constructor<>())
        .def("set_name", & CLight::SetName)
        .def("get_name", & CLight::GetName)
        .def("set_color", & CLight::SetColor)
        .def("get_color", & CLight::GetColor)
        .def("set_intensity", & CLight::SetIntensity)
        .def("get_intensity", & CLight::GetIntensity)
		.def("get_valoryfijo", & CLight::GetValorYFijo)
        .def("set_start_range_attenuation", & CLight::SetStartRangeAttenuation)
        .def("get_start_range_attenuation", & CLight::GetStartRangeAttenuation)
        .def("set_end_range_attenuation", & CLight::SetEndRangeAttenuation)
        .def("get_end_range_attenuation", & CLight::GetEndRangeAttenuation)
        .def("render_shadows", & CLight::RenderShadows)
        .def("set_type", & CLight::SetType)
        .def("get_type", & CLight::GetType)
        .def("render", & CLight::Render)
        ////////////////////////////
        .def("set_generate_dynamic_shadow_map", & CLight::SetGenerateDynamicShadowMap)
        .def("get_generate_dynamic_shadow_map", & CLight::GetGenerateDynamicShadowMap)
        .def("set_generate_static_shadow_map", & CLight::SetGenerateStaticShadowMap)
        .def("get_generate_static_shadow_map", & CLight::GetGenerateStaticShadowMap)
        .def("set_must_update_shadow_map", & CLight::SetMustUpdateStaticShadowMap)
        .def("get_must_update_shadow_map", & CLight::GetMustUpdateStaticShadowMap)
        .def("get_static_shadow_map", & CLight::GetStaticShadowMap)
        .def("get_generate_static_shadow_map", & CLight::GetGenerateStaticShadowMap)
        .def("set_must_update_shadow_map", & CLight::SetMustUpdateStaticShadowMap)
        .def("get_must_update_shadow_map", & CLight::GetMustUpdateStaticShadowMap)
        .def("get_static_shadow_map", & CLight::GetStaticShadowMap)
        .def("get_dynamic_shadow_map", & CLight::GetStaticShadowMap)
        .def("get_shadow_map_texture", & CLight::GetShadowMaskTexture)
        .def("get_static_shadow_map_renderable_objects_managers", & CLight::GetStaticShadowMapRenderableObjectsManagers)
        .def("get_dynamic_shadow_map_renderable_objects_managers", & CLight::GetDynamicShadowMapRenderableObjectsManagers)
        .def("generate_shadow_map", & CLight::GenerateShadowMap)
        .def("get_view_shadow_map", & CLight::GetViewShadowMap)
        .def("begin_render_effect_manager_shadow_map", & CLight::BeginRenderEffectManagerShadowMap)
    ];
    luabind::module(LUA_STATE) [
        class_<CMapManager<CLight>>("CMapManagerLight")
        .def("get_resource", &CMapManager< CLight >::GetResource)
        .def("existe_resource", &CMapManager< CLight >::ExisteResource)
        .def("add_resource", &CMapManager< CLight >::AddResource)
        .def("destroy", &CMapManager< CLight >::Destroy)
    ];
    luabind::module(LUA_STATE) [
        class_<CLightManager , CMapManager<CLight>>("CLightManager")
        .def(constructor<>())
        .def("load", & CLightManager::Load)
        .def("render", & CLightManager::Render)
    ];
    luabind::module(LUA_STATE) [
        class_<COmniLight, CLight>("COmniLight")
        .def(constructor<>())
        .def("set_shadow_map", & CDirectionalLight::SetShadowMap)
    ];
    luabind::module(LUA_STATE) [
        class_<CDirectionalLight, CLight>("CDirectionalLight")
        .def(constructor<>())
        .def("set_direction", & CDirectionalLight::SetDirection)
        .def("get_direction", & CDirectionalLight::GetDirection)
        .def("render", & CDirectionalLight::Render)
        .def("set_shadow_map", & CDirectionalLight::SetShadowMap)
    ];
    luabind::module(LUA_STATE) [
        class_<CSpotLight, CDirectionalLight>("CSpotLight")
        .def(constructor<>())
        .def("set_angle", & CSpotLight::SetAngle)
        .def("get_angle", & CSpotLight::GetAngle)
        .def("set_fall_off", & CSpotLight::SetFallOff)
        .def("get_fall_off", & CSpotLight::GetFallOff)
    ];
}

#endif
