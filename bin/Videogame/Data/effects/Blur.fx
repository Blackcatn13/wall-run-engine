#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"
#include "Functions.fxh"

float4 BlurPS(in float2 UV : TEXCOORD0) : COLOR
{
	float3 Color;
	float Percent = 0.5;
    Color = tex2D(S1LinearWrapSampler, UV.xy).xyz;
	
    return float4(Color, Percent);
}


technique ZBlurTechnique
{
	pass p0
	{
		//AlphaBlendEnable = false;
		//CullMode = CCW;
		PixelShader = compile ps_3_0 BlurPS();
	}
}