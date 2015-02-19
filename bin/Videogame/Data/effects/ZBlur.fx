#include "Globals.fxh"
#include "VertexTypes.fxh"

float4 ZBlurPS(in float2 UV : TEXCOORD0) : COLOR
{
	return float4(0,0,0,0);
	float l_DepthMap=tex2D(S1LinearSampler,UV).x;
	float3 l_WorldPosition=GetPositionFromZDepthView(l_DepthMap, UV,g_InverseViewMatrix,g_InverseProjectionMatrix);
	float l_Distance=length(l_WorldPosition-g_EyePos);
	float4 l_Color=float4(0,0,0,0);
	float l_Blur=0.0;
	
	if(l_Distance>g_ZBlurFocalStart)
		l_Blur=g_ConstantBlur;//max(l_Distance/g_ZBlurFocalStart, g_ConstantBlur);
	if(l_Distance>g_ZBlurFocalEnd)
		l_Blur=saturate((l_Distance-g_ZBlurFocalEnd)/(g_ZBlurEnd-g_ZBlurFocalEnd));
		
	const float2 delta[8] =
	{
		float2(-1,1), float2(1,-1), float2(-1,1), float2(1,1), float2(-1,0), float2(1,0), float2(0,-1), float2(0,1)
	};
	
	float2 l_PixelInc=float2(1/1280.0, 1/720.0);
	float4 l_AlbedoColor=tex2D(S0PointSampler,UV);
	for( int i=0;i<8;i++ )
		l_Color += tex2D(S0PointSampler,UV + delta[i]*l_PixelInc)*l_Blur+(1.0-l_Blur)*l_AlbedoColor;
	l_Color = l_Color*(1.0/8.0);
	
	return l_Color;
}


technique ZBlurTechnique
{
	pass p0
	{
		//AlphaBlendEnable = false;
		//CullMode = CCW;
		PixelShader = compile ps_3_0 ZBlurPS();
	}
}