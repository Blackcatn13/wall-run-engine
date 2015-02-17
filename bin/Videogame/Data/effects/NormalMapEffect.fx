#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"



//DWORD  BCLR = 0xff202080;  // Background color (if no image)
float3 g_LightAmbient=float3(0.2, 0.2, 0.2);
float g_SpecularExponent = 20;
float g_Bump = 3.0;

struct PNormalVertex
{
    float4 HPosition		: POSITION;
    float2 UV				: TEXCOORD0;
    float3 WorldNormal		: TEXCOORD1;
    float3 WorldTangent		: TEXCOORD2;
    float3 WorldBinormal 	: TEXCOORD3;
    float3 WorldPosition	: TEXCOORD4;
};

PNormalVertex RenderNormalsVS(
    VertexVS_TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX IN)
{
    PNormalVertex OUT = (PNormalVertex)0;
	OUT.UV = IN.UV;
	OUT.HPosition = mul(float4 (IN.Position.xyz, 1.0),g_WorldViewProjectionMatrix);
	OUT.WorldPosition=mul(float4(IN.Position.xyz, 1.0), g_WorldMatrix);
	OUT.WorldNormal= normalize(mul(IN.Normal.xyz, (float3x3)g_WorldMatrix));
	OUT.WorldTangent= normalize(mul(IN.Tangent.xyz, (float3x3)g_WorldMatrix));
	OUT.WorldBinormal= normalize(mul(IN.Binormal.xyz, (float3x3)g_WorldMatrix));
	//OUT.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)g_WorldMatrix);
	return OUT;
 
}

float4 RN20(PNormalVertex IN) : COLOR
{
	float3 l_TangentNormalized = normalize(IN.WorldTangent);
	float3 l_WNn= normalize(IN.WorldNormal);
	float3 l_BinormalNormalized = normalize(IN.WorldBinormal);
	float4 l_DiffuseTex = tex2D(S0LinearClampSampler, IN.UV);
	float4 l_NormalTex = tex2D(S1LinearClampSampler, IN.UV);
	//return l_NormalTex;
	float3 l_Bump = g_Bump * (l_NormalTex.rgb - float3 (0.5,0.5,0.5));
	float3 l_Nn = l_WNn + l_Bump.x*l_TangentNormalized + l_Bump.y*l_BinormalNormalized;
	float3 finalColor = g_LightAmbient*l_DiffuseTex.xyz;
	l_Nn = normalize(l_Nn);
	//return float4(l_Nn, 1);
	float3 l_CameraPosition = g_InverseViewMatrix[3].xyz;
	for(int i = 0; i < MAXLIGHTS; i++) 
	{
		if(g_LightEnabled[i] >= 0) 
		{
			
			if(g_LightType[i]==0) //omni
			{	
				float3 l_DirVectorNoNormal = g_LightPosition[i]-IN.WorldPosition;
				float l_distance = distance(g_LightPosition[i],IN.WorldPosition);
				float l_Attenuation = 1-saturate((l_distance-g_NearAtten[i])/(g_FarAtten[i]-g_NearAtten[i]));
				float3 l_DirVector = normalize(-l_DirVectorNoNormal);
				float3 l_DiffuseContribution = saturate(dot(l_Nn,-l_DirVector));
				float3 l_HV = normalize(normalize(l_CameraPosition - IN.WorldPosition)-l_DirVector);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nn,l_HV)), g_SpecularExponent)*l_NormalTex.a;
				finalColor = finalColor +((l_DiffuseTex.xyz*g_LightIntensity[i])*(l_DiffuseContribution*g_LightColor[i]*l_Attenuation) +l_SpecularComponent*g_LightIntensity[i]);
			}
			else if(g_LightType[i]==1) //directional
			{
				
				float l_LightColor = saturate(dot(l_Nn, -g_LightDirection[i]));
				float3 l_HV = normalize(normalize(l_CameraPosition - IN.WorldPosition)-g_LightDirection[i]);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nn,l_HV)), g_SpecularExponent)*l_NormalTex.a;
			
				//finalColor = finalColor +(l_DiffuseTex.xyz*g_LightAmbient+l_DiffuseTex.xyz*l_LightColor*g_LightColor[i]*g_LightIntensity[i]+l_SpecularComponent*g_LightIntensity[i]);
				finalColor = finalColor +(l_DiffuseTex.xyz*l_LightColor*g_LightColor[i]*g_LightIntensity[i]+l_SpecularComponent*g_LightIntensity[i]);
			}
			else if(g_LightType[i]==2) //spot
			{
				float l_distance = distance(g_LightPosition[i],IN.WorldPosition);
				float l_Attenuation = 1-saturate((l_distance-g_NearAtten[i])/(g_FarAtten[i]-g_NearAtten[i]));
								
				float l_HalfFallOff = g_FallOff[i]/2.0;
				float l_HalfAngle = g_LightAngle[i]/2.0;
				float l_MaxAngle = l_HalfFallOff + l_HalfAngle;
							
				float l_AngAttenuation = /*1-*/saturate((g_LightDirection[i] - cos(l_HalfAngle))/(cos(l_HalfAngle+g_FallOff[i])-cos(l_HalfAngle)));

				float l_LightColor = saturate(dot(l_Nn, -g_LightDirection[i]));
				float3 l_HV = normalize(normalize(l_CameraPosition - IN.WorldPosition)-g_LightDirection[i]);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nn,l_HV)), g_SpecularExponent)*l_NormalTex.a;
				float3 l_DifuseContrib = l_DiffuseTex.xyz*l_LightColor*g_LightColor[i];

				if(dot(l_Nn, -g_LightDirection[i]) >= cos(l_HalfAngle+g_FallOff[i]) ){
					finalColor = finalColor +(l_DifuseContrib*g_LightIntensity[i]*l_AngAttenuation*l_Attenuation+l_SpecularComponent*g_LightIntensity[i]);
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
        VertexShader =compile vs_3_0 RenderNormalsVS();
        PixelShader = compile ps_3_0 RN20();
    }
}