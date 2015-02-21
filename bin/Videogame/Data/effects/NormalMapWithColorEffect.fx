#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

//DWORD  BCLR = 0xff202080;  // Background color (if no image)
float3 g_LightAmbient=float3(0.2, 0.2, 0.2);
float g_SpecularExponent = 100;
float g_Bump = 30.0;

struct PNormalVertex
{
    float4 HPosition	: POSITION;
    float2 UV		: TEXCOORD0;
    float3 WorldNormal	: TEXCOORD1;
    float3 WorldTangent	: TEXCOORD2;
    float3 WorldBinormal : TEXCOORD3;
    float3 WorldPosition : TEXCOORD4;
    float4 Col           : TEXCOORD5;
};

PNormalVertex RenderNormalsVS(
    VertexVS_TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX IN)
{
    PNormalVertex OUT = (PNormalVertex)0;
	OUT.UV = IN.UV;
	OUT.HPosition = mul(float4 (IN.Position.xyz, 1.0),g_WorldViewProjectionMatrix);
	OUT.WorldPosition=mul(float4(IN.Position.xyz, 1.0), g_WorldMatrix);
	OUT.WorldNormal= mul(IN.Normal.xyz, (float3x3)g_WorldMatrix);
	OUT.WorldTangent= mul(IN.Tangent.xyz, (float3x3)g_WorldMatrix);
	OUT.WorldBinormal= mul(IN.Binormal.xyz, (float3x3)g_WorldMatrix);
	OUT.Col = IN.Color;
	return OUT;
 
}

float4 RN20(PNormalVertex IN) : COLOR
{
	float3 l_TangentNormalized = normalize(IN.WorldTangent);
	float3 l_WNn= normalize(IN.WorldNormal);
	float3 l_BinormalNormalized = normalize(IN.WorldBinormal);
	float4 l_DiffuseTex = IN.Col;
	float4 l_NormalTex = tex2D(S0LinearWrapSampler, IN.UV);
	float3 l_Bump = g_Bump * (l_NormalTex.rgb - float3 (0.5,0.5,0.5));
	float3 l_Nn = l_WNn + l_Bump.x*l_TangentNormalized + l_Bump.y*l_BinormalNormalized;
	float3 finalColor = g_LightAmbient*l_DiffuseTex.xyz;
	l_Nn = normalize(l_Nn);
	float3 l_CameraPosition = g_InverseViewMatrix[3].xyz;
	for(int i = 0; i < MAXLIGHTS; i++) {
		if(g_LightEnabled[i] >= 0) {
			if(g_LightType[i] == 0) {
				float l_Distance=length(IN.WorldPosition-g_LightPosition[i]);
				float l_DistanceAttenuation=1.0-saturate((l_Distance-g_NearAtten[i])/(g_FarAtten[i]-g_NearAtten[i]));
				
				float3 l_DirVectorNoNormal = g_LightPosition[i]-IN.WorldPosition;
				float l_distance = distance(g_LightPosition[i],IN.WorldPosition);
				float l_Attenuation = 1-saturate((l_distance-g_NearAtten[i])/(g_FarAtten[i]-g_NearAtten[i]));
				float3 l_DirVector = normalize(l_DirVectorNoNormal);
				float3 l_DiffuseContribution = saturate(dot(l_Nn,l_DirVector));

				float3 l_HV = normalize(normalize(l_CameraPosition - IN.WorldPosition)-l_DirVector);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nn,l_HV)), g_SpecularExponent);
				finalColor = finalColor +((l_DiffuseTex.xyz*g_LightIntensity[i])*((l_DiffuseContribution*g_LightColor[i]*l_Attenuation)+l_SpecularComponent*g_LightColor[i]));
			}
			if(g_LightType[i] == 1) {
				float3 diff = saturate(dot(l_Nn, -g_LightDirection[i])) * l_DiffuseTex.xyz * g_LightColor[i];	
				float3 l_Hn=normalize(normalize(l_CameraPosition-IN.WorldPosition)-g_LightDirection[i]);
				float l_SpecularContribution=pow(saturate(dot(l_Hn,l_Nn)), g_SpecularExponent) * g_LightColor[i];		
				finalColor = finalColor + diff + l_SpecularContribution;
			}
			if(g_LightType[i] == 2) {
				float l_Distance=length(IN.WorldPosition-g_LightPosition[i]);
				float l_DistanceAttenuation=1.0-saturate((l_Distance-g_NearAtten[i])/(g_FarAtten[i]-g_NearAtten[i]));
				
				float3 l_DirVectorNoNormal = g_LightPosition[i]-IN.WorldPosition;
				float l_distance = distance(g_LightPosition[i],IN.WorldPosition);
				float l_Attenuation = 1-saturate((l_distance-g_NearAtten[i])/(g_FarAtten[i]-g_NearAtten[i]));
				float3 l_DirVector = normalize(l_DirVectorNoNormal);
				float3 l_Color = saturate(dot(l_Nn, l_DirVector));
				
				float l_HalfFallOff = g_FallOff[i]/2.0;
				float l_HalfAngle = g_LightAngle[i]/2.0;
				float l_MaxAngle = l_HalfFallOff + l_HalfAngle;
				float3 l_LightToPixelDirection = normalize(l_DirVectorNoNormal);
				float l_DotDirNormal= dot(l_Nn, l_LightToPixelDirection);
				
				float l_AngAttenuation = /*1*/-saturate((l_DotDirNormal - cos(l_HalfAngle))/(cos(l_HalfAngle+g_FallOff[i])-cos(l_HalfAngle)));

				float3 l_HV = normalize(normalize(l_CameraPosition - IN.WorldPosition)-l_DirVector);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nn,l_HV)), g_SpecularExponent);

				if(l_DotDirNormal >= cos(l_HalfAngle+g_FallOff[i]) ){
					finalColor = finalColor+(l_DiffuseTex.xyz*g_LightIntensity[i])*((l_Color*l_Attenuation*l_AngAttenuation*g_LightColor[i])+l_SpecularComponent);
				}
			}
		}
	}
	return float4(finalColor, 1);
}

technique tec0
{
    pass p0
    {
		//CullMode = None;
        VertexShader =compile vs_3_0 RenderNormalsVS();
        PixelShader = compile ps_3_0 RN20();
    }
}