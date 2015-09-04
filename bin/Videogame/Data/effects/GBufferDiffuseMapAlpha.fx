#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"



struct TGBUFFER_TEXTURED1_VERTEX_PS
{
	float4 HPosition : POSITION;
    float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float4 WorldPosition : TEXCOORD2;
};

float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

TGBUFFER_TEXTURED1_VERTEX_PS GBufferVS(VertexVS_TTEXTURE_NORMAL_VERTEX IN){
	
	TGBUFFER_TEXTURED1_VERTEX_PS OUT = (TGBUFFER_TEXTURED1_VERTEX_PS)0;
	
    OUT.UV = IN.UV;
    if (g_ChangeUV) {
    	OUT.UV.x = IN.UV.x + 0.5;
    }
    OUT.HPosition = mul(float4(IN.Position,1.0), g_WorldViewProjectionMatrix);
	OUT.Normal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	OUT.WorldPosition = OUT.HPosition;
    return OUT;
}

TMultiRenderTargetPixel GBufferPS(TGBUFFER_TEXTURED1_VERTEX_PS IN) {
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	float4 l_DiffuseColor = tex2D(S0LinearWrapSampler , IN.UV);
	clip(l_DiffuseColor.a < 0.5f ? -1 : 1);
	float3 Nn = normalize(IN.Normal);
	float3 NnScalated = Normal2Texture(Nn);
	// Cálculo de la z en formato color
	float l_Depth = IN.WorldPosition.z/IN.WorldPosition.w;
	
	OUT.RT0=float4(l_DiffuseColor.xyz, 1.0);
	OUT.RT1=float4(l_DiffuseColor.xyz*g_LightAmbient*g_LightAmbientIntensity, 1.0);
	OUT.RT2.xyz=NnScalated;
	OUT.RT2.w = 1;
	OUT.RT3=l_Depth;  
	
	return OUT;
}


technique GBufferDiffuseMapTechniqueMovement
{
	pass p0
	{
		//AlphaBlendEnable = false;
		//CullMode = NONE; // NONE - CW
		VertexShader = compile vs_3_0 GBufferVS();
		PixelShader = compile ps_3_0 GBufferPS();
	}
}