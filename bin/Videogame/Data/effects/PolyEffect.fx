#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

struct VNormalVertex 
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};


struct PNormalVertex
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

PNormalVertex RenderNormalsVS(VNormalVertex IN)
{
    PNormalVertex OUT = (PNormalVertex )0;
    OUT.pos=mul(float4(IN.pos.xyz,1),g_WorldViewProjectionMatrix);
    OUT.uv=IN.uv;
    OUT.normal=normalize(mul(IN.normal,(float3x3)g_WorldMatrix));

    return OUT;
}

float4 RenderNormalsPS20(PNormalVertex IN) : COLOR
{
	return float4(IN.normal, 1);
	return tex2D(S0LinearClampSampler, IN.uv);
	return float4(1,0,0,1);
}

technique BasicTechnique
{
    pass p0
    {
        VertexShader =compile vs_3_0 RenderNormalsVS();
        PixelShader = compile ps_3_0 RenderNormalsPS20();
    }
}