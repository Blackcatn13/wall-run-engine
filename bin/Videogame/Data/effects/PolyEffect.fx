#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"



float4 RenderPolyPS(in float2 UV : TEXCOORD0) : COLOR
{
	
	//for(int i=0;i<MAXPOLYS;i++){
		
	float4 finalColor = float4(0,1,0,0.5);
			
	
	//float4 color = float4(1,0,0,1);
	//float4 l_DiffuseColor = tex2D(S0LinearWrapSampler, UV);
	return finalColor;
	//return float4(1,0,0,0.5);
	//return CalcExpFog(l_Depth, g_ExpDensityFog, g_FogColor);
	//return CalcLinearFog(l_Depth, g_StartLinearFog, g_EndLinearFog, g_FogColor);
}
technique PolyTechnique
{
	pass p0
	{
		/*AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;*/
		PixelShader=compile ps_3_0 RenderPolyPS();
	}
}