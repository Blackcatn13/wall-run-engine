#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

//#define RADIUS  7
//#define KERNEL_SIZE (RADIUS * 2 + 1)
#define KERNEL_SIZE 9

//-----------------------------------------------------------------------------
// Globals.
//-----------------------------------------------------------------------------

//float weights[KERNEL_SIZE];
//float2 offsets[KERNEL_SIZE];

float2 offsets[KERNEL_SIZE] =
	{
		float2(-1,1), float2(0,1), float2(1,1), float2(-1,0), float2(0,0), float2(0,1), float2(-1,-1), float2(0,-1), float2(1,-1)
	};
//float2 offsets[KERNEL_SIZE] =
//	{
//		float2(0,-3),float2(0,-2),float2(0,-1),float2(0,0),float2(0,1),float2(0,2),float2(0,3)
//	};
	
float weights[KERNEL_SIZE] =
	{
		float(0.077847), float(0.123317), float(0.077847), float(0.123317), float(0.195346), float(0.123317), float(0.077847), float(0.123317) , float(0.077847)
	};

//float weights[KERNEL_SIZE] =
//	{
//		float(0.3), float(0.15), float(0.05), float(0.0), float(0.05), float(0.15), float(0.3)
//	};

//-----------------------------------------------------------------------------
// Textures.
//-----------------------------------------------------------------------------

//texture colorMapTexture;

//sampler2D colorMap = sampler_state
//{
//    Texture = <colorMapTexture>;
//    MipFilter = Linear;
//    MinFilter = Linear;
//    MagFilter = Linear;
//};

//-----------------------------------------------------------------------------
// Pixel Shaders.
//-----------------------------------------------------------------------------

float4 PS_GaussianBlur(in float2 UV : TEXCOORD0) : COLOR
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    for (int i = 0; i < KERNEL_SIZE; ++i)
        color += float4(tex2D(S0LinearClampSampler, UV + offsets[i]) * weights[i]);
    return color;
}

//-----------------------------------------------------------------------------
// Techniques.
//-----------------------------------------------------------------------------

technique GaussianBlur
{
    pass
    {
		VertexShader = null;
        PixelShader = compile ps_3_0 PS_GaussianBlur();
    }
}
