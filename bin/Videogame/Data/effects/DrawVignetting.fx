#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

float4 DrawQuadSolidPS(in float2 UV:TEXCOORD0) : COLOR
{
	float4 finalTexture = tex2D(S0LinearWrapSampler, UV);
	if(g_UseVigneting == true){
		float2 l_Offset = float2(cos(g_Time), sin(g_Time));
		float2 l_UV = UV+l_Offset;
		float4 l_noise = tex2D(S1LinearWrapSampler, l_UV) * 0.1;
		float4 l_vig = tex2D(S2LinearWrapSampler, UV);
		//float l_vig = (1 - l_vig_aux) * 2;
		//return float4(0,1,0,1);
		finalTexture = float4(l_noise.xyz, l_vig.a);
	}
	return finalTexture;
}

technique DrawQuadSolidTechnique
{

	pass p0
	{
		VertexShader = null;
		PixelShader = compile ps_3_0 DrawQuadSolidPS();
	}
}