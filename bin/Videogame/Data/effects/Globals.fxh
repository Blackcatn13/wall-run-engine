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
float3 g_LightAngle[MAXLIGHTS] : LIGHTSANGLE;
float3 g_LightColor[MAXLIGHTS] : LIGHTSCOLOR;
//float g_LightIntensity[MAXLIGHTS] : LIGHTSINTENSITY;
float3 g_CameraPosition : CAMERAPOSITION;
float3x4 g_Bones[MAXBONES] : BONES;
float g_Time : TIME;

