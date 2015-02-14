#ifndef BINDADVANCEDSHADERS_H
#define BINDADVANCEDSHADERS_H

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"

#include "Utils\Named.h"
#include "GraphicsManager.h"
#include "Object\Object3D.h"

#include "XML/XMLTreeNode.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Renderable\RenderableObjectsManager.h"

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

void RegisterAdvancedShaders()
{
//
//	luabind::module(LUA_STATE) [
//    class_<CTemplatedVectorMapManager<CRenderableObjectsManager>>("CTemplatedVectorMapManagerRenderableObjectManager")
//	.def(constructor<>())
//	.scope
//		[
//			class_<CTemplatedVectorMapManager<CRenderableObjectsManager>::CMapResourceValue>("CMapResourceValue")
//			.def(constructor<>())
//			.def_readwrite("m_Value", & CTemplatedVectorMapManager<CRenderableObjectsManager>::CMapResourceValue::m_Value)
//			.def_readwrite("m_Id", & CTemplatedVectorMapManager<CRenderableObjectsManager>::CMapResourceValue::m_Id)
//		]
//	//.def("remove_resource", &CTemplatedVectorMapManager<CRenderableObjectsManager>::RemoveResource)
//	.def("get_resource", &CTemplatedVectorMapManager<CRenderableObjectsManager>::GetResource)
//    .def("get_resource_by_id", &CTemplatedVectorMapManager<CRenderableObjectsManager>:: GetResourceById)
//    .def("add_resource", &CTemplatedVectorMapManager<CRenderableObjectsManager>::AddResource)
//    .def("destroy", &CTemplatedVectorMapManager<CRenderableObjectsManager>::Destroy)
//	.def("get_resource_map", &CTemplatedVectorMapManager<CRenderableObjectsManager>::GetResourcesMap)
//    .def("get_resource_vector", &CTemplatedVectorMapManager<CRenderableObjectsManager>::GetResourcesVector)
//    ];
//
//	luabind::module(LUA_STATE) [
//	class_<CRenderableObjectsLayersManager, CTemplatedVectorMapManager<CRenderableObjectsManager>>("CRenderableObjectsLayersManager")
//	.def(constructor<>())
//	.def("destroy", & CRenderableObjectsLayersManager::Destroy)
//	.def("load", & CRenderableObjectsLayersManager::Load)
//	.def("reload", & CRenderableObjectsLayersManager::Reload)
//	.def("update", & CRenderableObjectsLayersManager::Update)
//	.def("render", (void (CRenderableObjectsLayersManager::*) (CGraphicsManager *)) &CRenderableObjectsLayersManager::Render)
//	.def("render", (void (CRenderableObjectsLayersManager::*) (CGraphicsManager *, const std::string &)) &CRenderableObjectsLayersManager::Render)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CRenderableObjectTechnique, CNamed>("CRenderableObjectTechnique")
//	.def(constructor<const std::string &, CEffectTechnique *>())
//	.def("set_effect_technique", & CRenderableObjectTechnique::SetEffectTechnique)
//	.def("get_effect_technique", & CRenderableObjectTechnique::GetEffectTechnique)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CPoolRenderableObjectTechnique, CNamed>("CPoolRenderableObjectTechnique")
//	.def(constructor<CXMLTreeNode>())
//	.def("destroy", & CPoolRenderableObjectTechnique::Destroy)
//	.def("addElement", & CPoolRenderableObjectTechnique::AddElement)
//	.def("apply", & CPoolRenderableObjectTechnique::Apply)
//	];
//
//	luabind::module(LUA_STATE) [
//    class_<CMapManager<CRenderableObjectTechnique>>("CMapManagerRenderableObjectTechnique")
//    .def("get_resource", &CMapManager< CRenderableObjectTechnique >::GetResource)
//    .def("existe_resource", &CMapManager< CRenderableObjectTechnique >::ExisteResource)
//    .def("add_resource", &CMapManager< CRenderableObjectTechnique >::AddResource)
//    .def("destroy", &CMapManager< CRenderableObjectTechnique >::Destroy)
//    ];
//
//	luabind::module(LUA_STATE) [
//	class_<CRenderableObjectTechniqueManager, CMapManager<CRenderableObjectTechnique>>("CRenderableObjectTechniqueManager")
//	.def(constructor<>())
//    .def("destroy", &CRenderableObjectTechniqueManager::Destroy)
//    .def("load", &CRenderableObjectTechniqueManager::Load)
//    .def("get_renderable_object_technique_name_by_vertex_type", &CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType)
//    .def("get_pool_renderable_object_techniques", &CRenderableObjectTechniqueManager::GetPoolRenderableObjectTechniques)
//    ];
//
//	luabind::module(LUA_STATE) [
//	class_<CSceneRendererCommandManager>("CSceneRendererCommandManager")
//	.def(constructor<>())
//	.def("load", & CSceneRendererCommandManager::Load)
//	.def("execute", & CSceneRendererCommandManager::Execute)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CSceneRendererCommand, /*bases<CUABActive,*/CNamed/*>*/>("CSceneRendererCommand")
//	.def(constructor<CXMLTreeNode>())
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CClearSceneRendererCommand, CSceneRendererCommand>("CClearSceneRendererCommand")
//	.def(constructor<CXMLTreeNode>())
//	.def("execute", & CClearSceneRendererCommand::Execute)
//	];
}
#endif
