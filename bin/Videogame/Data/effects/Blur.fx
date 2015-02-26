#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"
#include "Functions.fxh"

float4 BlurPS(in float2 UV : TEXCOORD0) : COLOR
{
	//return float4(1,0,0,0.5);
//	float3 Color;
	float Percent = 0.8;
	float4 Color = tex2D(S0LinearWrapSampler, UV.xy);
	float3 PrevColor = tex2D(S1LinearWrapSampler, UV.xy).xyz;
	//return Color;
	//return float4(PrevColor, 1);
	
	float4 FinalColor = float4(Color)*0.7+ float4(PrevColor,Percent)*0.3;
    return FinalColor;
}


technique ZBlurTechnique
{
	pass p0
	{
		//CullMode = CCW;
		PixelShader = compile ps_3_0 BlurPS();
	}
}