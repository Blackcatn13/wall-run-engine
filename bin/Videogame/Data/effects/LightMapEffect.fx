#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"

//DWORD  BCLR = 0xff202080;  // Background color (if no image)
float3 g_LightAmbient=float3(0.2, 0.2, 0.2);
float g_SpecularExponent = 100;
float g_LightMapContrib = 0.5;

struct PSVertex
{
    float4 HPosition	: POSITION;
    float2 UV		: TEXCOORD0;
    float2 UV2		: TEXCOORD1;
	float3 WorldNormal: TEXCOORD2;
    float3 WorldPosition: TEXCOORD3;
};

PSVertex RenderLightMapVS(
    VertexVS_TTEXTURE2_NORMAL_VERTEX IN)
{
    PSVertex OUT = (PSVertex)0;
	OUT.UV = IN.UV;
	OUT.UV2 = IN.UV2;
	OUT.HPosition = mul(float4 (IN.Position.xyz, 1.0),g_WorldViewProjectionMatrix);
	OUT.WorldPosition=mul(float4(IN.Position.xyz, 1.0), g_WorldMatrix);
	OUT.WorldNormal= normalize(mul(IN.Normal.xyz, (float3x3)g_WorldMatrix));
	return OUT;
}

float4 RN20(PSVertex IN) : COLOR
{
	//return float4(IN.UV2, 0, 1);
	float3 l_Nn = normalize(IN.WorldNormal);
	float3 l_DiffuseTex = tex2D(S0LinearWrapSampler, IN.UV).rgb;
	//return float4(l_DiffuseTex,1);
	float3 l_DiffuseLigthMap = tex2D(S3LinearWrapSampler, IN.UV2).rgb;
	float3 l_intensity = l_DiffuseTex * l_DiffuseLigthMap;
	//return float4(l_DiffuseLigthMap,1);
	float3 finalColor = l_DiffuseTex *  g_LightAmbient + l_intensity;
	return float4(finalColor,1);
	// A PARTIR DE AQUI SOBRA SI NO INFLUYEN LAS LUCES
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
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nn,l_HV)), g_SpecularExponent);
				finalColor = finalColor +((l_DiffuseTex.xyz*g_LightIntensity[i])*(l_DiffuseContribution*g_LightColor[i]*l_Attenuation) +l_SpecularComponent*g_LightIntensity[i]);
			}
			else if(g_LightType[i]==1) //directional
			{
				
				float l_LightColor = saturate(dot(l_Nn, -g_LightDirection[i]));
				float3 l_HV = normalize(normalize(l_CameraPosition - IN.WorldPosition)-g_LightDirection[i]);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nn,l_HV)), g_SpecularExponent);
			
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
							
				float l_AngAttenuation = saturate((g_LightDirection[i] - cos(l_HalfAngle))/(cos(l_HalfAngle+g_FallOff[i])-cos(l_HalfAngle)));

				float l_LightColor = saturate(dot(l_Nn, -g_LightDirection[i]));
				float3 l_HV = normalize(normalize(l_CameraPosition - IN.WorldPosition)-g_LightDirection[i]);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nn,l_HV)), g_SpecularExponent);
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
		//CullMode = None;
        VertexShader =compile vs_3_0 RenderLightMapVS();
        PixelShader = compile ps_3_0 RN20();
    }
}