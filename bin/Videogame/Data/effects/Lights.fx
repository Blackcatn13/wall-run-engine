#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"



//DWORD  BCLR = 0xff202080;  // Background color (if no image)
float3 g_LightAmbient=float3(0.3, 0.35, 0.4);
float g_SpecularExponent = 200;

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
	float3 l_Nm = normalize(IN.normal);
    float4 l_DiffuseColor = tex2D(S0LinearWrapSampler, IN.uv);
    
    float3 l_EyePos = g_InverseViewMatrix[3].xyz;//g_WorldViewMatrix[3].xyz;
	//float3 l_LightColor = saturate(dot(l_Nm, -g_LightDirection[1]));
   // float3 l_HV = normalize(normalize(l_EyePos - IN.WorldPosition)-g_LightDirection[1]);
    //float3 l_SpecularComponent = g_LightColor[1]*pow(saturate(dot(l_Nm,l_HV)), g_SpecularExponent);
    //float3 finalColor = l_DiffuseColor.xyz*g_LightAmbient+l_DiffuseColor*l_LightColor+l_SpecularComponent;

	float3 finalColor = l_DiffuseColor.xyz*g_LightAmbient;
	for(int i=0;i<MAXLIGHTS;i++)
	{
		if(g_LightEnabled[i]==1)
		{
			if(g_LightType[i]==0) //omni
			{
				
			//	float l_Distance=length(IN.WorldPosition-g_LightPosition[i]);		
				float3 l_DirVectorNoNormal = g_LightPosition[i]-IN.WorldPosition;
				float l_distance = distance(g_LightPosition[i],IN.WorldPosition);
				float l_Attenuation = 1-saturate((l_distance-g_NearAtten[i])/(g_FarAtten[i]-g_NearAtten[i]));
				float3 l_DirVector = normalize(l_DirVectorNoNormal);
				float3 l_DiffuseContribution = saturate(dot(l_Nm,l_DirVector));

				float3 l_HV = normalize(normalize(l_EyePos - IN.WorldPosition)-l_DirVector);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nm,l_HV)), g_SpecularExponent);
				finalColor = finalColor +(l_DiffuseColor.xyz*g_LightIntensity[i]*l_DiffuseContribution*g_LightColor[i]*l_Attenuation)+l_SpecularComponent;
			
			}
			else if(g_LightType[i]==1) //directional
			{
				
				float l_LightColor = saturate(dot(l_Nm, -g_LightDirection[i]));
				float3 l_HV = normalize(normalize(l_EyePos - IN.WorldPosition)-g_LightDirection[i]);
				float l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nm,l_HV)), g_SpecularExponent);
				//finalColor = finalColor +(l_DiffuseColor.xyz*g_LightAmbient+l_DiffuseColor.xyz*l_LightColor*g_LightColor[i]*g_LightIntensity[i]+l_SpecularComponent*g_LightIntensity[i]);
				finalColor = finalColor +(l_DiffuseColor.xyz*l_LightColor*g_LightColor[i]*g_LightIntensity[i]+l_SpecularComponent*g_LightIntensity[i]);
			}
			else if(g_LightType[i]==2) //spot
			{
			
				
				
			}
		}
	}

    return float4(finalColor, 1);
}

technique tec0
{
    pass p0
    {
        VertexShader =compile vs_3_0 RenderNormalsVS(g_WorldViewProjectionMatrix, g_WorldMatrix );
        PixelShader = compile ps_3_0 RenderNormalsPS20();
    }
}