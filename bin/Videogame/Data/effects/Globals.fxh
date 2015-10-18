#define MAXBONES 40
#define MAXLIGHTS 4
#define MAXPOLYS 10

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
int g_ScreenSize[2] : SCREENSIZE;
float g_LightIntensity[MAXLIGHTS] : LIGHTSINTENSITY;
float g_NearAtten[MAXLIGHTS] : LIGHTSSTARTRANGEATTENUATION;
float g_FarAtten[MAXLIGHTS] : LIGHTSENDRANGEATTENUATION;
float g_FallOff[MAXLIGHTS] : LIGHTSFALLOFF;
float g_DistanceFromPlayer[MAXLIGHTS] : DISTANCEFROMPLAYER;
float g_PolyEnabled[MAXPOLYS] : POLYENABLED;
bool g_useShadowMap : USESHADOWMAP;
int g_DynamicIntensity[MAXLIGHTS] : DYNAMICINTENSITY;
int g_nShadowmaps : NSHADOWMAPS;
float g_Tick : TICK;
bool g_ChangeUV : CHANGEUV; 
float g_VanishingModifier[1] : VANISHINGMODIFIER;

float g_ZBlurFocalStart = 7.0;
float g_ZBlurFocalEnd = 10.5;
float g_ZBlurEnd = 300;
float g_ConstantBlur = 0.05;
float2 g_RenderTargetSize = float2(754 , 584);
float g_ContourNormalThickness = 0.0;

float3 g_LightAmbient=float3(0.35, 0.35, 0.35);
float g_SpecularExponent = 200;
float g_LightAmbientIntensity = 1;

// Distancia inicio Fog lineal respecto a la cámara
float g_StartLinearFog = 5.0;
// Distancia final Fog lineal respecto a la cámara
float g_EndLinearFog = 20.0;
float4 g_FogColor = float4(0.05f,0.05f,0.05f,1.0f); 
float g_ExpDensityFog = 0.20;
float g_Exp2DensityFog = 0.02; //0.20
float g_ShadowEpsilon = 0.0005f;
// Fire values
float3 scrollSpeeds = float3(1.3f, 1.9f, 2.1f);
float3 scrollSpeedsLava = float3(0.3f, 0.5f, 0.8f);
float3 scales = float3(1.f, 2.f, 3.f);
float2 distortion1 = float2(0.1f, 0.2f);
float2 distortion2 = float2(0.1f, 0.3f);
float2 distortion3 = float2(0.1f, 0.1f);
float distortionScale = 0.8f;
float distortionBias = 0.5f;
// Lava values
float loopDuration = 0.1;
float displacement = 1.2;

