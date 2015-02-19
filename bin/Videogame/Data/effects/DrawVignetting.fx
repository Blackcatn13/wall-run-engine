#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

float4 DrawQuadSolidPS(in float2 UV:TEXCOORD0) : COLOR
{
	float3 l_diff = tex2D(S0LinearWrapSampler, UV).rgb;
	float3 l_noise = tex2D(S1LinearWrapSampler, UV * 25).rgb;
	float l_vig_aux = tex2D(S2LinearWrapSampler, UV).a;
	float l_vig = (1 - l_vig_aux) * 2;
	return float4(l_diff * l_noise * l_vig, 1);
}

technique DrawQuadSolidTechnique
{

	pass p0
	{
		//AlphaBlendEnable = true;
		VertexShader = null;
		PixelShader = compile ps_3_0 DrawQuadSolidPS();
	}
}