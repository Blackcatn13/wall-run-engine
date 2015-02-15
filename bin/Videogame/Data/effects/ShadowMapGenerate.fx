void VertShadow(float4 Pos: POSITION,
	float3 Normal: NORMAL,
	out float4 oPos: POSITION,
	out float2 Depth: TEXCOORD0)
{
	oPos = mul(Pos, g_WorldViewProj);
	Depth.xy = oPpos.zw;
}

void PixShadow(float2 Depth: TEXTCOORD0, out float4 Color: COLOR)
{
	Color = Depth.x / Depth.y;
}

technique tec0
{
    pass p0
    {
        VertexShader =compile vs_3_0 VertShadow();
        PixelShader = compile ps_3_0 PixShadow();
    }
}