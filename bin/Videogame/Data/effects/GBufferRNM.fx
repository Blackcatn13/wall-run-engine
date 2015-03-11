#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"


float g_Bump = 3.0;

struct TGBUFFER_TEXTURED2_TANGENT_BINORMAL_VERTEX_PS
{
	float4 HPosition : POSITION;
    float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float3 WorldNormal : TEXCOORD2;
	float3 WorldTangent : TEXCOORD3;
	float3 WorldBinormal : TEXCOORD4;
	float4 WorldPosition : TEXCOORD5;
};

float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

float3 GetRadiosityNormalMap(float3 Nn, float2 UV, float3x3 WorldMatrix)
{
	float3 l_LightmapX=tex2D(S2LinearWrapSampler, UV)*2;
	float3 l_LightmapY=tex2D(S3LinearWrapSampler, UV)*2;
	float3 l_LightmapZ=tex2D(S4LinearWrapSampler, UV)*2;
	float3 l_BumpBasisX=normalize(float3(0.816496580927726, 0.5773502691896258, 0 ));
	float3 l_BumpBasisY=normalize(float3(-0.408248290463863, 0.5773502691896258,0.7071067811865475 ));
	float3 l_BumpBasisZ=normalize(float3(-0.408248290463863, 0.5773502691896258, -0.7071067811865475));
	float3 diffuseLighting=saturate( dot( Nn, l_BumpBasisX ) ) * l_LightmapX +saturate( dot( Nn, l_BumpBasisY ) ) * l_LightmapY +saturate( dot( Nn, l_BumpBasisZ ) ) * l_LightmapZ;
	return diffuseLighting;
}

TGBUFFER_TEXTURED2_TANGENT_BINORMAL_VERTEX_PS GBufferVS(VertexVS_TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX IN){
	
	TGBUFFER_TEXTURED2_TANGENT_BINORMAL_VERTEX_PS OUT = (TGBUFFER_TEXTURED2_TANGENT_BINORMAL_VERTEX_PS)0;
    OUT.UV = IN.UV;
	OUT.UV2 = IN.UV2;
    OUT.HPosition = mul(float4(IN.Position,1.0), g_WorldViewProjectionMatrix);
	OUT.WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	OUT.WorldTangent = normalize(mul(IN.Tangent.xyz, (float3x3) g_WorldMatrix));
	OUT.WorldBinormal = normalize(mul(IN.Binormal.xyz, (float3x3) g_WorldMatrix));
	OUT.WorldPosition = OUT.HPosition;
    return OUT;
}

TMultiRenderTargetPixel GBufferPS(TGBUFFER_TEXTURED2_TANGENT_BINORMAL_VERTEX_PS IN) {
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	float4 l_DiffuseColor = tex2D(S0LinearWrapSampler , IN.UV);
	
	float3 Nn = normalize(IN.WorldNormal);
	float3 l_TangentNormalized = normalize(IN.WorldTangent);
	float3 l_WNn= normalize(IN.WorldNormal);
	float3 l_BinormalNormalized = normalize(IN.WorldBinormal);
	float4 l_NormalTex = tex2D(S1LinearClampSampler, IN.UV);
	float3 l_Bump = g_Bump * (l_NormalTex.rgb - float3 (0.5,0.5,0.5));
	float3 l_Nn = l_WNn + l_Bump.x*l_TangentNormalized + l_Bump.y*l_BinormalNormalized;
	l_Nn = normalize(l_Nn);
	
	float3 l_DiffuseLigthMap = GetRadiosityNormalMap(l_Nn, IN.UV2, g_WorldMatrix);
	float3 l_intensity = l_DiffuseColor.xyz * l_DiffuseLigthMap;

	float3 NnScalated = Normal2Texture(Nn);
	// Cálculo de la z en formato color
	float l_Depth = IN.WorldPosition.z/IN.WorldPosition.w;
	
	OUT.RT0=float4(l_DiffuseColor.xyz, 1.0);
	OUT.RT1=float4(l_intensity*g_LightAmbientIntensity, 1.0);
	OUT.RT2.xyz=NnScalated;
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