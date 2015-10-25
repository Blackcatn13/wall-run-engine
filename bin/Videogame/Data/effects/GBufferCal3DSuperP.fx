#include "VertexTypes.fxh"
#include "Globals.fxh"
#include "Samplers.fxh"

float g_Bump = 3.0;

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

float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

void CalcAnimatedNormalTangent(float3 Normal, float3 Tangent, float4 Indices, float4 Weight, out float3 OutNormal, out float3 OutTangent)
{
	OutNormal = 0;
	OutTangent =0;
	
	float3x3 m;
	
	m[0].xyz = g_Bones[Indices.x][0].xyz;
	m[1].xyz = g_Bones[Indices.x][1].xyz;
	m[2].xyz = g_Bones[Indices.x][2].xyz;
	
	OutNormal += mul(m, Normal.xyz)* Weight.x;
	OutTangent += mul(m, Tangent.xyz)* Weight.x;
	
	m[0].xyz = g_Bones[Indices.y][0].xyz;
	m[1].xyz = g_Bones[Indices.y][1].xyz;
	m[2].xyz = g_Bones[Indices.y][2].xyz;
	
	OutNormal += mul(m, Normal.xyz)* Weight.y;
	OutTangent += mul(m, Tangent.xyz)* Weight.y;
	
	m[0].xyz = g_Bones[Indices.z][0].xyz;
	m[1].xyz = g_Bones[Indices.z][1].xyz;
	m[2].xyz = g_Bones[Indices.z][2].xyz;
	
	OutNormal += mul(m, Normal.xyz)* Weight.z;
	OutTangent += mul(m, Tangent.xyz)* Weight.z;
	
	m[0].xyz = g_Bones[Indices.w][0].xyz;
	m[1].xyz = g_Bones[Indices.w][1].xyz;
	m[2].xyz = g_Bones[Indices.w][2].xyz;
	
	OutNormal += mul(m, Normal.xyz)* Weight.w;
	OutTangent += mul(m, Tangent.xyz)* Weight.w;
	OutNormal.z = -OutNormal.z;
	OutTangent.z = -OutTangent.z;
	OutNormal = normalize(OutNormal);
	OutTangent = normalize(OutTangent);
}

CAL3D_HW_VERTEX_PS RenderCal3DHWVS(CAL3D_HW_VERTEX_VS IN)
{
	CAL3D_HW_VERTEX_PS OUT=(CAL3D_HW_VERTEX_PS)0;
	
	float3 l_Normal= 0;
	float3 l_Tangent=0;

	CalcAnimatedNormalTangent(IN.Normal.xyz, IN.Tangent.xyz, IN.Indices, IN.Weight, l_Normal, l_Tangent);
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	float4 l_WorldPosition=float4(l_Position, 1.0);
	
	OUT.HPosition = mul(l_WorldPosition, g_WorldViewProjectionMatrix );
	OUT.WorldPosition=OUT.HPosition;
	OUT.WorldNormal=normalize(mul(l_Normal,(float3x3)g_WorldMatrix));
	OUT.WorldTangent=normalize(mul(l_Tangent,(float3x3)g_WorldMatrix));
	OUT.WorldBinormal=normalize(mul(cross(l_Normal, l_Tangent),(float3x3)g_WorldMatrix));
	OUT.RealWorldPosition = mul(float4(l_Position, 1.0), g_WorldMatrix).xyz;

	OUT.UV.x = IN.TexCoord.x;
	OUT.UV.y = 1 - IN.TexCoord.y;
	//OUT.UV = IN.TexCoord.xy;
	
	return OUT;
}

TMultiRenderTargetPixel RenderCal3DHWPS(CAL3D_HW_VERTEX_PS IN)
{
	TMultiRenderTargetPixel OUT = (TMultiRenderTargetPixel)0;
	float l_Depth = IN.WorldPosition.z / IN.WorldPosition.w;
	float3 l_WNn = normalize(IN.WorldNormal);
	float3 l_Tan = normalize(IN.WorldTangent);
	float3 l_Binormal = normalize(IN.WorldBinormal);
	float4 l_NormText = tex2D(S1LinearClampSampler, IN.UV);
	float3 l_Bump = g_Bump * (l_NormText.rgb - float3(0.5,0.5,0.5));
	float3 l_Nn = l_WNn + l_Bump.x * l_Tan + l_Bump.y * l_Binormal;
	l_Nn = normalize(l_Nn);
	float3 NnScaled = Normal2Texture(l_Nn);
	float3 l_DiffuseColor = tex2D(S0LinearClampSampler, IN.UV);
	
	// CONTOUR ------------------------------------------------
	float3 l_EyePos = g_InverseViewMatrix[3].xyz;
	float3 l_CameraToPixel=normalize(l_EyePos-IN.RealWorldPosition.xyz);
	float l_normalDiff = saturate(dot(l_Nn,l_CameraToPixel));
	float rim_contrib = 0;
	if(l_normalDiff < g_RimThicknessSP)
	{
		float value = l_normalDiff/g_RimThicknessSP;
		rim_contrib = 1 - value;
		rim_contrib = rim_contrib * g_RimIntensitySP;
	}
	if(l_normalDiff < g_ContourNormalThickness)
	{
		l_DiffuseColor = float3(0.0,0.0,0.0);
	}
	//---------------------------------------------------------
	
	float4 l_ambientColor = float4(l_DiffuseColor*g_LightAmbientIntensity*g_LightAmbient, 1.0);
	

	OUT.RT0 = float4(l_DiffuseColor, 1.0);
	OUT.RT1 = saturate(l_ambientColor + rim_contrib * g_RimColorSP);
	OUT.RT2.xyz = NnScaled;
	OUT.RT2.w = l_NormText.a;
	OUT.RT3 = l_Depth;

	return OUT;
}

technique Cal3DRimTechnique
{
	pass p0
	{
		CullMode = CW;
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		PixelShader = compile ps_3_0 RenderCal3DHWPS();
	}
}