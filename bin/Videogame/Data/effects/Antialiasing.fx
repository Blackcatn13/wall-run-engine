#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

// Sampler0 -> frame
// Sampler1 -> sobel
int g_kSize = 3; // Size del kernel alrededor del pixel (g_kSize x g_kSize) TIENE QUE SER IMPAR
float4 AntialiasingPS(in float2 UV : TEXCOORD0):COLOR
{
	float3x3 l_I;
    float2 l_UV = float2(1.0/g_ScreenSize[0], 1.0 / g_ScreenSize[1]);
	float4 sobel = tex2D(S1LinearClampSampler, UV);
	float4 finalColor = float4(0,0,0,1);
	float sum = (sobel.x + sobel.y + sobel.z) / 3;
	if(sum > 0.9)
	{
		finalColor = tex2D(S0LinearClampSampler, UV);
	}
	else
	{
		for(int i = 0; i < g_kSize; i++)
		{
			for(int j = 0; j < g_kSize; j++)
			{
				finalColor += tex2D(S0LinearClampSampler, UV + float2(i * l_UV.x - (((g_kSize-1) / 2) * l_UV.x), j * l_UV.y - (((g_kSize-1) / 2) * l_UV.y)));
			}
		}
		finalColor = finalColor / (g_kSize * g_kSize);
	}
	
	return finalColor;
}

technique SobelTechnique
{
	pass p0
	{		
		PixelShader = compile ps_3_0 AntialiasingPS();
	}
}