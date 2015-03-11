#define MAXBONES 40
#define MAXLIGHTS 4

float4x4 g_WorldMatrix : WORLD;
float4x4 g_ViewMatrix : VIEW;
float4x4 g_ProjectionMatrix : PROJECTION;
float4x4 g_InverseWorldMatrix : INVERSEWORLD;
float4x4 g_InverseViewMatrix: INVERSEVIEW;
float4x4 g_InverseProjectionMatrix : INVERSEPROJECTION;
float4x4 g_WorldViewMatrix : WORLDVIEW;
float4x4 g_ViewProjectionMatrix : VIEWPROJECTION;
float4x4 g_WorldViewProjectionMatrix : WORLDVIEWPROJECTION;
float4x4 g_ViewToLightProjectionMatrix : VIEWTOLIGHTPROJECTION;
int g_LightEnabled[MAXLIGHTS] : LIGHTENABLED;
int g_LightType[MAXLIGHTS] : LIGHTSTYPE;
float3 g_LightPosition[MAXLIGHTS] : LIGHTSPOSITION;
float3 g_LightDirection[MAXLIGHTS] : LIGHTSDIRECTION;
float g_LightAngle[MAXLIGHTS] : LIGHTSANGLE;
float3 g_LightColor[MAXLIGHTS] : LIGHTSCOLOR;
float3 g_CameraPosition : CAMERAPOSITION;
float3x4 g_Bones[MAXBONES] : BONES;
float g_Time : TIME;
float g_LightIntensity[MAXLIGHTS] : LIGHTSINTENSITY;
float g_NearAtten[MAXLIGHTS] : LIGHTSSTARTRANGEATTENUATION;
float g_FarAtten[MAXLIGHTS] : LIGHTSENDRANGEATTENUATION;
float g_FallOff[MAXLIGHTS] : LIGHTSFALLOFF;

float g_ZBlurFocalStart = 7.0;
float g_ZBlurFocalEnd = 10.5;
float g_ZBlurEnd = 300;
float g_ConstantBlur = 0.05;
float2 g_RenderTargetSize = float2(754 , 584);

float3 g_LightAmbient=float3(0.3, 0.35, 0.4);
float g_SpecularExponent = 200;
float g_LightAmbientIntensity = 1;

// Distancia inicio Fog lineal respecto a la cámara
float g_StartLinearFog = 2.0;
// Distancia final Fog lineal respecto a la cámara
float g_EndLinearFog = 10.0;
float4 g_FogColor = float4(0.7f,0.7,0.7f,1.0f); 
float g_ExpDensityFog = 0.20;
float g_Exp2DensityFog = 0.20;
float g_ShadowEpsilon = 0.0000005f;

