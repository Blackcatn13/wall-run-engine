#include "EffectManager.h"
#include "EffectTechnique.h"
#include "Effect.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Logger.h"
#include "Utils\Defines.h"
#include "RenderableVertex\VertexTypes.h"

CEffectManager::CEffectManager()
{
	TTEXTURE_VERTEX::GetVertexDeclaration();
	TTEXTURE_NORMAL_VERTEX::GetVertexDeclaration();
	TCOLOR_TEXTURE_VERTEX::GetVertexDeclaration();
	TCOLOR_TEXTURE_NORMAL_VERTEX::GetVertexDeclaration();
	TTEXTURE2_VERTEX::GetVertexDeclaration();
	TTEXTURE2_NORMAL_VERTEX::GetVertexDeclaration();
	TCOLOR_TEXTURE2_VERTEX::GetVertexDeclaration();
	TCOLOR_NORMAL_DIFFUSE_TEXTURE2_VERTEX::GetVertexDeclaration();
	TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexDeclaration();
	TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexDeclaration();
	TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexDeclaration();
	TCOLORED_TEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexDeclaration();
	TCOLORED_VERTEX::GetVertexDeclaration();
	TCOLORED_NORMAL_VERTEX::GetVertexDeclaration();
}
CEffectManager::~CEffectManager()
{
	CleanUp();
}
const Mat44f & CEffectManager::GetWorldMatrix() const
{
	return m_WorldMatrix;
}
const Mat44f & CEffectManager::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}
const Mat44f & CEffectManager::GetViewMatrix() const
{
	return m_ViewMatrix;
}
const Mat44f & CEffectManager::GetViewProjectionMatrix()
{
	return m_ViewProjectionMatrix;
}
const Vect3f & CEffectManager::GetCameraEye()
{
	return m_CameraEye;
}
const Mat44f & CEffectManager::GetLightViewMatrix() const
{
	return m_LightViewMatrix;
}
const Mat44f & CEffectManager::GetShadowProjectionMatrix()
{
	return m_ShadowProjectionMatrix;
}
void CEffectManager::ActivateCamera(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix,const Vect3f &CameraEye)
{
	m_ViewMatrix = ViewMatrix;
	m_ProjectionMatrix = ProjectionMatrix;
	m_CameraEye = CameraEye;
}
void CEffectManager::SetWorldMatrix(const Mat44f &Matrix)
{
	m_WorldMatrix = Matrix;
}
void CEffectManager::SetProjectionMatrix(const Mat44f &Matrix)
{
	m_ProjectionMatrix = Matrix;
}
void CEffectManager::SetViewMatrix(const Mat44f &Matrix)
{
	m_ViewMatrix = Matrix;
}
void CEffectManager::SetViewProjectionMatrix(const Mat44f &ViewProjectionMatrix)
{
	m_ViewProjectionMatrix = ViewProjectionMatrix;
}
void CEffectManager::SetLightViewMatrix(const Mat44f &Matrix)
{
	m_LightViewMatrix = Matrix;
}
void CEffectManager::SetShadowProjectionMatrix(const Mat44f &Matrix)
{
	m_ShadowProjectionMatrix = Matrix;
}
void CEffectManager::SetCameraEye(const Vect3f &CameraEye)
{
	m_CameraEye = CameraEye;
}
void CEffectManager::Load(const std::string &FileName)
{
	 m_FileName = FileName;
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(FileName.c_str())) {
        printf("ERROR loading the file.");
    } else {
        CXMLTreeNode  m = newFile["effects"];
        if (m.Exists()) {
            int count = m.GetNumChildren();
            for (int i = 0; i < count; ++i) {
                std::string name = m(i).GetName();
                if (name == "default_technique") {
					int vType = m(i).GetIntProperty("vertex_type");
                    std::string techniqueName = m(i).GetPszISOProperty("technique", "");
					m_DefaultTechniqueEffectMap[vType] = techniqueName;
				}
				if (name == "effect") {
                    std::string effectName = m(i).GetPszISOProperty("name", "");
					std::string effectFile = m(i).GetPszISOProperty("file", "");
					CEffect *effect = new CEffect();
					effect->Load(effectFile);
					m_Effects.AddResource(effectName,effect);
				}
				if (name == "technique") {
                    std::string techniqueName = m(i).GetPszISOProperty("name", "");
					std::string effectName = m(i).GetPszISOProperty("effect", "");
					bool UseCameraPosition = m(i).GetBoolProperty("use_camera_position");
					bool UseInverseProjMatrix = m(i).GetBoolProperty("use_inverse_projection_matrix");
					bool UseInverseViewMatrix = m(i).GetBoolProperty("use_inverse_view_matrix");
					bool UseInverseWorldMatrix = m(i).GetBoolProperty("use_inverse_world_matrix");
					bool UseLights = m(i).GetBoolProperty("use_lights");
					int NumOfLights = m(i).GetIntProperty("num_of_lights");
					bool UseLightAmbientColor = m(i).GetBoolProperty("use_light_ambient_color");
					bool UseProjMatrix = m(i).GetBoolProperty("use_projection_matrix");
					bool UseViewMatrix = m(i).GetBoolProperty("use_view_matrix");
					bool UseWorldMatrix = m(i).GetBoolProperty("use_world_matrix");
					bool UseWorldViewMatrix = m(i).GetBoolProperty("use_world_view_matrix");
					bool UseWorldViewProjectionMatrix = m(i).GetBoolProperty("use_world_view_projection_matrix");
					bool UseViewProjectionMatrix = m(i).GetBoolProperty("use__view_projection_matrix");
					bool UseViewToLightProjectionMatrix = m(i).GetBoolProperty("use_view_to_light_projection_matrix");
					bool UseTime = m(i).GetBoolProperty("use_time");
					CEffect * effect = m_Effects.GetResource(effectName);
					if(effect != NULL)
					{
						CEffectTechnique *effectTechnique = new CEffectTechnique();
						effectTechnique->SetEffect(effect);
						effectTechnique->SetUseCameraPosition(UseCameraPosition);
						effectTechnique->SetUseInverseProjMatrix(UseInverseProjMatrix);
						effectTechnique->SetUseInverseViewMatrix(UseInverseViewMatrix);
						effectTechnique->SetUseInverseWorldMatrix(UseInverseWorldMatrix);
						effectTechnique->SetUseLights(UseLights);
						effectTechnique->SetNumOfLights(NumOfLights);
						effectTechnique->SetUseLightAmbientColor(UseLightAmbientColor);
						effectTechnique->SetUseProjMatrix(UseProjMatrix);
						effectTechnique->SetUseViewMatrix(UseViewMatrix);
						effectTechnique->SetUseWorldMatrix(UseWorldMatrix);
						effectTechnique->SetUseWorldViewMatrix(UseWorldViewMatrix);
						effectTechnique->SetUseWorldViewProjectionMatrix(UseWorldViewProjectionMatrix);
						effectTechnique->SetUseViewProjectionMatrix(UseViewProjectionMatrix);
						effectTechnique->SetUseViewToLightProjectionMatrix(UseViewToLightProjectionMatrix);
						effectTechnique->SetUseTime(UseTime);
						AddResource(techniqueName,effectTechnique);
					}else
					{
						LOGGER->AddNewLog(ELL_ERROR, "Not effect '%s' exists in the map", effectName.c_str());
					}
				}
            }
        }
    }
}
void CEffectManager::Reload()
{
	CleanUp();
	Load(m_FileName);
}
std::string CEffectManager::GetTechniqueEffectNameByVertexDefault(unsigned short VertexType)
{
	std::string techniqueName = m_DefaultTechniqueEffectMap[VertexType];
	return techniqueName;
}
size_t CEffectManager::GetMaxLights() const
{
	return MAX_LIGHTS_BY_SHADER;
}
CEffect * CEffectManager::GetEffect(const std::string &Name)
{
	CEffect *effect = m_Effects.GetResource(Name);
	return effect;
}
CEffectTechnique * CEffectManager::GetEffectTechnique(const std::string &Name)
{
	CEffectTechnique *effectTechnique = GetResource(Name);
	return effectTechnique;
}

void CEffectManager::CleanUp()
{
	m_DefaultTechniqueEffectMap.clear();
	m_Effects.Destroy();
	Destroy();
}