#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"



float4 RenderPolyPS() : COLOR
{
	return float4(0,1,0,0.5);	
}
technique PolyTechnique
{
	pass p0
	{
		/*AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;*/
		VertexShader = null;
		PixelShader=compile ps_3_0 RenderPolyPS();
	}
}