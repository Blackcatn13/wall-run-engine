#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

static const float PI = 3.14159265f;

struct TGBUFFER_TEXTURED1_VERTEX_PS
{
	float4 HPosition : POSITION;
    float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD2;
	float4 WorldPosition : TEXCOORD3;
	float2 UV1 : TEXCOORD4;
};


float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

TGBUFFER_TEXTURED1_VERTEX_PS GBufferVS(VertexVS_TTEXTURE_NORMAL_VERTEX IN){
	
	TGBUFFER_TEXTURED1_VERTEX_PS OUT = (TGBUFFER_TEXTURED1_VERTEX_PS)0;
	float r = sin((g_Tick / loopDuration) * 2 * PI) * 0.5f + 0.25f;
	float g = sin((g_Tick / loopDuration + 0.33333333f) * 2 * PI) * 0.5f + 0.25f;
	float b = sin((g_Tick / loopDuration + 0.66666667f) * 2 * PI) * 0.5f + 0.25f;
	float correction = 1 / (r + g + b);
	r *= correction;
	g *= correction;
	b *= correction;
    OUT.UV = IN.UV;
    float4 texturePos = float4(IN.UV.xy, 0.0f, 0.0f);
    float4 dcolor = tex2Dlod(S1LinearWrapSampler, texturePos);
    float d = dcolor.r * r + dcolor.g * g + dcolor.b * b;
    float3 newPosition = IN.Position + IN.Normal * d * displacement;
    OUT.HPosition = mul(float4(newPosition.xyz,1.0), g_WorldViewProjectionMatrix);
	OUT.Normal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	OUT.WorldPosition = OUT.HPosition;
	OUT.UV1 = IN.UV;
	OUT.UV1.y = OUT.UV1.y - (g_Tick * scrollSpeeds.z);
    return OUT;
}

TMultiRenderTargetPixel GBufferPS(TGBUFFER_TEXTURED1_VERTEX_PS IN) {
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	float4 l_DiffuseColor = tex2D(S0LinearWrapSampler , IN.UV1);
	clip(l_DiffuseColor.a < 0.1f ? -1 : 1);
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

technique GBufferDiffuseMapTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 GBufferVS();
		PixelShader = compile ps_3_0 GBufferPS();
	}
}