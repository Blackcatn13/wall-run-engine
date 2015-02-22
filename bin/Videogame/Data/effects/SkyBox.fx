 
#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"
 
struct SkyBoxVSIn
{
    float3 Pos : POSITION;
};
 
struct SkyBoxVSOUT
{
    float4 Pos : POSITION;
    float3 UV : TEXCOORD0;
};
 
SkyBoxVSOUT VertexShaderFunction(SkyBoxVSIn IN)
{
    SkyBoxVSOUT OUT = (SkyBoxVSOUT )0;
 
    float4 l_WorldPosition = mul(IN.Pos, g_WorldMatrix);
    float4 l_ViewPosition = mul(l_WorldPosition, g_ViewMatrix);
    OUT.Pos = mul(l_ViewPosition, g_ProjectionMatrix);
	float3 l_EyePos= g_InverseViewMatrix[3].xyz;
   // float4 VertexPosition = mul(IN.Pos, g_WorldMatrix);
    OUT.UV = l_WorldPosition - l_EyePos;
 
    return OUT;
}
 
float4 PixelShaderFunction(SkyBoxVSOUT IN) : COLOR
{	
    return texCUBE(S1LinearWrapSampler, normalize(IN.UV));
}
 
technique Skybox
{
    pass p0
    {
		
        VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 PixelShaderFunction();
    }
}