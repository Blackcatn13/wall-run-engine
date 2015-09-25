#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

static const float PI = 3.14159265f;

struct TGBUFFER_TEXTURED1_VERTEX_PS
{
	float4 HPosition : POSITION;
    float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float3 Normal : TEXCOORD2;
	float4 WorldPosition : TEXCOORD3;
	float2 UV1 : TEXCOORD4;
};


float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

float hash (float n) {
	return frac(sin(n)*43758.5453);
}

float noise(float3 x) {
	float3 p = floor(x);
    float3 f = frac(x);

    f       = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0 + 113.0*p.z;

    return lerp(lerp(lerp( hash(n+0.0), hash(n+1.0),f.x),
                   lerp( hash(n+57.0), hash(n+58.0),f.x),f.y),
               lerp(lerp( hash(n+113.0), hash(n+114.0),f.x),
                   lerp( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
}

TGBUFFER_TEXTURED1_VERTEX_PS GBufferVS(VertexVS_TTEXTURE2_NORMAL_VERTEX IN){
	
	TGBUFFER_TEXTURED1_VERTEX_PS OUT = (TGBUFFER_TEXTURED1_VERTEX_PS)0;
	float r = sin((g_Tick / loopDuration) * 2 * PI) * 0.5f + 0.25f;
	float g = sin((g_Tick / loopDuration + 0.33333333f) * 2 * PI) * 0.5f + 0.25f;
	float b = sin((g_Tick / loopDuration + 0.66666667f) * 2 * PI) * 0.5f + 0.25f;
	float correction = 1 / (r + g + b);
	r *= correction;
	g *= correction;
	b *= correction;
    OUT.UV = IN.UV;
    float4 texturePos = float4(IN.UV.xy, 0.0f, 0.0f);
    //float4 dcolor = tex2Dlod(S1LinearWrapSampler, texturePos);
    float valToNoise = clamp(noise(IN.Position), 0.0, 1.0);
    float d = valToNoise * r +  g +  b;
    //float d = noise(dcolor.rgb);
    float3 newPosition = IN.Position + IN.Normal * d * displacement;
    OUT.HPosition = mul(float4(newPosition.xyz,1.0), g_WorldViewProjectionMatrix);
	OUT.Normal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	OUT.WorldPosition = OUT.HPosition;
	OUT.UV1 = IN.UV;
	OUT.UV2 = IN.UV2;
	OUT.UV1.y = OUT.UV1.y - (g_Tick * scrollSpeeds.z);
    return OUT;
}

TMultiRenderTargetPixel GBufferPS(TGBUFFER_TEXTURED1_VERTEX_PS IN) {
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	float4 l_DiffuseColor = tex2D(S0LinearWrapSampler , IN.UV1
		);
	//float4 l_DiffuseColor = tex2Dlod(S1LinearWrapSampler, float4(IN.UV,0,0));
	//float4 l_DiffuseColor = IN.Col;
	clip(l_DiffuseColor.a < 0.1f ? -1 : 1);
	float3 Nn = normalize(IN.Normal);
	float3 NnScalated = Normal2Texture(Nn);
	// Cálculo de la z en formato color
	float l_Depth = IN.WorldPosition.z/IN.WorldPosition.w;
	
	OUT.RT0=float4(l_DiffuseColor.xyz, 1.0);
	OUT.RT1=float4(l_DiffuseColor.xyz*g_LightAmbient*g_LightAmbientIntensity, 1.0);
	OUT.RT2.xyz=NnScalated;
	OUT.RT2.w = 1;
	OUT.RT3=l_Depth;  
	
	return OUT;
}


technique GBufferDiffuseMapTechnique
{
	pass p0
	{
		//AlphaBlendEnable = false;
		//CullMode = NONE; // NONE - CW
		VertexShader = compile vs_3_0 GBufferVS();
		PixelShader = compile ps_3_0 GBufferPS();
	}
}