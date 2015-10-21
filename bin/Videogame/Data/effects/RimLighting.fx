#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

technique RenderLightDeferredShadingTechnique
{

	float4 RimLightPS(in float2 UV:TEXCOORD0) : COLOR 
	{
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