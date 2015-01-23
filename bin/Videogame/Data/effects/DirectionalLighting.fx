

float3 g_LightDir
<
    string UIDirectional = "Light Direction";
> = {0.577, -0.577, 0.577};

string XFile = "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Samples\\Media\\tiger\\tiger.x";   // Model to load

DWORD  BCLR = 0xff202080;  // Background color (if no image)

float4x4 g_WorldViewProjectionMatrix : WORLDVIEWPROJECTION;
float4x4 g_WorldMatrix : WORLD;
float4x4 g_ViewMatrix : INVERSE_VIEW;
float g_SpecularExponent=20;
float3 g_LightColor=float3(1,0.6,0.7);
float3 g_LightAmbient=float3(0.3, 0.35, 0.4);

sampler DiffuseTextureSampler : register( s0 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


struct VNormalVertex 
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};


struct PNormalVertex
{
    float4 pos : POSITION;
    float3 normal : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3 WorldPosition: TEXCOORD2;
};

PNormalVertex RenderNormalsVS(
    VNormalVertex IN,
    uniform float4x4 WorldViewProj,
    uniform float4x4 worldMatrix)
{
    PNormalVertex OUT = (PNormalVertex )0;
    OUT.pos=mul(float4(IN.pos.xyz,1),WorldViewProj);
    OUT.uv=IN.uv;
    OUT.normal=mul(IN.normal,worldMatrix);
    OUT.WorldPosition=mul(float4(IN.pos.xyz, 1.0), g_WorldMatrix).xyz;


    return OUT;
}

float4 RenderNormalsPS20(PNormalVertex IN) : COLOR
{
    float3 l_Nn=normalize(IN.normal);
    float4 OUT;
    float4 l_DiffuseColor=tex2D(DiffuseTextureSampler , IN.uv);
    float3 l_AmbientContrib=g_LightAmbient*l_DiffuseColor.xyz;
    float3 l_DiffuseContrib=saturate(dot(l_Nn, -g_LightDir))*l_DiffuseColor.xyz*g_LightColor;
    float3 l_CameraPostion=g_ViewMatrix[3].xyz;
    float3 l_Hn=normalize(normalize(l_CameraPostion-IN.WorldPosition)-g_LightDir);
    float3 l_SpecularContrib=g_LightColor*pow(saturate(dot(l_Nn, l_Hn)), g_SpecularExponent);
    return float4(l_AmbientContrib+l_DiffuseContrib+l_SpecularContrib, l_DiffuseColor.a);
}

technique tec0
{
    pass p0
    {
        VertexShader =compile vs_3_0 RenderNormalsVS(g_WorldViewProjectionMatrix, g_WorldMatrix );
        PixelShader = compile ps_3_0 RenderNormalsPS20();
    }
}