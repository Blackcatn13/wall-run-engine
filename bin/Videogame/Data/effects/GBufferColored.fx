#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"



struct TGBUFFER_COLORED_NORMAL_VERTEX_PS
{
	float4 HPosition : POSITION;
	float4 Col : TEXCOORD0;
	float3 Normal	: TEXCOORD1;
	float4 WorldPosition : TEXCOORD2;
};

float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

TGBUFFER_COLORED_NORMAL_VERTEX_PS GBufferVS(VertexVS_TCOLORED_NORMAL_VERTEX IN){
	
	TGBUFFER_COLORED_NORMAL_VERTEX_PS OUT = (TGBUFFER_COLORED_NORMAL_VERTEX_PS)0;
    OUT.HPosition = mul(float4(IN.Position,1.0), g_WorldViewProjectionMatrix);
    OUT.Normal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	OUT.WorldPosition = OUT.HPosition;
	OUT.Col = IN.Color;
    return OUT;
}

TMultiRenderTargetPixel GBufferPS(TGBUFFER_COLORED_NORMAL_VERTEX_PS IN) {
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	float4 l_DiffuseColor = IN.Col;
	
	float3 Nn = normalize(IN.Normal);
	float3 NnScalated = Normal2Texture(Nn);
	// Cálculo de la z en formato color
	float l_Depth = IN.WorldPosition.z/IN.WorldPosition.w;
	
	OUT.RT0= float4(l_DiffuseColor.xyz, 1.0);
	OUT.RT1= float4(l_DiffuseColor.xyz*g_LightAmbient*g_LightAmbientIntensity, 1.0);
	OUT.RT2.xyz=NnScalated;
	OUT.RT2.w = 1;
	OUT.RT3=l_Depth;  
	
	return OUT;
}


technique GBufferDiffuseMapTechnique
{
	pass p0
	{
		//AlphaBlendEnable = false;
		//CullMode = NONE; // NONE - CW
		VertexShader = compile vs_3_0 GBufferVS();
		PixelShader = compile ps_3_0 GBufferPS();
	}
}