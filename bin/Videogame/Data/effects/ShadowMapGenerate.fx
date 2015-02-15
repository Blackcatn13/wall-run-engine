#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

struct PNormalVertex
{
	float4 oPos : POSITION;
	float2 Depth : TEXCOORD0;
};

PNormalVertex VertShadow(float4 Pos: POSITION,
	float3 Normal: NORMAL)
{
	PNormalVertex OUT = (PNormalVertex)0;
	OUT.oPos = mul(Pos, g_WorldViewProj);
	OUT.Depth.xy = oPpos.zw;
	return OUT;
}

float4 PixShadow(PNormalVertex IN) : COLOR
{
	Color = IN.Depth.x / IN.Depth.y;
	return Color;
}

technique tec0
{
    pass p0
    {
        VertexShader =compile vs_3_0 VertShadow();
        PixelShader = compile ps_3_0 PixShadow();
    }
}