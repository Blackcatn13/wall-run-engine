#include "Effect.h"
#include "Lights\LightManager.h"
#include "Lights\Light.h"
#include "Lights\SpotLight.h"
#include "Core\Core.h"
#include "d3dx9.h"
#include <string>
#include <map>
#include "Core_Utils/MemLeaks.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "SceneElements\PolyPlatform.h"

void CEffect::SetNullParameters() {
  m_Effect = NULL;
  m_WorldMatrixParameter = NULL;
  m_ViewMatrixParameter = NULL;
  m_ProjectionMatrixParameter = NULL;
  m_WorldViewMatrixParameter = NULL;
  m_ViewProjectionMatrixParameter = NULL;
  m_WorldViewProjectionMatrixParameter = NULL;
  m_ViewToLightProjectionMatrixParameter = NULL;
  m_LightEnabledParameter = NULL;
  m_LightsTypeParameter = NULL;
  m_LightsPositionParameter = NULL;
  m_LightsDirectionParameter = NULL;
  m_LightsAngleParameter = NULL;
  m_LightsColorParameter = NULL;
  m_LightsFallOffParameter = NULL;
  m_LightsStartRangeAttenuationParameter = NULL;
  m_LightsEndRangeAttenuationParameter = NULL;
  m_LightsIntensityParameter = NULL;
  m_CameraPositionParameter = NULL;
  m_BonesParameter = NULL;
  m_TimeParameter = NULL;
  m_InverseWorldMatrixParameter = NULL;
  m_InverseViewMatrixParameter = NULL;
  m_InverseProjectionMatrixParameter = NULL;
  m_Light = NULL;
  m_UseShadowMaskTextureParameter = NULL;
  m_UseStaticShadowmapParameter = NULL;
  m_UseDynamicShadowmapParameter = NULL;
  m_PolyEnabledParameter = NULL;
}

void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle) {
  l_Handle = m_Effect->GetParameterBySemantic(NULL, SemanticName.c_str());
  if (l_Handle == NULL)
    LOGGER->AddNewLog(ELL_ERROR, "Parameter by semantic '%s' wasn't found on effect '%s'", SemanticName.c_str(), m_FileName.c_str());
}
bool CEffect::LoadEffect() {
  SetNullParameters();
  LPD3DXBUFFER l_ErrorBuffer = NULL;
  HRESULT l_HR = D3DXCreateEffectFromFile(GRAPHM->GetDevice(), m_FileName.c_str(), NULL, NULL, D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, NULL, &m_Effect, &l_ErrorBuffer);
  if (l_ErrorBuffer) {
    LOGGER->AddNewLog(ELL_ERROR, "Error creating effect '%s':\n%s", m_FileName.c_str(), l_ErrorBuffer->GetBufferPointer());
    CHECKED_RELEASE(l_ErrorBuffer);
    return false;
  }
  bool isOK = true;
  GetParameterBySemantic("WORLD", m_WorldMatrixParameter);
  GetParameterBySemantic("VIEW", m_ViewMatrixParameter);
  GetParameterBySemantic("PROJECTION", m_ProjectionMatrixParameter);
  GetParameterBySemantic("INVERSEWORLD", m_InverseWorldMatrixParameter);
  GetParameterBySemantic("INVERSEVIEW", m_InverseViewMatrixParameter);
  GetParameterBySemantic("INVERSEPROJECTION", m_InverseProjectionMatrixParameter);
  GetParameterBySemantic("WORLDVIEW", m_WorldViewMatrixParameter);
  GetParameterBySemantic("VIEWPROJECTION", m_ViewProjectionMatrixParameter);
  GetParameterBySemantic("WORLDVIEWPROJECTION", m_WorldViewProjectionMatrixParameter);
  GetParameterBySemantic("VIEWTOLIGHTPROJECTION", m_ViewToLightProjectionMatrixParameter);
  GetParameterBySemantic("LIGHTENABLED", m_LightEnabledParameter);
  GetParameterBySemantic("LIGHTSTYPE", m_LightsTypeParameter);
  GetParameterBySemantic("LIGHTSPOSITION", m_LightsPositionParameter);
  GetParameterBySemantic("LIGHTSDIRECTION", m_LightsDirectionParameter);
  GetParameterBySemantic("LIGHTSANGLE", m_LightsAngleParameter);
  GetParameterBySemantic("LIGHTSCOLOR", m_LightsColorParameter);
  GetParameterBySemantic("LIGHTSFALLOFF", m_LightsFallOffParameter);
  GetParameterBySemantic("LIGHTSSTARTRANGEATTENUATION", m_LightsStartRangeAttenuationParameter);
  GetParameterBySemantic("LIGHTSENDRANGEATTENUATION", m_LightsEndRangeAttenuationParameter);
  GetParameterBySemantic("LIGHTSINTENSITY", m_LightsIntensityParameter);
  GetParameterBySemantic("CAMERAPOSITION", m_CameraPositionParameter);
  GetParameterBySemantic("BONES", m_BonesParameter);
  GetParameterBySemantic("TIME", m_TimeParameter);
  GetParameterBySemantic("POLYENABLED", m_PolyEnabledParameter);
  GetParameterBySemantic("USESHADOWMAP", m_UseDynamicShadowmapParameter);
  return isOK;
}
void CEffect::Unload() {
  CHECKED_RELEASE(m_Effect);
  SetNullParameters();
}

CEffect::CEffect()
  : m_Effect(NULL)
  , m_WorldMatrixParameter(NULL)
  , m_ViewMatrixParameter(NULL)
  , m_ProjectionMatrixParameter(NULL)
  , m_WorldViewMatrixParameter(NULL)
  , m_ViewProjectionMatrixParameter(NULL)
  , m_WorldViewProjectionMatrixParameter(NULL)
  , m_ViewToLightProjectionMatrixParameter(NULL)
  , m_LightEnabledParameter(NULL)
  , m_LightsTypeParameter(NULL)
  , m_LightsPositionParameter(NULL)
  , m_LightsDirectionParameter(NULL)
  , m_LightsAngleParameter(NULL)
  , m_LightsColorParameter(NULL)
  , m_LightsFallOffParameter(NULL)
  , m_LightsStartRangeAttenuationParameter(NULL)
  , m_LightsEndRangeAttenuationParameter(NULL)
  , m_LightsIntensityParameter(NULL)
  , m_CameraPositionParameter(NULL)
  , m_BonesParameter(NULL)
  , m_TimeParameter(NULL)
  , m_InverseWorldMatrixParameter(NULL)
  , m_InverseViewMatrixParameter(NULL)
  , m_InverseProjectionMatrixParameter(NULL)
  , m_Light(NULL)
  , m_UseShadowMaskTextureParameter(NULL)
  , m_UseStaticShadowmapParameter(NULL)
  , m_UseDynamicShadowmapParameter(NULL) {
}

CEffect::~CEffect() {
  Unload();
}
bool CEffect::SetActivePoly( CPolyPlatform *poly) {

  m_PolyEnabled[0] =  poly->getEnabled();

  m_Effect->SetBoolArray(m_PolyEnabledParameter, &m_PolyEnabled[0], MAX_POLYS);
  return true;
}

bool CEffect::SetLight() {
  if (m_Light) {
    int l_type = m_Light->GetType();
    m_LightsType[0] = l_type;
    float l_angle = 0;
    if (l_type == CLight::TLightType::SPOT) {
      l_angle = ((CSpotLight *)m_Light)->GetAngle();
    }
    m_LightsAngle[0] = l_angle;
    float l_falloff = 0;
    if (l_type == CLight::TLightType::SPOT) {
      l_falloff = ((CSpotLight *)m_Light)->GetFallOff();
    }
    m_LightsFallOff[0] = l_falloff;
    float l_startRangeAtten = m_Light->GetStartRangeAttenuation();
    m_LightsStartRangeAttenuation[0] = l_startRangeAtten;
    float l_endRangeAtten = m_Light->GetEndRangeAttenuation();
    m_LightsEndRangeAttenuation[0] = l_endRangeAtten;
    float l_intensity = m_Light->GetIntensity();
    m_LightsIntensity[0] = l_intensity;
    Vect3f l_pos = m_Light->GetPosition();
    m_LightsPosition[0] = l_pos;
    Vect3f l_direction;
    if (l_type == CLight::TLightType::OMNI) {
      l_direction = Vect3f(0, 0, 0);
    } else {
      l_direction = ((CDirectionalLight *)m_Light)->GetDirection();
      l_direction = l_direction.Normalize();
    }
    m_LightsDirection[0] = l_direction;
    Vect3f l_color = Vect3f(m_Light->GetColor().GetRed(), m_Light->GetColor().GetGreen(), m_Light->GetColor().GetBlue()) ;
    m_LightsColor[0] = l_color;
    m_Effect->SetIntArray(m_LightsTypeParameter, &m_LightsType[0], MAX_LIGHTS_BY_SHADER);
    m_Effect->SetFloatArray(m_LightsAngleParameter, &m_LightsAngle[0], MAX_LIGHTS_BY_SHADER);
    m_Effect->SetFloatArray(m_LightsFallOffParameter, &m_LightsFallOff[0], MAX_LIGHTS_BY_SHADER);
    m_Effect->SetFloatArray(m_LightsIntensityParameter, &m_LightsIntensity[0], MAX_LIGHTS_BY_SHADER);
    m_Effect->SetFloatArray(m_LightsStartRangeAttenuationParameter, &m_LightsStartRangeAttenuation[0], MAX_LIGHTS_BY_SHADER);
    m_Effect->SetFloatArray(m_LightsEndRangeAttenuationParameter, &m_LightsEndRangeAttenuation[0], MAX_LIGHTS_BY_SHADER);
    m_Effect->SetFloatArray(m_LightsPositionParameter, &m_LightsPosition[0].x, MAX_LIGHTS_BY_SHADER * 3);
    m_Effect->SetFloatArray(m_LightsDirectionParameter, &m_LightsDirection[0].x, MAX_LIGHTS_BY_SHADER * 3);
    m_Effect->SetFloatArray(m_LightsColorParameter, &m_LightsColor[0].x, MAX_LIGHTS_BY_SHADER * 3);
    m_Light->BeginRenderEffectManagerShadowMap(this);
    return true;
  }
  return false;
}

bool CEffect::SetLights(size_t NumOfLights) {
  CLightManager::TMapResource::iterator it = LIGHTM->GetResources().begin();
  //std::map<std::string, CLight*>::iterator it = LIGHTM->GetResources().begin();
  int l_lightIndex = 0;
  std::map<std::string, CLight *> resources = LIGHTM->GetResources();
  while (it != LIGHTM->GetResources().end() && l_lightIndex < NumOfLights) {
    m_LightsEnabled[l_lightIndex] = 1;
    int l_type = it->second->GetType();
    m_LightsType[l_lightIndex] = l_type;
    float l_angle = 0;
    if (l_type == CLight::TLightType::SPOT) {
      l_angle = ((CSpotLight *)it->second)->GetAngle();
    }
    m_LightsAngle[l_lightIndex] = l_angle;
    float l_falloff = 0;
    if (l_type == CLight::TLightType::SPOT) {
      l_falloff = ((CSpotLight *)it->second)->GetFallOff();
    }
    m_LightsFallOff[l_lightIndex] = l_falloff;
    float l_startRangeAtten = it->second->GetStartRangeAttenuation();
    m_LightsStartRangeAttenuation[l_lightIndex] = l_startRangeAtten;
    float l_endRangeAtten = it->second->GetEndRangeAttenuation();
    m_LightsEndRangeAttenuation[l_lightIndex] = l_endRangeAtten;
    float l_intensity = it->second->GetIntensity();
    m_LightsIntensity[l_lightIndex] = l_intensity;
    Vect3f l_pos = it->second->GetPosition();
    m_LightsPosition[l_lightIndex] = l_pos;
    Vect3f l_direction;
    if (l_type == CLight::TLightType::OMNI) {
      l_direction = Vect3f(0, 0, 0);
    } else {
      l_direction = ((CDirectionalLight *)it->second)->GetDirection();
      l_direction = l_direction.Normalize();
    }
    m_LightsDirection[l_lightIndex] = l_direction;
    Vect3f l_color = Vect3f(it->second->GetColor().GetRed(), it->second->GetColor().GetGreen(), it->second->GetColor().GetBlue()) ;
    m_LightsColor[l_lightIndex] = l_color;
    it++;
    l_lightIndex += 1;
  }
  m_Effect->SetIntArray(m_LightsTypeParameter, &m_LightsType[0], MAX_LIGHTS_BY_SHADER);
  m_Effect->SetFloatArray(m_LightsAngleParameter, &m_LightsAngle[0], MAX_LIGHTS_BY_SHADER);
  m_Effect->SetFloatArray(m_LightsFallOffParameter, &m_LightsFallOff[0], MAX_LIGHTS_BY_SHADER);
  m_Effect->SetFloatArray(m_LightsIntensityParameter, &m_LightsIntensity[0], MAX_LIGHTS_BY_SHADER);
  m_Effect->SetFloatArray(m_LightsStartRangeAttenuationParameter, &m_LightsStartRangeAttenuation[0], MAX_LIGHTS_BY_SHADER);
  m_Effect->SetFloatArray(m_LightsEndRangeAttenuationParameter, &m_LightsEndRangeAttenuation[0], MAX_LIGHTS_BY_SHADER);
  m_Effect->SetFloatArray(m_LightsPositionParameter, &m_LightsPosition[0].x, MAX_LIGHTS_BY_SHADER * 3);
  m_Effect->SetFloatArray(m_LightsDirectionParameter, &m_LightsDirection[0].x, MAX_LIGHTS_BY_SHADER * 3);
  return true;
}
bool CEffect::Load(const std::string &FileName) {
  m_FileName = FileName;
  return LoadEffect();
}
bool CEffect::Reload() {
  Unload();
  bool isOK = LoadEffect();
  return isOK;
}
//DirectX Methods Interface
LPD3DXEFFECT CEffect::GetD3DEffect() const {
  return m_Effect;
}
D3DXHANDLE CEffect::GetTechniqueByName(const std::string &TechniqueName) {
  return m_Effect == NULL ? NULL : m_Effect->GetTechniqueByName(TechniqueName.c_str());
}

void CEffect::SetShadowMapParameters(bool UseShadowMaskTexture, bool UseStaticShadowmap, bool UseDynamicShadowmap) {
  m_Effect->SetBool(m_UseShadowMaskTextureParameter, UseShadowMaskTexture ? TRUE : FALSE);
  m_Effect->SetBool(m_UseStaticShadowmapParameter, UseStaticShadowmap ? TRUE : FALSE);
  m_Effect->SetBool(m_UseDynamicShadowmapParameter, UseDynamicShadowmap ? TRUE : FALSE);
}
