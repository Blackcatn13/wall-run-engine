#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

float OriginalIntensity = 1.0;
float BloomSaturation = 1.0;
float OriginalSaturation = 1.0;
float BloomIntensity = 5.0;

// This combines the bloom texture with the original scene texture.

// Our bloom texture
//sampler BloomSampler : register(s0);

// Our original SceneTexture
//texture ColorMap;

//En nuestro globals es:
//S0LinearWrapSampler:register(s0)
// Create a sampler for the ColorMap texture using lianear filtering and clamping
//sampler ColorMapSampler = sampler_state
//{
//   Texture = <ColorMap>;
//   MinFilter = Linear;
//   MagFilter = Linear;
//   MipFilter = Linear;  
//   AddressU  = Clamp;
//   AddressV  = Clamp;
//};

float4 AdjustSaturation(float4 color, float saturation)
{
    // We define gray as the same color we used in the grayscale shader
    float grey = float(dot(color, float3(0.3, 0.59, 0.11)));
    return float4(lerp(grey, color, saturation));
}

float4 PixelShader(in float2 UV:TEXCOORD0) : COLOR
{
    // Get our original pixel from ColorMap
    //float4 originalColor = float4(tex2D(S0LinearWrapSampler, texCoord));
	float4 originalColor = tex2D(S0LinearWrapSampler,UV);
	
    // Get our bloom pixel from bloom texture
    //float4 bloomColor = float4(tex2D(S1LinearWrapSampler, texCoord));
	float4 bloomColor = tex2D(S1LinearWrapSampler,UV);
	
    // Adjust color saturation and intensity based on the input variables to the shader
    bloomColor = AdjustSaturation(bloomColor, BloomSaturation) * BloomIntensity;
	originalColor = float4(AdjustSaturation(originalColor, OriginalSaturation) * OriginalIntensity);

	//bloomColor = saturate(bloomColor);
	originalColor = saturate(originalColor);
	
    // make the originalColor darker in very bright areas, avoiding these areas look burned-out
    originalColor = originalColor * (float4(1.0,1.0,1.0,1.0) - bloomColor);

    // Combine the two images.
    return originalColor + bloomColor;
	//float4 Result = float4(1.0,0.0,0.0,1.0);
	//return bloomColor;
}

technique CombineBloom
{
    pass P0
    {
        // A post process shader only needs a pixel shader.
		VertexShader = null;
        PixelShader = compile ps_3_0 PixelShader();
    }
}