#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"


float g_Bump = 3.0;

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

TGBUFFER_TEXTURED1_VERTEX_PS GBufferVS(VertexVS_TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX IN){
	
	TGBUFFER_TEXTURED1_VERTEX_PS OUT = (TGBUFFER_TEXTURED1_VERTEX_PS)0;
    
    OUT.HPosition = mul(float4(IN.Position,1.0), g_WorldViewProjectionMatrix);
	float3 l_WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	float3 l_WorldTangent = normalize(mul(IN.Tangent.xyz, (float3x3)g_WorldMatrix));
	float3 l_WorldBinormal = normalize(mul(IN.Binormal.xyz, (float3x3)g_WorldMatrix));
	float3 l_TangentNormalized = normalize(l_WorldTangent);
	float3 l_WNn= normalize(l_WorldNormal);
	float3 l_BinormalNormalized = normalize(l_WorldBinormal);
	float4 l_NormalTex = tex2D(S1LinearClampSampler, IN.UV);
	float3 l_Bump = g_Bump * (l_NormalTex.rgb - float3 (0.5,0.5,0.5));
	float3 l_Nn = l_WNn + l_Bump.x*l_TangentNormalized + l_Bump.y*l_BinormalNormalized;
	l_Nn = normalize(l_Nn);
	OUT.Normal = l_Nn;
	OUT.UV = IN.UV;
	OUT.WorldPosition = OUT.HPosition;
    return OUT;
}

TMultiRenderTargetPixel GBufferPS(TGBUFFER_TEXTURED1_VERTEX_PS IN) {
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	float4 l_DiffuseColor = tex2D(S0LinearWrapSampler , IN.UV);
	
	float3 Nn = normalize(IN.Normal);
	float3 NnScalated = Normal2Texture(Nn);
	// Cálculo de la z en formato color
	float l_Depth = IN.WorldPosition.z/IN.WorldPosition.w;
	
	OUT.RT0=float4(l_DiffuseColor.xyz,1.0);
	OUT.RT1=float4(l_DiffuseColor.xyz*g_LightAmbient*g_LightAmbientIntensity,1.0);
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