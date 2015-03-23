#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"


PNormalVertex RenderPolyVS(VNormalVertex IN)
{
    PNormalVertex OUT = (PNormalVertex )0;
    OUT.pos=mul(float4(IN.pos.xyz,1),g_WorldViewProjectionMatrix);
    OUT.uv=IN.uv;
    OUT.normal=normalize(mul(IN.normal,(float3x3)g_WorldMatrix));

    return OUT;
}

float4 RenderPolyPS20(PNormalVertex IN) : COLOR
{
	return float4(1,0,0,0.5);
	return float4(IN.normal, 1);
	return tex2D(S0LinearWrapSampler, IN.uv);
	
}

technique BasicTechnique
{
    pass p0
    {
        VertexShader =compile vs_3_0 RenderPolyVS();
        PixelShader = compile ps_3_0 RenderPolyPS20();
    }
}