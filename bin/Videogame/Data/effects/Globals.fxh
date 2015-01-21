#define MAXBONES 40
float4x4 g_WorldViewProj : WorldViewProjection;
float4x4 g_WorldMatrix : World; 
float4x4 g_InverseViewMatrix: ViewInverse;
float3x4 g_Bones[MAXBONES] : Bones;
