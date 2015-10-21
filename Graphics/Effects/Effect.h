#pragma once
#ifndef INC_EFFECT_H_
#define INC_EFFECT_H_

#define MAX_LIGHTS_BY_SHADER 4
#define MAX_POLYS 10

#include <string>
#include "d3d9.h"
#include "d3dx9shader.h"
#include "Math\Vector3.h"

class CLight;

class CPolyPlatform;
class CEffect {
 private:
  std::string m_FileName;
  LPD3DXEFFECT m_Effect;
  BOOL m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
  int m_LightsType[MAX_LIGHTS_BY_SHADER];
  float m_LightsAngle[MAX_LIGHTS_BY_SHADER];
  float m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
  float m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
  float m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
  float m_LightsIntensity[MAX_LIGHTS_BY_SHADER];
  BOOL	m_DynamicIntensity[MAX_LIGHTS_BY_SHADER];
  float m_LightDistanceFromPlayer[MAX_LIGHTS_BY_SHADER];
  float m_VanishingModifier[1];
  Vect3f m_LightsPosition[MAX_LIGHTS_BY_SHADER];
  Vect3f m_LightsDirection[MAX_LIGHTS_BY_SHADER];
  Vect3f m_LightsColor[MAX_LIGHTS_BY_SHADER];
  D3DXHANDLE m_WorldMatrixParameter, m_ViewMatrixParameter, m_ProjectionMatrixParameter;
  D3DXHANDLE m_InverseWorldMatrixParameter, m_InverseViewMatrixParameter, m_InverseProjectionMatrixParameter;
  D3DXHANDLE m_WorldViewMatrixParameter, m_ViewProjectionMatrixParameter, m_WorldViewProjectionMatrixParameter;
  D3DXHANDLE m_ViewToLightProjectionMatrixParameter;
  D3DXHANDLE m_LightEnabledParameter, m_LightsTypeParameter, m_LightsPositionParameter, m_LightsDirectionParameter, m_LightsAngleParameter, m_LightsColorParameter;
  D3DXHANDLE m_LightsFallOffParameter, m_LightsStartRangeAttenuationParameter, m_LightsEndRangeAttenuationParameter, m_LightsIntensityParameter, m_LightDynamicIntensityParameter, m_LightDistanceFromPlayerParameter;
  D3DXHANDLE m_CameraPositionParameter;
  D3DXHANDLE m_BonesParameter;
  D3DXHANDLE m_TimeParameter, m_TickParameter, m_ChangeUVParameter;
  D3DXHANDLE m_ScreenSizeParameter;
  D3DXHANDLE m_UseVignetingParameter;
  D3DXHANDLE m_VanishingModifierParameter;
  CLight *m_Light;
  ////
  D3DXHANDLE m_UseShadowMaskTextureParameter;
  D3DXHANDLE m_UseStaticShadowmapParameter;
  D3DXHANDLE m_UseDynamicShadowmapParameter;
  D3DXHANDLE m_NShadowmapsParameter;

  BOOL m_PolyEnabled[MAX_POLYS];
  D3DXHANDLE m_PolyEnabledParameter;

  ////
  void SetNullParameters();
  void GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle);
  bool LoadEffect();
  void Unload();
 public:
  CEffect();
  ~CEffect();
  bool SetLights(size_t NumOfLights);
  bool SetLight();
  bool SetVanishingModifier();
  void ChangeLight(CLight *Light) {
    m_Light = Light;
  }
  bool Load(const std::string &FileName);
  bool Reload();
  bool SetActivePoly (CPolyPlatform *poly);
  void SetShadowMapParameters(bool UseShadowMaskTexture, bool UseStaticShadowmap, bool UseDynamicShadowmap, int nshadomaps);
  //Getters
  BOOL *GetLightsEnabled() {
    return m_LightsEnabled;
  }
  int *GetLightsType() {
    return m_LightsType;
  }
  float *GetLightsAngle() {
    return m_LightsAngle;
  }
  float *GetLightsFallOff() {
    return m_LightsFallOff;
  }
  float *GetLightsStartRangeAttenuation() {
    return m_LightsStartRangeAttenuation;
  }
  float *GetLightsEndRangeAttenuation() {
    return m_LightsEndRangeAttenuation;
  }
  float *GetLightsIntensity() {
    return m_LightsIntensity;
  }

  BOOL *GetLightsDynamicIntensity() {
    return m_DynamicIntensity;
  }

  Vect3f *GetLightsPosition() {
    return m_LightsPosition;
  }
  Vect3f *GetLightsDirection() {
    return m_LightsDirection;
  }
  Vect3f *GetLightsColor() {
    return m_LightsColor ;
  }

  float *GetVanishingModifier() {
    return m_VanishingModifier;
  }

  //DirectX Methods Interface
  LPD3DXEFFECT GetD3DEffect() const;
  D3DXHANDLE GetTechniqueByName(const std::string &TechniqueName);
  D3DXHANDLE GetWorldMatrixParameter() {
    return m_WorldMatrixParameter;
  }
  D3DXHANDLE GetViewMatrixParameter() {
    return m_ViewMatrixParameter;
  }
  D3DXHANDLE GetProjectionMatrixParameter() {
    return m_ProjectionMatrixParameter;
  }
  D3DXHANDLE GetWorldViewMatrixParameter() {
    return m_WorldViewMatrixParameter;
  }
  D3DXHANDLE GetViewProjectionMatrixParameter() {
    return m_ViewProjectionMatrixParameter;
  }
  D3DXHANDLE GetWorldViewProjectionMatrixParameter() {
    return m_WorldViewProjectionMatrixParameter;
  }
  D3DXHANDLE GetViewToLightProjectionMatrixParameter() {
    return m_ViewToLightProjectionMatrixParameter;
  }
  D3DXHANDLE GetLightEnabledParameter() {
    return m_LightEnabledParameter;
  }
  D3DXHANDLE GetLightsTypeParameter() {
    return m_LightsTypeParameter;
  }
  D3DXHANDLE GetLightsPositionParameter() {
    return m_LightsPositionParameter;
  }
  D3DXHANDLE GetLightsDirectionParameter() {
    return m_LightsDirectionParameter;
  }
  D3DXHANDLE GetLightsAngleParameter() {
    return m_LightsAngleParameter;
  }
  D3DXHANDLE GetLightsColorParameter() {
    return m_LightsColorParameter;
  }
  D3DXHANDLE GetLightsFallOffParameter() {
    return m_LightsFallOffParameter;
  }
  D3DXHANDLE GetLightDynamicIntensityParameter() {
    return m_LightDynamicIntensityParameter;
  }

  D3DXHANDLE GetLightsStartRangeAttenuationParameter() {
    return m_LightsStartRangeAttenuationParameter;
  }
  D3DXHANDLE GetLightsEndRangeAttenuationParameter() {
    return m_LightsEndRangeAttenuationParameter;
  }
  D3DXHANDLE GetCameraPositionParameter() {
    return m_CameraPositionParameter;
  }
  D3DXHANDLE GetBonesParameter() {
    return m_BonesParameter;
  }
  D3DXHANDLE GetTimeParameter() {
    return m_TimeParameter;
  }
  D3DXHANDLE GetTickParameter() {
    return m_TickParameter;
  }
  D3DXHANDLE GetInverseWorldMatrixParameter() {
    return m_InverseWorldMatrixParameter;
  }
  D3DXHANDLE GetInverseViewMatrixParameter() {
    return m_InverseViewMatrixParameter;
  }
  D3DXHANDLE GetInverseProjectionMatrixParameter() {
    return m_InverseProjectionMatrixParameter;
  }
  D3DXHANDLE GetNShadowmapsParameter() {
    return m_NShadowmapsParameter;
  }
  D3DXHANDLE GetScreenSizeParameter() {
    return m_ScreenSizeParameter;
  }
  D3DXHANDLE GetChangeUVParameter() {
    return m_ChangeUVParameter;
  }
  D3DXHANDLE GetVanishingModifierParameter() {
    return m_VanishingModifierParameter;
  }
  D3DXHANDLE GetUseVignetingParameter() {
    return m_UseVignetingParameter;
  }
};

#endif //INC_EFFECT_H_