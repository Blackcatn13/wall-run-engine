#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"
#include "Functions.fxh"

float4 ZBlurPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_DepthMap = float4(tex2D(S0LinearWrapSampler , UV).xyz, 1.0);
	float3 l_WorldPosition=GetPositionFromZDepthView(l_DepthMap, float2(0,0),g_InverseViewMatrix, g_InverseProjectionMatrix);
	float l_Distance=length(l_WorldPosition-g_CameraPosition);
	float4 l_Color=float4(0,0,0,0);
	float l_Blur=0.0;
	if(l_Distance<g_ZBlurFocalEnd)
		l_Blur=max(l_Distance/g_ZBlurFocalStart, g_ConstantBlur);
	else if(l_Distance>g_ZBlurFocalEnd)
		l_Blur=max(1.0-(l_Distance-g_ZBlurFocalEnd)/g_ZBlurEnd, g_ConstantBlur);
	
	const float2 delta[8] =
	{
		float2(-1,1),float2(1,-1),float2(-1,1),float2(1,1),float2(-1,0),float2(1,0),float2(0,-1),float2(0,1)
	};
	float2 l_PixelInc=4*1/g_RenderTargetSize; //4 pixeles a la redonda
	float4 l_AlbedoColor=tex2D(S1LinearWrapSampler,UV);
	for( int i=0;i<8;i++ )
		l_Color += tex2D(S1LinearWrapSampler,UV + delta[i]*l_PixelInc)*l_Blur+(1.0-l_Blur)*l_AlbedoColor;
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