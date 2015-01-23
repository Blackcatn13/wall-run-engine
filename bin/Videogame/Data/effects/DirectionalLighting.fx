#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"



DWORD  BCLR = 0xff202080;  // Background color (if no image)
float3 g_LightAmbient=float3(0.3, 0.35, 0.4);


struct PNormalVertex
{
    float4 pos : POSITION;
    float3 normal : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3 WorldPosition: TEXCOORD2;
};

PNormalVertex RenderNormalsVS(
    VertexVS_TTEXTURE_NORMAL_VERTEX IN,
    uniform float4x4 WorldViewProj,
    uniform float4x4 worldMatrix)
{
    PNormalVertex OUT = (PNormalVertex )0;
    OUT.pos=mul(float4(IN.Position.xyz,1),WorldViewProj);
    OUT.uv=IN.UV;
    OUT.normal=mul(IN.Normal,worldMatrix);
    OUT.WorldPosition=mul(float4(IN.Position.xyz, 1.0), g_WorldMatrix).xyz;
    return OUT;
}

float4 RenderNormalsPS20(PNormalVertex IN) : COLOR
{
    float3 l_Nn=normalize(IN.normal);
    float4 OUT;
    float4 l_DiffuseColor=tex2D(S0LinearWrapSampler , IN.uv);
    float3 l_AmbientContrib=g_LightAmbient*l_DiffuseColor.xyz;
	float3 l_DiffuseContrib;
	for(int i=0;i<MAXLIGHTS;i++)
	{
		if(g_LightEnabled[i]==1)
		{
			if(g_LightType[i]==1)
			{
				l_DiffuseContrib=saturate(dot(l_Nn, -g_LightDirection[i]))*l_DiffuseColor.xyz*g_LightColor[i];
			}
		}
	}
	return float4(g_LightDirection[0],1);
    //float3 l_DiffuseContrib=saturate(dot(l_Nn, -g_LightDir))*l_DiffuseColor.xyz*g_LightColor;
    //float3 l_CameraPostion=g_ViewMatrix[3].xyz;
    //float3 l_Hn=normalize(normalize(l_CameraPostion-IN.WorldPosition)-g_LightDir);
    //float3 l_SpecularContrib=g_LightColor*pow(saturate(dot(l_Nn, l_Hn)), g_SpecularExponent);
    //return float4(l_AmbientContrib+l_DiffuseContrib+l_SpecularContrib, l_DiffuseColor.a);
}

technique tec0
{
    pass p0
    {
        VertexShader =compile vs_3_0 RenderNormalsVS(g_WorldViewProjectionMatrix, g_WorldMatrix );
        PixelShader = compile ps_3_0 RenderNormalsPS20();
    }
}