#include "Lights\LightManager.h"
#include "XML\XMLTreeNode.h"
#include <map>
#include "OmniLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "Utils\Logger.h"

#include "Renderable\RenderableObjectsManager.h"

CLightManager::CLightManager()
{}
CLightManager::~CLightManager()
{
	Destroy();
}

void CLightManager::Load(const std::string &FileName)
{
	m_File = FileName;
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
	/*		//	XML type ejemplo
				std::string l_Name = m(i).GetPszISOProperty("name", "");
				std::string l_Type = m(i).GetPszISOProperty("type", "");
				Vect3f l_Pos = m(i).GetVect3fProperty("pos", (0.0f,0.0f,0.0f));
				Vect3f l_Dir = m(i).GetVect3fProperty("dir", (0.0f,0.0f,0.0f));
				Vect3f l_Color = m(i).GetVect3fProperty("color", (0.0f,0.0f,0.0f));
				float l_StartRange = m(i).GetFloatProperty("att_start_range",0.0f);
				float l_EndRange = m(i).GetFloatProperty("att_end_range",0.0f);
				float l_Intensity = m(i).GetFloatProperty("intensity",0.0f); 
				//Opcionales
			//	bool l_ExportShadows = m(i).GetBoolProperty("export_shadows",false); //<= Este no está en el xml del pdf
				bool l_GenerateDynamicShadowMap = m(i).GetBoolProperty("generate_shadow_map",false);
				std::string l_ShadowMapFormatType = m(i).GetPszISOProperty("shadow_map_format_type", "");
				int l_ShadowMapWith = m(i).GetIntProperty("shadow_map_width", 0);
				int l_ShadowMapHeight = m(i).GetIntProperty("shadow_map_height", 0);
				bool l_GenerateStaticShadowMap = m(i).GetBoolProperty("generate_static_shadow_map",false);
				std::string l_StaticShadowMapFormatType = m(i).GetPszISOProperty("static_shadow_map_format_type", "");
				int l_StaticShadowMapWidth = m(i).GetIntProperty("static_shadow_map_width", 0);
				int l_StaticShadowMapHeight = m(i).GetIntProperty("static_shadow_map_height", 0);
				bool l_UpdateStaticShadowMap = m(i).GetBoolProperty("update_static_shadow_map",false);
				float l_Angle =  m(i).GetFloatProperty("angle", 0.0f);
				float l_FallOff =  m(i).GetFloatProperty("fall_off", 0.0f);
				std::string l_ShadowTextureMask = m(i).GetPszISOProperty("shadow_texture_mask", "");

				std::string l_StaticRenderableObjectsManager, l_DynamicRenderableObjectsManager;
				int countChild = m(i).GetNumChildren();
				for (int j= 0; j<countChild; ++j)
				{
					if(std::string("static")==m(i)(j).GetName())
					{
						l_StaticRenderableObjectsManager = m(i)(j).GetPszISOProperty("renderable_objects_manager", "");
					}
					else if(std::string("dynamic")==m(i)(j).GetName())
					{
						l_DynamicRenderableObjectsManager = m(i)(j).GetPszISOProperty("renderable_objects_manager", "");
					}
				}

				*/

				/*int l_OrthoShadowMapWidht = m(i).GetIntProperty("ortho_shadow_map_width", 0);										//<= Esto no está en el xml del pdf pero hará falta
				int l_OrthoShadowMapHeight = m(i).GetIntProperty("ortho_shadow_map_height", 0);										//<= Esto no está en el xml del pdf pero hará falta
				bool l_UpdateStaticShadowMap = m(i).GetBoolProperty("update_static_shadow_map",false);								//<= Esto no está en el xml del pdf pero hará falta
				bool l_UsesCascadedShadowsMap = m(i).GetBoolProperty("uses_cascaded_shadows_map",true);								//<= Esto no está en el xml del pdf pero hará falta
				std::string l_CascadedMapFormatType = m(i).GetPszISOProperty("cascaded_shadow_map_format_type", "");				//<= Esto no está en el xml del pdf pero hará falta
				int l_CascadedMapWith = m(i).GetIntProperty("cascaded_shadow_map_width", 0);										//<= Esto no está en el xml del pdf pero hará falta
				int l_CascadedHeight = m(i).GetIntProperty("cascaded_shadow_map_height", 0);										//<= Esto no está en el xml del pdf pero hará falta
				int l_OrthoShadowMapWidth0 = m(i).GetIntProperty("ortho_shadow_map_width_0", 0);									//<= Esto no está en el xml del pdf pero hará falta
				int l_OrthoShadowMapHeight0 = m(i).GetIntProperty("ortho_shadow_map_heigth_0", 0);									//<= Esto no está en el xml del pdf pero hará falta
				int l_OrthoShadowMapWidth1 = m(i).GetIntProperty("ortho_shadow_map_width_1", 0);									//<= Esto no está en el xml del pdf pero hará falta
				int l_OrthoShadowMapHeight1 = m(i).GetIntProperty("ortho_shadow_map_heigth_1", 0);									//<= Esto no está en el xml del pdf pero hará falta
				int l_OrthoShadowMapWidth2 = m(i).GetIntProperty("ortho_shadow_map_width_2", 0);									//<= Esto no está en el xml del pdf pero hará falta
				int l_OrthoShadowMapHeight2 = m(i).GetIntProperty("ortho_shadow_map_heigth_2", 0);									//<= Esto no está en el xml del pdf pero hará falta
				float l_ShadowMapEpsilon = m(i).GetFloatProperty("shadow_map_epsilon",0.0f);										//<= Esto no está en el xml del pdf pero hará falta
				float l_CascadedMapEpsilon = m(i).GetFloatProperty("cascaded_epsilon",0.0f);										//<= Esto no está en el xml del pdf pero hará falta
				int l_CascadedLightDistance = m(i).GetIntProperty("cascaded_light_distance", 0);									//<= Esto no está en el xml del pdf pero hará falta
				std::vector<std::string> l_Cascade;																					//<= Esto no está en el xml del pdf pero hará falta
				if(m(i).GetNumChildren() >= 1)																						//<= Esto no está en el xml del pdf pero hará falta
				{																													//<= Esto no está en el xml del pdf pero hará falta
					for(int j = 0; j< m(i).GetNumChildren(); ++ j)																	//<= Esto no está en el xml del pdf pero hará falta
					{																												//<= Esto no está en el xml del pdf pero hará falta
						if(std::string("cascaded")==m(j).GetName()) {																//<= Esto no está en el xml del pdf pero hará falta
							std::string l_RenderableObjectsManager = m(i).GetPszISOProperty("renderable_objects_manager", "");		//<= Esto no está en el xml del pdf pero hará falta
							l_Cascade.push_back(l_RenderableObjectsManager);														//<= Esto no está en el xml del pdf pero hará falta
						}																											//<= Esto no está en el xml del pdf pero hará falta
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
				float l_Intensity = m(i).GetFloatProperty("intensity",0.0f); 
				
				if(l_Type == "omni"){
					COmniLight * l_OmniLight = new COmniLight();
					l_OmniLight->SetName(l_Name);
					l_OmniLight->SetType(CLight::TLightType::OMNI);
					l_OmniLight->SetPosition(l_Pos);
					l_OmniLight->SetColor(CColor(l_Color));
					l_OmniLight->SetIntensity(l_Intensity);
					l_OmniLight->SetStartRangeAttenuation(l_StartRange);
					l_OmniLight->SetEndRangeAttenuation(l_EndRange);
					/////////////////////////////////
	/*				l_OmniLight->SetGenerateDynamicShadowMap(l_GenerateDynamicShadowMap);
					l_OmniLight->SetGenerateStaticShadowMap(l_GenerateStaticShadowMap);
					l_OmniLight->SetMustUpdateStaticShadowMap(l_UpdateStaticShadowMap);
					
					//Faltan Format Type (static y dynamic), ShadowMap Heigh y with (static y dynamic), shadowTexTuremask
					if(l_StaticRenderableObjectsManager != "")
					{
						CRenderableObjectsManager * l_RenderableObjectManager = new  CRenderableObjectsManager();
						//Aqui se le añadirá al CRenderableObjectsManager el valor de l_StaticRenderableObjectsManager
						l_OmniLight->GetStaticShadowMapRenderableObjectsManagers().push_back(l_RenderableObjectManager);
					}
					if(l_DynamicRenderableObjectsManager != "")
					{
						CRenderableObjectsManager * l_RenderableObjectManager = new  CRenderableObjectsManager();
						//Aqui se le añadirá al CRenderableObjectsManager el valor de l_DynamicRenderableObjectsManager
						l_OmniLight->GetDynamicShadowMapRenderableObjectsManagers().push_back(l_RenderableObjectManager);
					}
*/
					AddResource(l_Name, l_OmniLight);
				}
				else if (l_Type == "directional"){
					CDirectionalLight * l_DirectionalLight = new CDirectionalLight();
					l_DirectionalLight->SetName(l_Name);
					l_DirectionalLight->SetType(CLight::TLightType::DIRECTIONAL);
					l_DirectionalLight->SetPosition(l_Pos);
					l_DirectionalLight->SetColor(CColor(l_Color));
					l_DirectionalLight->SetIntensity(l_Intensity);
					l_DirectionalLight->SetStartRangeAttenuation(l_StartRange);
					l_DirectionalLight->SetEndRangeAttenuation(l_EndRange);
					l_DirectionalLight->SetDirection(l_Dir);

					/////////////////////////////////
	/*				l_DirectionalLight->SetGenerateDynamicShadowMap(l_GenerateDynamicShadowMap);
					l_DirectionalLight->SetGenerateStaticShadowMap(l_GenerateStaticShadowMap);
					l_DirectionalLight->SetMustUpdateStaticShadowMap(l_UpdateStaticShadowMap);
					l_DirectionalLight->SetOrthoShadowMapSize((l_ShadowMapWith, l_ShadowMapHeight));
					//Faltan Format Type (static y dynamic), ShadowMap Heigh y with ( dynamic), shadowTexTuremask
					if(l_StaticRenderableObjectsManager != "")
					{
						CRenderableObjectsManager * l_RenderableObjectManager = new  CRenderableObjectsManager();
						//Aqui se le añadirá al CRenderableObjectsManager el valor de l_StaticRenderableObjectsManager
						l_DirectionalLight->GetStaticShadowMapRenderableObjectsManagers().push_back(l_RenderableObjectManager);
					}
					if(l_DynamicRenderableObjectsManager != "")
					{
						CRenderableObjectsManager * l_RenderableObjectManager = new  CRenderableObjectsManager();
						//Aqui se le añadirá al CRenderableObjectsManager el valor de l_DynamicRenderableObjectsManager
						l_DirectionalLight->GetDynamicShadowMapRenderableObjectsManagers().push_back(l_RenderableObjectManager);
					}
*/
					AddResource(l_Name, l_DirectionalLight);		
				}else
				{
					CSpotLight * l_SpotLight = new CSpotLight();
					l_SpotLight->SetName(l_Name);
					l_SpotLight->SetType(CLight::TLightType::SPOT);
					l_SpotLight->SetPosition(l_Pos);
					l_SpotLight->SetColor(CColor(l_Color));
					l_SpotLight->SetIntensity(l_Intensity);
					l_SpotLight->SetStartRangeAttenuation(l_StartRange);
					l_SpotLight->SetEndRangeAttenuation(l_EndRange);
					l_SpotLight->SetDirection(l_Dir);
					l_SpotLight->SetAngle(l_Angle);
					l_SpotLight->SetFallOff(l_FallOff);

					/////////////////////////////////
		/*			l_SpotLight->SetGenerateDynamicShadowMap(l_GenerateDynamicShadowMap);
					l_SpotLight->SetGenerateStaticShadowMap(l_GenerateStaticShadowMap);
					l_SpotLight->SetMustUpdateStaticShadowMap(l_UpdateStaticShadowMap);
					l_SpotLight->SetOrthoShadowMapSize((l_ShadowMapWith, l_ShadowMapHeight));
					//Faltan Format Type (static y dynamic), ShadowMap Heigh y with ( dynamic), shadowTexTuremask
					if(l_StaticRenderableObjectsManager != "")
					{
						CRenderableObjectsManager * l_RenderableObjectManager = new  CRenderableObjectsManager();
						//Aqui se le añadirá al CRenderableObjectsManager el valor de l_StaticRenderableObjectsManager
						l_SpotLight->GetStaticShadowMapRenderableObjectsManagers().push_back(l_RenderableObjectManager);
					}
					if(l_DynamicRenderableObjectsManager != "")
					{
						CRenderableObjectsManager * l_RenderableObjectManager = new  CRenderableObjectsManager();
						//Aqui se le añadirá al CRenderableObjectsManager el valor de l_DynamicRenderableObjectsManager
						l_SpotLight->GetDynamicShadowMapRenderableObjectsManagers().push_back(l_RenderableObjectManager);
					}
					*/
					AddResource(l_Name, l_SpotLight);
				}
			}
		} 
	}

}
void CLightManager::Render(CGraphicsManager *RenderManager)
{
	//El render de las luces solo es para debug
	#ifdef _DEBUG
	{ 
		TMapResource::iterator it = m_Resources.begin();	
		TMapResource::iterator it_End = m_Resources.end();	
		 while (it != it_End) 
		 {
			 it->second->Render(RenderManager);
			++it;
		}
	}
	#endif
}

void CLightManager::Reload()
{
	m_Resources.clear();
	Load(m_File);
}