#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

struct PSVertex
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float2 uv2 : TEXCOORD1;
    float2 uv3 : TEXCOORD2;
    float2 uv4 : TEXCOORD3;
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
		OUT.uv2 = IN.uv * scales.x;
		OUT.uv2.y = OUT.uv2.y + (IN.params.z * scrollSpeeds.x);
		OUT.uv3 = IN.uv * scales.y;
		OUT.uv3.y = OUT.uv3.y + (IN.params.z * scrollSpeeds.y);
		OUT.uv4 = IN.uv * scales.z;
		OUT.uv4.y = OUT.uv4.y + (IN.params.z * scrollSpeeds.z);
	//}

    return OUT;
}

float4 PS(PSVertex IN) : COLOR
{
	float4 noise1 = tex2D(S1LinearWrapSampler, IN.uv2);
	float4 noise2 = tex2D(S1LinearWrapSampler, IN.uv3);
	float4 noise3 = tex2D(S1LinearWrapSampler, IN.uv4);

	noise1 = (noise1 - 0.5f) * 2.f;
	noise2 = (noise2 - 0.5f) * 2.f;
	noise3 = (noise3 - 0.5f) * 2.f;

	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	float4 finalNoise = noise1 + noise2 + noise3;

	float perturb = ((1.0f - IN.uv.y) * distortionScale) + distortionBias;

	float2 noiseCoords = (finalNoise.xy * perturb) + IN.uv.xy;

	float4 fireColor = tex2D(S0LinearClampSampler, noiseCoords.xy);
	float4 alphaColor = tex2D(S2LinearClampSampler, noiseCoords.xy);
	fireColor.a = alphaColor;
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