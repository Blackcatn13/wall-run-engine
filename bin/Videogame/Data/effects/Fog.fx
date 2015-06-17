#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"
#include "Functions.fxh"

float4 RenderQuadFogPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_DepthColor=tex2D(S0LinearWrapSampler, UV);
	float3 l_ViewPosition=GetPositionFromZDepthViewInViewCoordinates(l_DepthColor, UV, g_InverseProjectionMatrix);
	float l_Depth=length(l_ViewPosition);
	//return float4(0,0,0,0.8);
	return CalcExp2Fog(l_Depth, g_Exp2DensityFog, g_FogColor);
	
	
	//return CalcExpFog(l_Depth, g_ExpDensityFog, g_FogColor);
	//return CalcLinearFog(l_Depth, g_StartLinearFog, g_EndLinearFog, g_FogColor);
}
technique DrawQuadFogTechnique
{
	pass p0
	{
		/*AlphaBlendEnable = true;
		BlendOp=Add;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;*/
		PixelShader=compile ps_3_0 RenderQuadFogPS();
	}
}