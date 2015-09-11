#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

struct PSVertex
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

struct BILLBOARDIN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float4 params : TEXCOORD2;
};

PSVertex VS(BILLBOARDIN IN){
	
	float3 rightVector = normalize(float3(g_ViewMatrix[0][0], g_ViewMatrix[1][0], g_ViewMatrix[2][0]));
	float3 upVector = normalize(float3(g_ViewMatrix[0][1], g_ViewMatrix[1][1], g_ViewMatrix[2][1]));
	float3 viewVector = normalize(float3(g_ViewMatrix[0][2], g_ViewMatrix[1][2], g_ViewMatrix[2][2]));

	PSVertex OUT = (PSVertex)0;
	//if (IN.params.z == 1) {
	 	OUT.uv = IN.uv;
		float3 position = IN.worldPos + (IN.pos.x * rightVector * IN.params.x + IN.pos.z * upVector * IN.params.y);
		OUT.pos = mul(float4(position,1.0), g_WorldViewProjectionMatrix);
		OUT.uv1 = IN.uv;
		OUT.uv1.y = OUT.uv1.y + (IN.params.z * scrollSpeedsLava.x);
	//	}

    return OUT;
}

float4 PS(PSVertex IN) : COLOR
{
	
	float4 fireColor = tex2D(S0LinearWrapSampler, IN.uv1.xy);
	return fireColor;
}


technique TextureTechnique
{
	pass p0
	{
		//AlphaBlendEnable = false;
		CullMode = NONE; // NONE - CW
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}