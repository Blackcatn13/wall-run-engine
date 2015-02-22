#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

float g_ReflectionContribution = 0.3;

struct TGBUFFER_TEXTURED1_VERTEX_PS
{
	float4 HPosition : POSITION;
    float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float4 WorldPosition : TEXCOORD2;
};

// Estructura de píxel que contiene la info de 4 píxeles de salida
struct TMultiRenderTargetPixel
{
	float4 RT0 : COLOR0; //Albedo (float3) + (float) SpecularFactor
	float4 RT1 : COLOR1; //AmbientLight (float3) + (float) SpecularPow
	float4 RT2 : COLOR2; //Normal (float3) + (float) Not used
	float4 RT3 : COLOR3; //Depth (float4)
};

float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

TGBUFFER_TEXTURED1_VERTEX_PS GBufferVS(VertexVS_TTEXTURE_NORMAL_VERTEX IN){
	
	TGBUFFER_TEXTURED1_VERTEX_PS OUT = (TGBUFFER_TEXTURED1_VERTEX_PS)0;
    OUT.UV = IN.UV;
    OUT.HPosition = mul(float4(IN.Position,1.0), g_WorldViewProjectionMatrix);
	OUT.Normal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	OUT.WorldPosition = OUT.HPosition;
    return OUT;
}

TMultiRenderTargetPixel GBufferPS(TGBUFFER_TEXTURED1_VERTEX_PS IN) {
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	float4 l_DiffuseColor = tex2D(S0LinearWrapSampler , IN.UV);
	float3 Nn = normalize(IN.Normal);
	float3 l_EyePos=g_InverseViewMatrix[3].xyz;
	float3 l_CameraToPixel=normalize(l_EyePos-IN.WorldPosition);
	float3 l_ReflectVector=reflect(l_CameraToPixel, Nn);
	float4 l_Cube=texCUBE(S2LinearWrapSampler, -l_ReflectVector);
	float3 l_Color = l_DiffuseColor.xyz + l_Cube * g_ReflectionContribution;
	
	float3 NnScalated = Normal2Texture(Nn);
	// Cálculo de la z en formato color
	float l_Depth = IN.WorldPosition.z/IN.WorldPosition.w;
	
	OUT.RT0=float4(l_Color, 1.0);
	OUT.RT1=float4(l_Color*g_LightAmbient*g_LightAmbientIntensity, 1.0);
	OUT.RT2.xyz=NnScalated;
	OUT.RT3=l_Depth;  
	
	return OUT;
}


technique GBufferDiffuseMapTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		//CullMode = NONE; // NONE - CW
		VertexShader = compile vs_3_0 GBufferVS();
		PixelShader = compile ps_3_0 GBufferPS();
	}
}