#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"


struct PSVertex
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

struct PARTICLEIN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float2 params : TEXCOORD2;
};

PSVertex VS(PARTICLEIN IN){
	
	float3 rightVector = normalize(float3(g_ViewMatrix[0][0], g_ViewMatrix[1][0], g_ViewMatrix[2][0]));
	float3 upVector = normalize(float3(g_ViewMatrix[0][1], g_ViewMatrix[1][1], g_ViewMatrix[2][1]));
	float3 viewVector = normalize(float3(g_ViewMatrix[0][2], g_ViewMatrix[1][2], g_ViewMatrix[2][2]));

	PSVertex OUT = (PSVertex)0;
	if (IN.params.y == 1) {
		OUT.uv = IN.uv;
		float3 position = (IN.pos.x * rightVector + IN.pos.z*upVector) * IN.params.x;
		OUT.pos = mul(float4(position,1.0), g_WorldViewProjectionMatrix);
		OUT.normal = float4(viewVector.xyz, 1);
	}
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