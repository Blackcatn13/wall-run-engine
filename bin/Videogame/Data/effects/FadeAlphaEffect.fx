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
	//return float4(IN.normal, 1);
	float4 l_DiffuseColor = tex2D(S0LinearClampSampler, IN.uv);
	float3 l_AuxColor = l_DiffuseColor.xyz;
	float currentAlpha = l_DiffuseColor.w * g_VanishingModifier[0];
	//currentAlpha -= modifier;
	//if(fadeOn == true){
	/*	if(currentAlpha >= 0)
			currentAlpha -= modifier;
		if (currentAlpha <= 0 ){
			fadeOn = false;
		}*/
	//}
	/*else{
		currentAlpha += modifier;
		if (currentAlpha >= 1 ){
			fadeOn = true;
		}
	}*/
	return float4(l_AuxColor, currentAlpha);
	//return tex2D(S0LinearClampSampler, IN.uv);
	//return float4(1,0,0,1);
}


//-----------------------------------------------------------------------------
// Techniques.
//-----------------------------------------------------------------------------

technique GlowTechnique
{
    pass
    {
		VertexShader =compile vs_3_0 RenderNormalsVS();
        PixelShader = compile ps_3_0 RenderNormalsPS20();
    }
}