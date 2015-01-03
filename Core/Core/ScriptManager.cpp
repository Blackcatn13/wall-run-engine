#include "ScriptManager.h"
#include "Camera\Camera.h"

//Código de la función Alert que se llamará al generarse algún error de LUA
int Alert(/*IN */lua_State * State)
{
        std::string l_Text;
        int n = lua_gettop(State);
        int i;
        lua_getglobal(State, "tostring");
        for (i=1; i<=n; i++) 
        {
                const char *s;
                lua_pushvalue(State, -1);
                lua_pushvalue(State, i);
                lua_call(State, 1, 1);
                s = lua_tostring(State, -1);
                if (s == NULL)
                        return luaL_error(State, "`tostring' must return a string to `print'");
                if (i>1) l_Text += '\t';
                l_Text += s;
                lua_pop(State, 1);
        }
        l_Text += '\n';
        //Info( l_Text.c_str() );
        LOGGER->AddNewLog(ELL_INFORMATION,  l_Text.c_str());
        return true;
}

CScriptManager::CScriptManager()
{
}


CScriptManager::~CScriptManager()
{
        Destroy();
}


//Para inicializar el motor de LUA
void CScriptManager::Initialize()
{
		m_DeleteMap = false;
        m_LS=luaL_newstate();
        
        luaL_openlibs(m_LS);
        //Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar
        //código LUA
		luabind::open(m_LS);
        lua_register(m_LS,"_ALERT",Alert);
		RegisterLUAFunctions();
}

//Para desinicializar el motor de LUA
void CScriptManager::Destroy()
{
        lua_close(m_LS);
		if(m_DeleteMap)
			m_ScriptsMap.clear();
}
//Para ejecutar un fragmento de código LUA
void CScriptManager::RunCode(const std::string &Code) const
{
        if(luaL_dostring(m_LS,Code.c_str()))
        {
        const char *l_Str=lua_tostring(m_LS, -1);
        //Info("%s",l_Str);
        LOGGER->AddNewLog(ELL_INFORMATION, l_Str);
        }
}
//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
        if(luaL_dofile(m_LS, FileName.c_str()))
        {
                const char *l_Str=lua_tostring(m_LS, -1);
                //Info("%s",l_Str);
                LOGGER->AddNewLog(ELL_INFORMATION, l_Str);
        }
}

void CScriptManager::Load(const std::string &XMLFile)
{
	//TODO Añadir scripts a un vector?
	std::string l_FileName = XMLFile;
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(XMLFile.c_str()))
	{
		std::string l_ErrorMsg = "ERROR loading file" + XMLFile;
		LOGGER->AddNewLog(ELL_ERROR,  l_ErrorMsg.c_str());
		
	}else
	{
		CXMLTreeNode  m = newFile["lua_scripts"];
		if (m.Exists())
		{
			int count = m.GetNumChildren();
			for (int i = 0; i < count; ++i)
			{
					std::string l_name = m(i).GetPszISOProperty("name", "");
					std::string l_file = m(i).GetPszISOProperty("file", "");
					m_ScriptsMap.insert ( std::pair<std::string,std::string>(l_name,l_file) );
			} 
			m_DeleteMap = true;
		}
	}
}

void RegisterLights()
{
	luabind::module(LUA_STATE) [
	class_<CLight, CObject3D>("CLight")
	.def(constructor<>())
    .def("set_name", & CLight::SetName)
    .def("get_name", & CLight::GetName)
	.def("set_color", & CLight::SetColor)
    .def("get_color", & CLight::GetColor)
	.def("set_start_range_attenuation", & CLight::SetStartRangeAttenuation)
    .def("get_start_range_attenuation", & CLight::GetStartRangeAttenuation)
	.def("set_end_range_attenuation", & CLight::SetEndRangeAttenuation)
    .def("get_end_range_attenuation", & CLight::GetEndRangeAttenuation)
    .def("render_shadows", & CLight::RenderShadows)
    .def("set_type", & CLight::SetType)
	.def("get_type", & CLight::GetType)
    .def("render", & CLight::Render)
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

//void RegisterCameras()
//{
//	luabind::module(LUA_STATE) [
//	class_<CCameraInfo>("CCameraInfo")
//	.def(constructor<>())
//	.def(constructor<const Vect3f &, const Vect3f &, const Vect3f &, float, float, float >())
//	.def(constructor<CXMLTreeNode>())
//	.def_readwrite("nearPlane", &CCameraInfo::m_NearPlane)
//	.def_readwrite("farPlane", &CCameraInfo::m_FarPlane)
//	.def_readwrite("fov", &CCameraInfo::m_FOV)
//	.def_readwrite("eye", &CCameraInfo::m_Eye)
//	.def_readwrite("lookAt", &CCameraInfo::m_LookAt)
//	.def_readwrite("up", &CCameraInfo::m_Up)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CCameraKey>("CCameraKey")
//	.def(constructor<CCameraInfo, float>())
//	.def_readwrite("cameraInfo", &CCameraKey::m_CameraInfo)
//	.def_readwrite("time", &CCameraKey::m_Time)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CCameraKeyController>("CCameraKeyController")
//	.def(constructor<CXMLTreeNode &>())
//	.def("update", & CCameraKeyController::Update)
//	.def("set_current_time", & CCameraKeyController::SetCurrentTime)
//	.def("reset_time", & CCameraKeyController::ResetTime)
//	.def("is_cycle", & CCameraKeyController::IsCycle)
//	.def("set_cycle", & CCameraKeyController::SetCycle)
//	.def("is_reverse", & CCameraKeyController::IsReverse)
//	.def("set_reverse", & CCameraKeyController::SetReverse)
//	];
//
//}
//
//void RegisterCinematics()
//{
//	luabind::module(LUA_STATE) [
//	class_<CCinematicPlayer>("CCinematicPlayer")
//	.def(constructor<>())
//	.def("init", & CCinematicPlayer::Init)
//	.def("update", & CCameraKeyController::Update)
//	.def("stop", & CCinematicPlayer::Stop)
//	.def("play", & CCinematicPlayer::Play)
//	.def("pause", & CCinematicPlayer::Pause)
//	.def("is_finished", & CCinematicPlayer::IsFinished)
//	.def("get_duration", & CCinematicPlayer::GetDuration)
//	.def("get_current_time", & CCinematicPlayer::GetCurrentTimes) //Cambiar por GetCurrentTime
//	.def("on_restart_cycle", & CCinematicPlayer::OnRestartCycle)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CCinematicObjectKeyFrame, CObject3D>("CCinematicObjectKeyFrame")
//	.def(constructor<CXMLTreeNode &>())
//	.def("get_key_frame_time", & CCinematicObjectKeyFrame::GetKeyFrameTime)
//	.def("set_key_frame_time", & CCinematicObjectKeyFrame::SetKeyFrameTime)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CCinematicObject, CCinematicPlayer>("CCinematicObject")
//	.def(constructor<CXMLTreeNode &>())
//	.def("is_ok", & CCinematicObject::IsOk)
//	.def("add_cinematic_object_key_frame", & CCinematicObject::AddCinematicObjectKeyFrame)
//	.def("update", & CCinematicObject::Update)
//	.def("stop", & CCinematicObject::Stop)
//	.def("on_restart_cycle", & CCinematicObject::OnRestartCycle)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CCinematic, bases<CRenderableObject, CCinematicPlayer>>("CCinematic")
//	.def(constructor<CXMLTreeNode &>())
//	.def("stop", & CCinematic::Stop)
//	.def("play", & CCinematic::Play)
//	.def("pause", & CCinematic::Pause)
//	.def("load_xml", & CCinematic::LoadXML)
//	.def("add_cinematic_object", & CCinematic::AddCinematicObject)
//	.def("update", & CCinematic::Update)
//	.def("render", & CCinematic::Render)
//	];
//}
//
//void RegisterEffects()
//{
//	luabind::module(LUA_STATE) [
//	class_<CEffect>("CEffect")
//	.def(constructor<>())
//	.def("set_lights", & CEffect::SetLights)
//	.def("load", & CEffect::Load)
//	.def("reload", & CEffect::Reload)
//	.def("get_d3d_effect", & CEffect::GetD3DEffect)
//	.def("get_technique_by_name", & CEffect::GetTechniqueByName)
//	];
//
//	luabind::module(LUA_STATE) [
//	class_<CEffectTechnique>("CEffectTechnique")
//	.def(constructor<>())
//	.def("get_effect", & CEffectTechnique::GetEffect)
//	.def("begin_render", & CEffectTechnique::BeginRender)
//	.def("refresh", & CEffectTechnique::Refresh)
//	.def("get_d3d_technique", & CEffectTechnique::GetD3DTechnique)
//	];
//
//	luabind::module(LUA_STATE) [
//    class_<CMapManager<CEffectTechnique>>("CMapManagerEffectTechnique")
//    .def("get_resource", &CMapManager< CEffectTechnique >::GetResource)
//    .def("existe_resource", &CMapManager< CEffectTechnique >::ExisteResource)
//    .def("add_resource", &CMapManager< CEffectTechnique >::AddResource)
//    .def("destroy", &CMapManager< CEffectTechnique >::Destroy)
//    ];
//
//	luabind::module(LUA_STATE) [
//	class_<CEffectManager, CMapManager<CEffectTechnique>>("CEffectManager") 
//	.def(constructor<>())
//	.def("get_world_matrix", & CEffectManager::GetWorldMatrix)
//	.def("get_projection_matrix", & CEffectManager::GetProjectionMatrix)
//	.def("get_view_matrix", & CEffectManager::GetViewMatrix)
//	.def("get_view_projection_matrix", & CEffectManager::GetViewProjectionMatrix)
//	.def("get_camera_eye", & CEffectManager::GetCameraEye)
//	.def("get_light_view_matrix", & CEffectManager::GetLightViewMatrix)
//	.def("get_shadow_projection_matrix", & CEffectManager::GetShadowProjectionMatrix)
//	.def("set_world_matrix", & CEffectManager::SetWorldMatrix)
//	.def("set_projection_matrix", & CEffectManager::SetProjectionMatrix)
//	.def("set_view_matrix", & CEffectManager::SetViewMatrix)
//	.def("set_view_projection_matrix", & CEffectManager::SetViewProjectionMatrix)
//	.def("set_light_view_matrix", & CEffectManager::SetLightViewMatrix)
//	.def("set_shadow_projection_matrix", & CEffectManager::SetShadowProjectionMatrix)
//	.def("set_camera_eye", & CEffectManager::SetCameraEye)
//	.def("load", & CEffectManager::Load)
//	.def("reload", & CEffectManager::Reload)
//	.def("get_technique_effect_name_by_vertex_default", & CEffectManager::GetTechniqueEffectNameByVertexDefault)
//	.def("get_max_lights", & CEffectManager::GetMaxLights)
//	.def("get_effect", & CEffectManager::GetEffect)
//	.def("get_effect_technique", & CEffectManager::GetEffectTechnique)
//	.def("get_static_mesh_technique", & CEffectManager::GetStaticMeshTechnique)
//	.def("get_static_mesh_mechnique", & CEffectManager::SetStaticMeshTechnique)
//	.def("get_animated_model_technique", & CEffectManager::GetAnimatedModelTechnique)
//	.def("set_animated_model_technique", & CEffectManager::SetAnimatedModelTechnique)
//	.def("cleanUp", & CEffectManager::CleanUp)
//	];
//
//}
//void RegisterAdvancedShaders()
//{
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
//}

void CScriptManager::RegisterLUAFunctions()
{

        luabind::module(LUA_STATE) [
        class_<CScriptManager>("CScriptManager")
       // .def("load_file", & CScriptManager::LoadFile)
        .def("run_code", & CScriptManager::RunCode)
        .def("run_file", & CScriptManager::RunFile)
        .def("load", & CScriptManager::Load)
		.def("get_scripts_map", & CScriptManager::GetScriptsMap)
        ];

		luabind::module(LUA_STATE) [
        class_<Vect3f>("Vect3f") 
        .def(constructor<float, float, float>())
        .def(constructor<>())
        .def(constructor<Vect3f>())
      
        .def(const_self + const_self)
        .def(const_self - const_self)
        .def(const_self * const_self)
        .def(const_self / float())
        .def(const_self == const_self)
        .def(const_self ^ const_self)
        // Operadores y funciones de asignación
        .def("set",  &Vect3f::Set)
        .def("set_zero",  &Vect3f::SetZero)
        // Coordenadas polares
        .def("set_from_polar", &Vect3f::SetFromPolar)
        .def("get_polar",  &Vect3f::GetPolar)
        //funciones de comparacion
        .def("is_equal_epsilon",  &Vect3f::IsEqualEpsilon)
        .def("is_not_equal_epsilon",  &Vect3f::IsNotEqualEpsilon)
        // Producto por componentes (escalado)
        .def("scale",  &Vect3f::Scale)
        .def("get_scaled",  &Vect3f::GetScaled)
        // Establecimiento condicional
        .def("set_if_min_components",  &Vect3f::SetIfMinComponents)
        .def("set_if_max_components",  &Vect3f::SetIfMaxComponents)
        //Proyecciones
        .def("get_proj_xy",  &Vect3f::GetProjXY)
        .def("get_proj_yz",  &Vect3f::GetProjYZ)
        .def("get_proj_zx",  &Vect3f::GetProjZX)
        // Funciones de la longitud 
        .def("normalize",  &Vect3f::Normalize)
        .def("get_normalized",  &Vect3f::GetNormalized)
        .def("length",  &Vect3f::Length)
        .def("squared_length",  &Vect3f::SquaredLength)
        .def("distance",  &Vect3f::Distance)
        .def("sq_distance",  &Vect3f::SqDistance)
        // Rotaciones en los ejes principales
        .def("rotate_x",  &Vect3f::RotateX)
        .def("get_rotated_x",  &Vect3f::GetRotatedX)
        .def("rotate_y",  &Vect3f::RotateY)
        .def("get_rotated_y",  &Vect3f::GetRotatedY)
        .def("rotate_z",  &Vect3f::RotateZ)
        .def("get_rotated_z",  &Vect3f::GetRotatedZ)
        .def("get_angle_x",  &Vect3f::GetAngleX)
        .def("get_angle_y",  &Vect3f::GetAngleY)
        .def("get_angle_z",  &Vect3f::GetAngleZ)
        .def("get_angles",  &Vect3f::GetAngles)
         // Interpolación lineal
        .def("lerp",  &Vect3f::Lerp)
        .def("get_lerp",  &Vect3f::GetLerp)
        // cosas externas (funciones, constantes...) no se registran?*/
        //Variables
        .def_readwrite("x", &Vect3f::x)
        .def_readwrite("y", &Vect3f::y)
        .def_readwrite("z", &Vect3f::z)
        ];
        
		luabind::module(LUA_STATE) [
        class_< Mat44f >("Mat44f")
        .def(constructor<Mat44f>())
		.def(constructor<>())
		.def(constructor<D3DXMATRIX>())
	/*	.def(constructor<float, float, float, float, 
						 float, float, float, float,
						 float,float, float, float,
						 float, float, float, float>())*/
		.def(constructor<Vect3f,Vect3f,Vect3f,Vect3f>())
		.def(constructor<float,float,float,float>())
		 // Reseteos parciales (traslación/rotación/escalado) y totales (a identidad, a cero)
		.def("reset_translation", & Mat44f::ResetTranslation)
		.def("reset_rotation", & Mat44f::ResetRotation)
		.def("reset_scale", & Mat44f::ResetScale)
		.def("reset_rotation_scale", & Mat44f::ResetRotationScale)
		.def("set_identity", & Mat44f::SetIdentity)
		.def("set_zero", & Mat44f::SetZero)
		// Establecimiento de datos de la matriz con reseteo previo a identidad
		.def("set_from_basis",(Mat44f (Mat44f::*)(Vect3f &, Vect3f &, Vect3f &, Vect3f &))  & Mat44f::SetZero)            
		.def("set_from_basis",(Mat44f (Mat44f::*)(Vect3f &, Vect3f &, Vect3f &))  & Mat44f::SetZero)
		.def("set_from_angle_x", & Mat44f::SetFromAngleX)
		.def("set_from_angle_y", & Mat44f::SetFromAngleY)
		.def("set_from_angle_z", & Mat44f::SetFromAngleZ)
		.def("set_from_angles_xz", & Mat44f::SetFromAnglesXZ)
		.def("set_from_angles_yxz", & Mat44f::SetFromAnglesYXZ)
		.def("set_from_pos",(Mat44f & (Mat44f::*)(const Vect3f &)) & Mat44f::SetFromPos)
		.def("set_from_pos",(Mat44f & (Mat44f::*)(const float, const float, const float)) & Mat44f::SetFromPos)
        .def("set_from_scale", & Mat44f::SetFromScale)
		.def("set_from_look_at",(Mat44f & (Mat44f::*)(const Vect3f &, const Vect3f &, const Vect3f &)) & Mat44f::SetFromLookAt)
		.def("set_from_look_at",(Mat44f & (Mat44f::*)(const Vect3f &, const Vect3f &)) & Mat44f::SetFromLookAt)
		//.def("set_from_quat_pos", & Mat44f::SetFromQuatPos)
		.def("set_rot_by_angle_x", & Mat44f::SetRotByAngleX)
		.def("set_rot_by_angle_y", & Mat44f::SetRotByAngleY)
		.def("set_rot_by_angle_z", & Mat44f::SetRotByAngleZ)
		.def("set_rot_by_angles_xz", & Mat44f::SetRotByAnglesXZ)
		.def("set_rot_by_angles_yxz", & Mat44f::SetRotByAnglesYXZ)
		//.def("set_rot_by_quat", & Mat44f::SetRotByQuat)

		.def("set_pos",(Mat44f & (Mat44f::*)(const Vect3f &))  & Mat44f::SetPos)
		.def("set_pos", (Mat44f & (Mat44f::*)(const float, const float, const float)) & Mat44f::SetPos)
		
		//En estas da error de compilación aunque las encuentra
		//.def("set_scale", (Mat44f & (Mat44f::*)(const Matrix33<float> &mat_escala)) & Mat44f::SetScale)
		//.def("set_scale", (Mat44f & (Mat44f::*)(const Mat44f &)) & Mat44f::SetScale)
		.def("set_scale", (Mat44f & (Mat44f::*)(const Vect3f &)) & Mat44f::SetScale)
		.def("set_scale", (Mat44f & (Mat44f::*)(const float,const float,const float)) & Mat44f::SetScale)
		
		.def("set_sub_matrix",  & Mat44f::SetSubMatrix33)
		.def("set_sub_matrix",  & Mat44f::SetSubMatrix34)
		// Acceso
		.def("get_row",  & Mat44f::GetRow)
		.def("set_colum",  & Mat44f::GetColum)
		.def("set_vector_basis",  & Mat44f::GetVectorBasis)
		.def("set_vector_basis_length",  & Mat44f::GetVectorBasisLength)
		.def("set_pos_basis",  & Mat44f::GetPosBasis)
		.def("set_basis",  & Mat44f::GetBasis)
	
		//Estas no las encuentra
		//.def("get_sub_matrix33", (Mat33f (Mat44f::*) (void)) & Mat44f::GetSubMatrix33)
		//.def("get_sub_matrix33", (void (Mat44f::*)(Mat33f &)) & Mat44f::GetSubMatrix33)
		//.def("get_sub_matrix34", (Mat34f  (Mat44f::*)()) & Mat44f::GetSubMatrix34)
		//.def("get_sub_matrix34", (void  (Mat44f::*)(Mat34f &)) & Mat44f::GetSubMatrix34)
		
		.def("get_33_rotation_normalized",  & Mat44f::Get33RotationNormalized)
		.def("get_33_rotation_scaled",  & Mat44f::Get33RotationScaled)
		.def("get_33_scale",  & Mat44f::Get33Scale)
		.def("get_translation_vector",  & Mat44f::GetTranslationVector)

		.def("get_44_rotation_normalized",  & Mat44f::Get44RotationNormalized)
		.def("get_44_rotation_scaled",  & Mat44f::Get44RotationScaled)
		.def("get_44_scale",  & Mat44f::Get44Scale)
		.def("get_44_translation",  & Mat44f::Get44Translation)

		.def("get_pos",  & Mat44f::GetPos)
		.def("get_scale",  & Mat44f::GetScale)
		.def("get_scale_x",  & Mat44f::GetScaleX)
		.def("get_scale_y",  & Mat44f::GetScaleX)
		.def("get_scale_z",  & Mat44f::GetScaleX)
		 // Angulos de Euler
		.def("get_angles_xyz",  & Mat44f::GetAnglesYXZ)
		.def("get_angle_x",  & Mat44f::GetScaleX)
		.def("get_angle_y",  & Mat44f::GetScaleY)
		.def("get_angle_z",  & Mat44f::GetScaleZ)
		 // Angulos de Euler con criterio de Pitch-Roll-Yaw
		 // (Orden XZY, teniendo los ángulos Y,Z el sentido contrario de rotación)
		.def("get_roll",  & Mat44f::GetRoll)
		.def("get_pitch",  & Mat44f::GetPitch)
		.def("get_yaw",  & Mat44f::GetYaw)
		.def("get_pitch_roll_yaw",  & Mat44f::GetPitchRollYaw)

		.def("set_fom_pitch_roll_yaw",  & Mat44f::SetFromPitchRollYaw)
		.def("set_pitch_roll_yaw",  & Mat44f::SetPitchRollYaw)
		 // Operadores de aritmética de matrices
		.def(const_self + const_self)
        .def(const_self - const_self)
        .def(const_self * const_self)
		.def(const_self * float())
		.def(const_self * (float(), const_self))
		//.def(Vect3f() * Vect3f())
        .def(const_self / float())
        .def(const_self == const_self)
       // .def(Vect3f() ^ Vect3f())

	     // Operadores de comparación
	 	.def("is_equal_epsilon",  & Mat44f::IsEqualEpsilon)
		.def("is_not_equal_epsilon",  & Mat44f::IsNotEqualEpsilon)
		 // Funciones de transformación de vectores / puntos
		.def("transform_vector",  & Mat44f::TransformVector)
		.def("transform_point",  & Mat44f::TransformPoint)
		.def("get_transform_vector",  & Mat44f::GetTransformVector)
		.def("get_transform_point",  & Mat44f::GetTransformPoint)
		.def("get_transform_array_vectors",  & Mat44f::TransformArrayVectors)
		.def("get_transform_array_points",  & Mat44f::TransformArrayPoints)
		.def("get_d3dx_matrix",  & Mat44f::GetD3DXMatrix)
		  // Operaciones especiales con matrices
		.def("get_inverted",  & Mat44f::GetInverted)
		.def("get_inverted_trs",  & Mat44f::GetInvertedTRS)
		.def("get_inverted_tr",  & Mat44f::GetInvertedTR)
		.def("get_transposed",  & Mat44f::GetTransposed)
		.def("get_roted_by_angle_x",  & Mat44f::GetRotedByAngleX)
		.def("get_roted_by_angle_y",  & Mat44f::GetRotedByAngleY)
		.def("get_roted_by_angle_z",  & Mat44f::GetRotedByAngleZ)
		.def("get_roted_by_angles_xz",  & Mat44f::GetRotedByAnglesXZ)
		.def("get_roted_by_angles_yxz",  & Mat44f::GetRotedByAnglesYXZ)
		.def("get_translated",  & Mat44f::GetTranslated)
		.def("get_scaled",  & Mat44f::GetScaled)
		
		.def("invert",  & Mat44f::Invert)
		.def("invert_trs",  & Mat44f::InvertTRS)
		.def("invert_tr",  & Mat44f::InvertTR)
		.def("transpose",  & Mat44f::Transpose)
		.def("rot_by_angle_x",  & Mat44f::RotByAngleX)
		.def("rote_by_angle_y",  & Mat44f::RotByAngleY)
		.def("rote_by_angle_z",  & Mat44f::RotByAngleZ)
		.def("rote_by_angles_xz",  & Mat44f::RotByAnglesXZ)
		.def("rote_by_angles_yxz",  & Mat44f::RotByAnglesYXZ)
		.def("translate",  & Mat44f::Translate)
		.def("scaled",  & Mat44f::Scale)

		.def("align_xy",  & Mat44f::AlignYX)
		.def("align_yz",  & Mat44f::AlignYZ)
		.def("align_xz",  & Mat44f::AlignXZ)

		 // Acceso/determinación a/de propiedades matriciales
		.def("determinant",  & Mat44f::Determinant)
		.def("is_orthogonal_epsilon",  & Mat44f::IsOrthogonalEpsilon)
		.def("is_orthonormal_epsilon",  & Mat44f::IsOrthonormalEpsilon)
         ];

        luabind::module(LUA_STATE) [
        class_<CVisible>("CVisible")
        .def("get_visible", & CVisible::getVisible)
        .def("set_visible", & CVisible::setVisible)
        ];

		module(LUA_STATE) [
        class_<CNamed>("CNamed")
		.def(constructor<>()) //Como no tiene metodos estaticos para probar el LUA pongo un constructor vacio
        .def("get_name", & CNamed::getName)
        .def("set_name", & CNamed::setName)
        ];
		
		luabind::module(LUA_STATE) [
        class_< CObject3D, CVisible >("CObject3D")
        .def(constructor<>())
        .def(constructor<Vect3f, float, float, float>()) //Falta registrar la Vect3f
        .def("get_yaw", & CObject3D::GetYaw)
        .def("get_pitch", & CObject3D::GetPitch)
        .def("get_roll", & CObject3D::GetRoll)
        .def("get_position", & CObject3D::GetPosition)
        .def("set_yaw", & CObject3D::SetYaw)
        .def("set_pitch", & CObject3D::SetPitch)
        .def("set_roll", & CObject3D::SetRoll)
        .def("set_position", & CObject3D::SetPosition)
        ];

        luabind::module(LUA_STATE) [
		class_< CRenderableObject, bases<CObject3D, CNamed> >("CRenderableObject")
        //.def(constructor<>())
        .def("update", & CRenderableObject::Update)
        .def("render", & CRenderableObject::Render)
        ];

		luabind::module(LUA_STATE) [
		class_<CTemplatedVectorMapManager<CRenderableObject>>("CTemplatedVectorMapManagerRenderableObject")
		.def(constructor<>())
		.scope
		[
			class_<CTemplatedVectorMapManager<CRenderableObject>::CMapResourceValue>("CMapResourceValue")
			.def(constructor<>())
			.def_readwrite("m_Value", & CTemplatedVectorMapManager<CRenderableObject>::CMapResourceValue::m_Value)
			.def_readwrite("m_Id", & CTemplatedVectorMapManager<CRenderableObject>::CMapResourceValue::m_Id)
		]

		//.def("remove_resource", &CTemplatedVectorMapManager<CRenderableObject>::RemoveResource) // <= m_Id identificador no declarado xq está en clase interna
		.def("get_resource", &CTemplatedVectorMapManager<CRenderableObject>::GetResource)
		.def("get_resource_by_id", &CTemplatedVectorMapManager<CRenderableObject>:: GetResourceById)
		.def("add_resource", &CTemplatedVectorMapManager<CRenderableObject>::AddResource)
		.def("destroy", &CTemplatedVectorMapManager<CRenderableObject>::Destroy)
		.def("get_resource_map", &CTemplatedVectorMapManager<CRenderableObject>::GetResourcesMap)
		.def("get_resource_vector", &CTemplatedVectorMapManager<CRenderableObject>::GetResourcesVector)
		];

       luabind:: module(LUA_STATE) [
        class_< CRenderableObjectsManager, CTemplatedVectorMapManager<CRenderableObject> >("CRenderableObjectsManager")
        .def(constructor<>())
        .def("update", & CRenderableObjectsManager::Update)
        .def("render", & CRenderableObjectsManager::Render)
        .def("add_mesh_instance", (CRenderableObject *(CRenderableObjectsManager::*)(const std::string &, const std::string &, const Vect3f &)) &CRenderableObjectsManager::AddMeshInstance)
        .def("add_mesh_instance", (CRenderableObject *(CRenderableObjectsManager::*)(CXMLTreeNode &Node)) &CRenderableObjectsManager::AddMeshInstance)
        .def("add_animated_instance_model", (CRenderableObject *(CRenderableObjectsManager::*)(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position)) &CRenderableObjectsManager::AddAnimatedInstanceModel)
        .def("add_animated_instance_model", (CRenderableObject *(CRenderableObjectsManager::*)(CXMLTreeNode &Node)) &CRenderableObjectsManager::AddAnimatedInstanceModel)
        .def("load", & CRenderableObjectsManager::Load)
        .def("reload", & CRenderableObjectsManager::Reload)
        ];
		
           //Si queremos registrar una clase templatizada como la clase CTextureManager debemos primero
        //registrar su clase base con la clases templatizada y después la clase
        luabind::module(LUA_STATE) [
        class_<CMapManager<CTexture>>("CMapManagerTexture")
        .def("get_resource", &CMapManager< CTexture >::GetResource)
        .def("existe_resource", &CMapManager< CTexture >::ExisteResource)
        .def("add_resource", &CMapManager< CTexture >::AddResource)
        .def("destroy", &CMapManager< CTexture >::Destroy)
        ];

        luabind::module(LUA_STATE) [
        class_< CTextureManager/*<CTexture>*/, CMapManager<CTexture>>("CTextureManager")
        .def(constructor<>())
        .def("get_resource", & CTextureManager::GetResource)
        .def("reload", & CTextureManager::Reload)
        ];

		//Camera Controller
		luabind::module(LUA_STATE) [
		class_< CCameraController>("CCameraController")
        .def(constructor<>())
        .def("add_new_camera", & CCameraController::AddNewCamera)
        .def("add_new_object", & CCameraController::AddNewObject)
		.def("update", (void (CCameraController::*)(float dt)) & CCameraController::Update)
		.def("update", (void (CCameraController::*)(std::string, float)) & CCameraController::Update)
		.def("get_active_camera", & CCameraController::getActiveCamera)
		.def("set_active_camera", (void (CCameraController::*)(CCamera *cam)) & CCameraController::setActiveCamera)
		.def("set_active_camera", (void (CCameraController::*)(std::string cam)) & CCameraController::setActiveCamera)
        ];

		RegisterLights();
		/*RegisterCameras();
		RegisterCinematics();
		RegisterEffects();
		RegisterAdvancedShaders();*/
}

