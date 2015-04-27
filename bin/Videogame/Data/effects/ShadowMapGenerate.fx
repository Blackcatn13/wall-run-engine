#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

struct PNormalVertex
{
	float4 oPos : POSITION;
	float2 Depth : TEXCOORD0;
};

PNormalVertex VertShadow(float3 Pos : POSITION, float3 Normal : NORMAL)
{
	PNormalVertex OUT = (PNormalVertex)0;
	OUT.oPos = mul(float4(Pos, 1.0), g_WorldViewProjectionMatrix);
	OUT.Depth = OUT.oPos.zw;
	return OUT;
}

float4 PixShadow(PNormalVertex IN) : COLOR
{
	//float4 Color = IN.Depth.x / IN.Depth.y;
	//return float4(0,0,1,1);
	return IN.Depth.x / IN.Depth.y;
}

technique GenerateShadowMap
{
    pass p0
    {
        VertexShader = compile vs_3_0 VertShadow();
        PixelShader = compile ps_3_0 PixShadow();
    }
}