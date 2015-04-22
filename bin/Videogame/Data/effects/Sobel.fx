#include "globals.fxh"
#include "samplers.fxh"

float g_SobelThreshold = 0.1;

float3x3 g_SobelX = float3x3
(
	float3(-1.0, 0.0, 1.0),
	float3(-2.0, 0.0, 2.0),
	float3(-1.0, 0.0, 1.0)
);

float3x3 g_SobelY = float3x3
(
	float3( 1.0,  2.0,  1.0),
	float3( 0.0,  0.0,  0.0),
	float3(-1.0, -2.0, -1.0)
);

float getGray(float4 c)
{
    return dot(c.xyz, float3(0.2126, 0.7152, 0.0722));
}

float4 SobelPS(in float2 UV : TEXCOORD0) : COLOR
{
    float3x3 l_I;
    float2 l_UV = float2(1.0/g_ScreenSize[0], 1.0 / g_ScreenSize[1]);
	
	for (int i = 0; i < 3; i++)	for(int j = 0; j < 3; j++)
	{
		l_I[i][j] = getGray(tex2D(S0LinearClampSampler, UV + float2(i * l_UV.x - l_UV.x, j * l_UV.y - l_UV.y)));
	}
	
	float l_SobelX = dot(g_SobelX[0], l_I[0]) + dot(g_SobelX[1], l_I[1]) + dot(g_SobelX[2], l_I[2]);
	float l_SobelY = dot(g_SobelY[0], l_I[0]) + dot(g_SobelY[1], l_I[1]) + dot(g_SobelY[2], l_I[2]);
       
    if(sqrt(l_SobelX * l_SobelX + l_SobelY * l_SobelY) > g_SobelThreshold) return float4(0.0, 0.0, 0.0, 1.0);
    else return float4(1.0, 1.0, 1.0, 1.0);
}


technique SobelTechnique
{
	pass p0
	{		
		PixelShader = compile ps_3_0 SobelPS();
	}
}