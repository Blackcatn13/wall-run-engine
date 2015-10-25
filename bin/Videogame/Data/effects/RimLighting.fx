#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV, float4x4 InverseProjectionMatrix)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, InverseProjectionMatrix);
	// Divide by w to get the view-space position
	return l_PositionVS.xyz / l_PositionVS.w;
}

float3 GetPositionFromZDepthView(float ZDepthView, float2 UV, float4x4 InverseViewMatrix, float4x4 InverseProjectionMatrix)
{
	float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV, InverseProjectionMatrix);
	return mul(float4(l_PositionView,1.0), InverseViewMatrix).xyz;
}

float3 Texture2Normal(float3 Color)
{
	return (Color-0.5)*2;
}

float4 RimLightPS(in float2 UV:TEXCOORD0) : COLOR 
{
	float3 rim_color = float3(1,1,1);
	float3 l_EyePos = g_InverseViewMatrix[3].xyz;
	float3 l_N = tex2D(S2LinearWrapSampler, UV).xyz;
	float3 l_Nn = normalize(Texture2Normal(l_N));
	float l_Depth = tex2D(S3LinearWrapSampler, UV).x;
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, l_Depth, 1.0);
	float4 l_PositionVS = mul(l_ProjectedPos, g_InverseProjectionMatrix);
	
	if(l_PositionVS.w>=1.0)
		clip(-1);
	
	float3 l_PositionFromDepth = GetPositionFromZDepthView(l_Depth, UV, g_InverseViewMatrix, g_InverseProjectionMatrix);
	float3 l_DirVec = normalize(l_EyePos - l_PositionFromDepth);
	float rim_contrib = 1 - saturate(dot(l_DirVec,l_Nn));
	
	float3 final_color = rim_color * rim_contrib;
	
	return float4(final_color,1);
}

technique RimLightingTechnique
{
	
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