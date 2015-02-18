#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"



struct SCREEN_TEXTURED_COLORED_VERTEX_PS 
{
    float4 HPosition : POSITION;
    float2 UV : TEXCOORD0;
};


SCREEN_TEXTURED_COLORED_VERTEX_PS DrawQuadVS(SCREEN_TEXTURED_COLORED_VERTEX_VS IN){
	
	SCREEN_TEXTURED_COLORED_VERTEX_PS OUT = (SCREEN_TEXTURED_COLORED_VERTEX_PS)0;
    OUT.UV = IN.UV;
    OUT.HPosition = mul(float4(IN.Position,1.0), g_WorldViewProjectionMatrix);
    return OUT;
}

float4 DrawQuadPS(SCREEN_TEXTURED_COLORED_VERTEX_PS IN) : COLOR {
	//return float4(0.0,1.0,0.0,1.0); 
	//return float4(IN.UV.y, 0.0, 0.0, 1.0);
	float4 l_DiffuseMat = tex2D(S0LinearWrapSampler , IN.UV);
	return l_DiffuseMat;  // Devolvemos textura del canal 0
}

technique DrawQuadTechnique
{
	pass p0 {
		AlphaBlendEnable = false;
		CullMode = NONE;
		VertexShader = null;
		PixelShader = compile ps_3_0 DrawQuadPS();
	}
}