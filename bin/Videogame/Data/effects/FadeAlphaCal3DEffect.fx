#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"
#include "Functions.fxh"

struct PNormalVertex
{
    float4 oPos : POSITION;
    float2 UV : TEXCOORD0;
   // float3 normal : NORMAL;
};

PNormalVertex RenderNormalsVS(CAL3D_HW_VERTEX_VS IN)
{
	PNormalVertex OUT = (PNormalVertex)0;
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	OUT.oPos = mul(float4(l_Position, 1.0), g_WorldViewProjectionMatrix);
	OUT.UV.x = IN.TexCoord.x;
	OUT.UV.y = 1 - IN.TexCoord.y;
	//OUT.UV=IN.UV;
	return OUT;
}

float4 RenderNormalsPS20(PNormalVertex IN) : COLOR
{
	//return float4(IN.normal, 1);
	float4 l_DiffuseColor = tex2D(S0LinearClampSampler, IN.UV);
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
	//return tex2D(S0LinearClampSampler, IN.UV);
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