#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

float random_size = 64;
float g_scale = 10;
float g_bias = 0.6;
float g_intensity = 1;
float g_sample_rad = 0.000001; //radio


float3 getPosition(in float2 uv)
{
	float l_Depth = tex2D(S3LinearWrapSampler, uv).x;
	float x = uv.x * 2 - 1;
	float y = (1 - uv.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, l_Depth, 1.0);
	float4 l_PositionVS = mul(l_ProjectedPos, g_InverseProjectionMatrix);
	return l_PositionVS.xyz;
}

float3 getNormal(in float2 uv)
{
	return normalize(tex2D(S2LinearWrapSampler, uv).xyz * 2.0f - 1.0f);
}

float2 getRandom(in float2 uv)
{
	float2 l_screen_size = float2(g_ScreenSize[0],g_ScreenSize[1]);
	return normalize(tex2D(S4LinearWrapSampler, l_screen_size * uv / random_size).xy * 2.0f - 1.0f);
}

float doAmbientOcclusion(in float2 tcoord,in float2 uv, in float3 p, in float3 cnorm)
{
	float3 diff = getPosition(tcoord + uv) - p;
	const float3 v = normalize(diff);
	const float d = length(diff)*g_scale;
	return max(0.0,dot(cnorm,v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}

float4 SSAOPS(in float2 UV:TEXCOORD0) : COLOR 
{

	float l_EffectAffect = tex2D(S0LinearWrapSampler, UV).w;
	float3 final_color = float3(1,1,1);
	const float2 vec[20] = 
	{
	float2(1,0),
	float2(0.9781,0.2079),
	float2(0.9135,0.4067),
	float2(0.5,0.8660),
	float2(0.3090,0.9510),
	float2(0,1),
	float2(0.2079,-0.9781),
	float2(0.4067,-0.9135),
	float2(0.5877,-0.8090),
	float2(0.8660,-0.5000),
	float2(-1,0),
	float2(-0.9781,-0.2079),
	float2(-0.9135,-0.4067),
	float2(-0.5,-0.8660),
	float2(-0.3090,-0.9510),
	float2(0,-1),
	float2(-0.2079,0.9781),
	float2(-0.4067,0.9135),
	float2(-0.8660,0.5000),
	float2(-0.9510,0.3090)
	};

	float3 p = getPosition(UV);
	float3 n = getNormal(UV);
	float2 rand = getRandom(UV);

	float ao = 0.0f;
	float rad = g_sample_rad/p.z;

	//**SSAO Calculation**//
	int iterations = 20;
	for (int j = 0; j < iterations; ++j)
	{
	  float2 coord1 = reflect(vec[j],rand)*rad;
	  //float2 coord2 = float2(coord1.x*0.707 - coord1.y*0.707, coord1.x*0.707 + coord1.y*0.707);
	  
	  ao += doAmbientOcclusion(UV,coord1, p, n);
	  //ao += doAmbientOcclusion(UV,coord2*0.5, p, n);
	  //ao += doAmbientOcclusion(UV,coord1*0.75, p, n);
	  //ao += doAmbientOcclusion(UV,coord2, p, n);
	}
	ao/=(float)iterations;

	if(l_EffectAffect > 0.5)
	{
		final_color = final_color * ao;
	}else
	{
		final_color = 0;
	}
	
	//Do stuff here with your occlusion value Ã¢aoÃ¢: modulate ambient lighting, write it to a buffer for later //use, etc.
	return float4(final_color,1);
}

technique SSAOTechnique
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
		PixelShader = compile ps_3_0 SSAOPS();
	}
}