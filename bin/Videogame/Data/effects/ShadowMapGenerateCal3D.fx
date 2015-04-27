#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

struct PNormalVertex
{
	float4 oPos : POSITION;
	float2 Depth : TEXCOORD0;
};

float3 CalcAnimtedPos(float4 Position, float4 Indices, float4 Weight)
{
	float3 l_Position=0;
	l_Position = mul(g_Bones[Indices.x], Position) * Weight.x;
	l_Position += mul(g_Bones[Indices.y], Position) * Weight.y;
	l_Position += mul(g_Bones[Indices.z], Position) * Weight.z;
	l_Position += mul(g_Bones[Indices.w], Position) * Weight.w;
	//l_Position.x = -l_Position.x;
	l_Position.z = -l_Position.z;
	return l_Position;
}

PNormalVertex VertShadow(CAL3D_HW_VERTEX_VS IN)
{
	PNormalVertex OUT = (PNormalVertex)0;
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	OUT.oPos = mul(float4(l_Position, 1.0), g_WorldViewProjectionMatrix);
	OUT.Depth = OUT.oPos.zw;
	return OUT;
}

float4 PixShadow(PNormalVertex IN) : COLOR
{
	//float4 Color = IN.Depth.x / IN.Depth.y;
	//return float4(0,0,1,1);
	return IN.Depth.x / IN.Depth.y;
}

technique GenerateShadowMapCal3D
{
    pass p0
    {
        VertexShader = compile vs_3_0 VertShadow();
        PixelShader = compile ps_3_0 PixShadow();
    }
}