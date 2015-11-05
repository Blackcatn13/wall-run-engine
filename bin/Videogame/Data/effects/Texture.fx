#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"


struct PSVertex
{
    float4 pos : POSITION;
    //float3 normal : TEXCOORD1;
    float2 uv : TEXCOORD0;
    float alpha : TEXCOORD2;
};

struct PARTICLEIN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float3 params : TEXCOORD2;
};

PSVertex VS(PARTICLEIN IN) {
	PSVertex OUT = (PSVertex)0;

	if (IN.params.y > 0 && IN.params.x < 6.1 && IN.params.x > 0) {

		float3 rightVector = normalize(float3(g_ViewMatrix[0][0], g_ViewMatrix[1][0], g_ViewMatrix[2][0]));
		float3 upVector = normalize(float3(g_ViewMatrix[0][1], g_ViewMatrix[1][1], g_ViewMatrix[2][1]));
		float3 viewVector = normalize(float3(g_ViewMatrix[0][2], g_ViewMatrix[1][2], g_ViewMatrix[2][2]));

		OUT.uv = float2(1,1) - IN.uv;
		float sn = sin(0.0);
		float cs = cos(0.0);
		//float3 nUpVector = cs * rightVector - sn * upVector;
		//float3 nRightVector = sn * rightVector + cs * upVector;
		float3 position = IN.worldPos + (IN.pos.x * rightVector + IN.pos.z * upVector) * IN.params.x;
		OUT.pos = mul(float4(position,1.0), g_WorldViewProjectionMatrix);
		OUT.alpha = IN.params.y;
		//float3 normal = float3(0,1,0);
		//OUT.normal ==normalize(mul(normal.xyz,(float3x3)g_WorldMatrix));
	}
    return OUT;
}

float4 PS(PSVertex IN) : COLOR
{
	float4 color = tex2D(S0PointClampSampler, IN.uv);
	return float4(color.rgb, color.a * IN.alpha);
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