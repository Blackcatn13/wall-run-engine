#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

float4 DrawQuadSolidPS(in float2 UV:TEXCOORD0) : COLOR
{
	return float4(tex2D(S0LinearWrapSampler , UV).xyz, 1.0);
}

technique DrawQuadSolidTechnique
{
	pass p0
	{
		VertexShader = null;
		PixelShader = compile ps_3_0 DrawQuadSolidPS();
	}
}