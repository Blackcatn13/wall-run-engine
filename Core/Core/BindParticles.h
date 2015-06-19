/*#ifndef BINDCORE_H
#define BINDCORE_H*/
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Particles\ParticleManager.h"
#include "Particles\ParticleEmitter.h"
#include "Particles\Particle.h"
#include "Object\Object3D.h"
#include "Utils\Named.h"

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

void RegisterParticles() {
  luabind::module(LUA_STATE) [
    class_<CTemplatedVectorMapManager<CParticleEmitter>>("CTemplatedVectorMapManagerParticleManager")
    .def(constructor<>())
    .scope
    [
      class_<CTemplatedVectorMapManager<CParticleEmitter>::CMapResourceValue>("CMapResourceValue")
      .def(constructor<>())
      .def_readwrite("m_Value", & CTemplatedVectorMapManager<CParticleEmitter>::CMapResourceValue::m_Value)
      .def_readwrite("m_Id", & CTemplatedVectorMapManager<CParticleEmitter>::CMapResourceValue::m_Id)
    ]
    //.def("remove_resource", &CTemplatedVectorMapManager<CRenderableObject>::RemoveResource) // <= m_Id identificador no declarado xq está en clase interna
    .def("get_resource", &CTemplatedVectorMapManager<CParticleEmitter>::GetResource)
    .def("get_resource_by_id", &CTemplatedVectorMapManager<CParticleEmitter>:: GetResourceById)
    .def("add_resource", &CTemplatedVectorMapManager<CParticleEmitter>::AddResource)
    .def("destroy", &CTemplatedVectorMapManager<CParticleEmitter>::Destroy)
    .def("get_resource_map", &CTemplatedVectorMapManager<CParticleEmitter>::GetResourcesMap)
    .def("get_resource_vector", &CTemplatedVectorMapManager<CParticleEmitter>::GetResourcesVector)
  ];
  luabind::module(LUA_STATE) [
    class_<CParticleManager, bases<CTemplatedVectorMapManager<CParticleEmitter> >>("CParticleManager")
    .def(constructor<>())
  ];

  luabind::module(LUA_STATE) [
    class_<CParticleEmitter, bases<CObject3D, CNamed> >("CParticleEmitter")
    .def(constructor<>())
    .enum_("EmitterType")
    [
      value("EMITTER_ESF", 0),
      value("EMITTER_PLANE", 1)
    ]
    .property("m_Type", &CParticleEmitter::getType, &CParticleEmitter::setType)
    .property("m_vPos", &CParticleEmitter::getvPos, &CParticleEmitter::setvPos)
    .property("m_vSpawnDir1", &CParticleEmitter::getvSpawnDir1, &CParticleEmitter::setvSpawnDir1)
    .property("m_vSpawnDir2", &CParticleEmitter::getvSpawnDir2, &CParticleEmitter::setvSpawnDir2)
    .property("m_MinSpeed", &CParticleEmitter::getMinSpeed, &CParticleEmitter::setMinSpeed)
    .property("m_MaxSpeed", &CParticleEmitter::getMaxSpeed, &CParticleEmitter::setMaxSpeed)
    .property("m_MinParticles", &CParticleEmitter::getMinParticles, &CParticleEmitter::setMinParticles)
    .property("m_MaxParticles", &CParticleEmitter::getMaxParticles, &CParticleEmitter::setMaxParticles)
    .property("m_MinEmissionTime", &CParticleEmitter::getMinEmissionTime, &CParticleEmitter::setMinEmissionTime)
    .property("m_MaxEmissionTime", &CParticleEmitter::getMaxEmissionTime, &CParticleEmitter::setMaxEmissionTime)
    .property("m_MinAge", &CParticleEmitter::getMinAge, &CParticleEmitter::setMinAge)
    .property("m_MaxAge", &CParticleEmitter::getMaxAge, &CParticleEmitter::setMaxAge)
    .property("m_MinSize", &CParticleEmitter::getMinSize, &CParticleEmitter::setMinSize)
    .property("m_MaxSize", &CParticleEmitter::getMaxSize, &CParticleEmitter::setMaxSize)
    .property("m_sTexture", &CParticleEmitter::getsTexture, &CParticleEmitter::setsTexture)
    .property("m_Gravity", &CParticleEmitter::getGravity, &CParticleEmitter::setGravity)
    .property("m_MaxVelocidadOndulacion", &CParticleEmitter::getMaxVelocidadOndulacion, &CParticleEmitter::setMaxVelocidadOndulacion)
    .property("m_MinVelocidadOndulacion", &CParticleEmitter::getMinVelocidadOndulacion, &CParticleEmitter::setMinVelocidadOndulacion)
    .property("m_vOndulacion1", &CParticleEmitter::getvOndulacion1, &CParticleEmitter::setvOndulacion1)
    .property("m_vOndulacion2", &CParticleEmitter::getvOndulacion2, &CParticleEmitter::setvOndulacion2)
    .property("m_visible", &CParticleEmitter::getVisible, &CParticleEmitter::setvisible)
    .def("reloadTexture", &CParticleEmitter::reloadTexture)
  ];
}

//#endif