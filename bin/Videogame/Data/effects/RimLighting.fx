#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

technique RenderLightDeferredShadingTechnique
{

	float4 RimLightPS(in float2 UV:TEXCOORD0) : COLOR 
	{
		float3 l_N = tex2D(S2LinearWrapSampler, UV).xyz;
		float3 l_Nn = normalize(Texture2Normal(l_N));
	}
	pass p0
	{
		//AlphaBlendEnable = true;
		//ZEnable = false;
		//ZWriteEnable = false;
		//BlendOp=Add;
		//SrcBlend = one;
		//DestBlend = one;
		VertexShader = null;
		PixelShader = compile ps_3_0 RimLightPS();
	}
}