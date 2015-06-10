#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"


struct PSVertex
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};


PSVertex VS(VertexVS_TTEXTURE_NORMAL_VERTEX IN){
	
	PSVertex OUT = (PSVertex)0;
    OUT.uv = IN.UV;
    OUT.pos = mul(float4(IN.Position,1.0), g_WorldViewProjectionMatrix);
    return OUT;
}

float4 PS(PSVertex IN) : COLOR
{
	return float4(1,0,0,1);
	return tex2D(S0LinearClampSampler, IN.uv);
}


technique TextureTechnique
{
	pass p0
	{
		//AlphaBlendEnable = false;
		//CullMode = NONE; // NONE - CW
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}