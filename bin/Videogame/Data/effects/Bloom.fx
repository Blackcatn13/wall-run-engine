#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

float BloomThreshold = 0.9;
float BloomIntensity = 1.0;

//float4 BloomPixelShader(float2 texCoord : TEXCOORD0) : COLOR0
float4 BloomPixelShader(in float2 UV:TEXCOORD0) : COLOR
{
    float4 Color = float4(tex2D(S0LinearWrapSampler , UV).xyz, 1.0);
    // Conseguir las zonas iluminadas que son más brillantes que el threshold y devolverlas.
	//return saturate((Color – Threshold) / (1 – BloomThreshold));
	float4 Bloom = saturate((Color - float4(BloomThreshold,BloomThreshold,BloomThreshold,0)) / (1-BloomThreshold));
	//float4 Result = saturate(Color + (Bloom * BloomIntensity));
	return Bloom;
}
 

technique Bloom
{
    pass P0
    {
        // AlphaBlendEnable = false;
        //CullMode = NONE; // NONE - CW
        // Un efecto de post procesado solo necesita un pixel shading.
		VertexShader = null;
        PixelShader = compile ps_3_0 BloomPixelShader();
    }
} 
