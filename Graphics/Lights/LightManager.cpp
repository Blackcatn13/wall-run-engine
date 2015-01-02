#include "Lights\LightManager.h"
#include "XML\XMLTreeNode.h"
#include <map>
#include "OmniLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "Utils\Logger.h"

CLightManager::CLightManager()
{}
CLightManager::~CLightManager()
{
	Destroy();
}

void CLightManager::Load(const std::string &FileName)
{
	std::string m_File = FileName;
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(FileName.c_str()))
	{
		std::string l_ErrorMsg = "ERROR loading file" + FileName;
		LOGGER->AddNewLog(ELL_ERROR,  l_ErrorMsg.c_str());
	//	printf("ERROR loading the file.");
	}
	CXMLTreeNode  m = newFile["lights"];
	if (m.Exists())
	{
		int count = m.GetNumChildren();
		for (int i = 0; i < count; ++i)
		{
			if(std::string("light")==m(i).GetName()) {
			//	XML type ejemplo
			/*	std::string l_Name = m(i).GetPszISOProperty("name", "");
				std::string l_Type = m(i).GetPszISOProperty("type", "");
				Vect3f l_Pos = m(i).GetVect3fProperty("pos", (0.0f,0.0f,0.0f));
				Vect3f l_Dir = m(i).GetVect3fProperty("dir", (0.0f,0.0f,0.0f));
				Vect3f l_Color = m(i).GetVect3fProperty("color", (0.0f,0.0f,0.0f));
				float l_StartRange = m(i).GetFloatProperty("att_start_range",0.0f);
				float l_EndRange = m(i).GetFloatProperty("att_end_range",0.0f);
				float l_Intensity = m(i).GetFloatProperty("intensity",0.0f);
				//Opcionales
				bool l_ExportShadows = m(i).GetBoolProperty("export_shadows",false);
				bool l_GenerateShadowMap = m(i).GetBoolProperty("generate_shadow_map",false);
				std::string l_ShadpwMapFormatType = m(i).GetPszISOProperty("shadow_map_format_type", "");
				int l_ShadowMapWith = m(i).GetIntProperty("shadow_map_width", 0);
				int l_ShadowMapHeight = m(i).GetIntProperty("shadow_map_height", 0);
				bool l_GenerateStaticShadowMap = m(i).GetBoolProperty("generate_static_shadow_map",false);
				std::string l_StaticShadowMapFormatType = m(i).GetPszISOProperty("static_shadow_map_format_type", "");
				int l_ShadowMapWith = m(i).GetIntProperty("shadow_map_width", 0);
				int l_StaticShadowMapFormatType = m(i).GetIntProperty("static_shadow_map_width", 0);
				int l_StaticShadowMapHeight = m(i).GetIntProperty("static_shadow_map_height", 0);
				bool l_UpdateStaticShadowMap = m(i).GetBoolProperty("update_static_shadow_map",false);
				int l_OrthoShadowMapWidht = m(i).GetIntProperty("ortho_shadow_map_width", 0);
				int l_OrthoShadowMapHeight = m(i).GetIntProperty("ortho_shadow_map_height", 0);
				bool l_UpdateStaticShadowMap = m(i).GetBoolProperty("update_static_shadow_map",false);
				bool l_UsesCascadedShadowsMap = m(i).GetBoolProperty("uses_cascaded_shadows_map",true);
				std::string l_CascadedMapFormatType = m(i).GetPszISOProperty("cascaded_shadow_map_format_type", "");
				int l_CascadedMapWith = m(i).GetIntProperty("cascaded_shadow_map_width", 0);
				int l_CascadedHeight = m(i).GetIntProperty("cascaded_shadow_map_height", 0);
				int l_OrthoShadowMapWidth0 = m(i).GetIntProperty("ortho_shadow_map_width_0", 0);
				int l_OrthoShadowMapHeight0 = m(i).GetIntProperty("ortho_shadow_map_heigth_0", 0);
				int l_OrthoShadowMapWidth1 = m(i).GetIntProperty("ortho_shadow_map_width_1", 0);
				int l_OrthoShadowMapHeight1 = m(i).GetIntProperty("ortho_shadow_map_heigth_1", 0);
				int l_OrthoShadowMapWidth2 = m(i).GetIntProperty("ortho_shadow_map_width_2", 0);
				int l_OrthoShadowMapHeight2 = m(i).GetIntProperty("ortho_shadow_map_heigth_2", 0);
				float l_ShadowMapEpsilon = m(i).GetFloatProperty("shadow_map_epsilon",0.0f);
				float l_CascadedMapEpsilon = m(i).GetFloatProperty("cascaded_epsilon",0.0f);
				int l_CascadedLightDistance = m(i).GetIntProperty("cascaded_light_distance", 0);
				std::vector<std::string> l_Cascade;
				if(m(i).GetNumChildren() >= 1)
				{
					for(int j = 0; j< m(i).GetNumChildren(); ++ j)
					{
						if(std::string("cascaded")==m(j).GetName()) {
							std::string l_RenderableObjectsManager = m(i).GetPszISOProperty("renderable_objects_manager", "");
							l_Cascade.push_back(l_RenderableObjectsManager);
						}
					}
				}

				*/
				//XML Type PDF
				std::string l_Name = m(i).GetPszISOProperty("name", "");
				std::string l_Type = m(i).GetPszISOProperty("type", "");
				Vect3f l_Pos = m(i).GetVect3fProperty("pos", (0.0f,0.0f,0.0f));
				Vect3f l_Dir = m(i).GetVect3fProperty("dir", (0.0f,0.0f,0.0f));
				Vect3f l_Color = m(i).GetVect3fProperty("color", (0.0f,0.0f,0.0f));
				float l_FallOff = m(i).GetFloatProperty("fall_off",0.0f);
				float l_Angle = m(i).GetFloatProperty("angle",0.0f);
				bool l_RenderShadows = m(i).GetFloatProperty("render_shadows",false);
				float l_StartRange = m(i).GetFloatProperty("att_start_range",0.0f);
				float l_EndRange = m(i).GetFloatProperty("att_end_range",0.0f);

				if(l_Type == "omni"){
					COmniLight * l_OmniLight = new COmniLight();
					l_OmniLight->SetName(l_Name);
					l_OmniLight->SetType(CLight::TLightType::OMNI);
					l_OmniLight->SetPosition(l_Pos);
					l_OmniLight->SetColor(CColor(l_Color));
					l_OmniLight->SetStartRangeAttenuation(l_StartRange);
					l_OmniLight->SetEndRangeAttenuation(l_EndRange);
					AddResource(l_Name, l_OmniLight);
				}
				else if (l_Type == "directional"){
					CDirectionalLight * l_DirectionalLight = new CDirectionalLight();
					l_DirectionalLight->SetName(l_Name);
					l_DirectionalLight->SetType(CLight::TLightType::DIRECTIONAL);
					l_DirectionalLight->SetPosition(l_Pos);
					l_DirectionalLight->SetColor(CColor(l_Color));
					l_DirectionalLight->SetStartRangeAttenuation(l_StartRange);
					l_DirectionalLight->SetEndRangeAttenuation(l_EndRange);
					l_DirectionalLight->SetDirection(l_Dir);

					AddResource(l_Name, l_DirectionalLight);		
				}else
				{
					CSpotLight * l_SpotLight = new CSpotLight();
					l_SpotLight->SetName(l_Name);
					l_SpotLight->SetType(CLight::TLightType::SPOT);
					l_SpotLight->SetPosition(l_Pos);
					l_SpotLight->SetColor(CColor(l_Color));
					l_SpotLight->SetStartRangeAttenuation(l_StartRange);
					l_SpotLight->SetEndRangeAttenuation(l_EndRange);
					l_SpotLight->SetDirection(l_Dir);
					l_SpotLight->SetAngle(l_Angle);
					l_SpotLight->SetFallOff(l_FallOff);

					AddResource(l_Name, l_SpotLight);
				}
			}
		} 
	}

}
void CLightManager::Render(CGraphicsManager *RenderManager)
{
	//TODO confirmar que tenga que hacer esto
	TMapResource::iterator it = m_Resources.begin();	
	TMapResource::iterator it_End = m_Resources.end();	
	 while (it != it_End) 
	 {
		 it->second->Render(RenderManager);
		++it;
    }
}

