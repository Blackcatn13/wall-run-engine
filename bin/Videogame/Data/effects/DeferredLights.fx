#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"



struct TGBUFFER_TEXTURED1_VERTEX_PS
{
	float4 HPosition : POSITION;
    float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float4 WorldPosition : TEXCOORD2;
};

// Estructura de píxel que contiene la info de 4 píxeles de salida
struct TMultiRenderTargetPixel
{
	float4 RT0 : COLOR0; //Albedo (float3) + (float) SpecularFactor
	float4 RT1 : COLOR1; //AmbientLight (float3) + (float) SpecularPow
	float4 RT2 : COLOR2; //Normal (float3) + (float) Not used
	float4 RT3 : COLOR3; //Depth (float4)
};

float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

TGBUFFER_TEXTURED1_VERTEX_PS GBufferVS(VertexVS_TTEXTURE_NORMAL_VERTEX IN){
	
	TGBUFFER_TEXTURED1_VERTEX_PS OUT = (TGBUFFER_TEXTURED1_VERTEX_PS)0;
    OUT.UV = IN.UV;
    OUT.HPosition = mul(float4(IN.Position,1.0), g_WorldViewProjectionMatrix);
	OUT.Normal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	OUT.WorldPosition = OUT.HPosition;
    return OUT;
}

float3 Texture2Normal(float3 Color)
{
	return (Color-0.5)*2;
}

float4 DeferredLightPS(in float2 UV:TEXCOORD0) : COLOR 
{
    float3 l_EyePos = g_InverseViewMatrix[3].xyz;//g_WorldViewMatrix[3].xyz;
	//float3 l_LightColor = saturate(dot(l_Nm, -g_LightDirection[1]));
   // float3 l_HV = normalize(normalize(l_EyePos - IN.WorldPosition)-g_LightDirection[1]);
    //float3 l_SpecularComponent = g_LightColor[1]*pow(saturate(dot(l_Nm,l_HV)), g_SpecularExponent);
    //float3 finalColor = l_DiffuseColor.xyz*g_LightAmbient+l_DiffuseColor*l_LightColor+l_SpecularComponent;
	
	float4 l_RT0 = tex2D(S0LinearWrapSampler, UV);
	float3 l_DiffuseColor = l_RT0.xyz;
	float l_SpecularFactor = l_RT0.w;
	float4 l_RT1 = tex2D(S1LinearWrapSampler, UV);
	float3 l_Ambient = l_RT1.xyz;
	float l_SpecularExponent = l_RT1.w;
	float3 l_N = tex2D(S2LinearWrapSampler, UV).xyz;
	float3 l_Nn = Texture2Normal(l_N);
	float4 l_Depth = tex2D(S3LinearWrapSampler, UV);
	//TODO : Con todos los parametros recogidos hacer los calculos para 1 luz, el for es a nivel de C, (como si MAXLIGHTS fuera 1, todo igual)
	// Para la pos del pixel hay que usar la l_Depth y usar la funcion que hay en el pdf para sacar la pos de la profundidad. Todo los calculos son
	// los que ya estan hechos, cambiando las variables por las de arriba.
	float3 finalColor = l_DiffuseColor.xyz*g_LightAmbient;
	for(int i=0;i<MAXLIGHTS;i++)
	{
		if(g_LightEnabled[i]==1)
		{
			if(g_LightType[i]==0) //omni
			{	
				float3 l_DirVectorNoNormal = g_LightPosition[i]-IN.WorldPosition;
				float l_distance = distance(g_LightPosition[i],IN.WorldPosition);
				float l_Attenuation = 1-saturate((l_distance-g_NearAtten[i])/(g_FarAtten[i]-g_NearAtten[i]));
				float3 l_DirVector = normalize(-l_DirVectorNoNormal);
				float3 l_DiffuseContribution = saturate(dot(l_Nm,-l_DirVector));

				float3 l_HV = normalize(normalize(l_EyePos - IN.WorldPosition)-l_DirVector);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nm,l_HV)), g_SpecularExponent);
				finalColor = finalColor +((l_DiffuseColor.xyz*g_LightIntensity[i])*(l_DiffuseContribution*g_LightColor[i]*l_Attenuation) +l_SpecularComponent*g_LightIntensity[i]);
			}
			else if(g_LightType[i]==1) //directional
			{
				
				
				float l_LightColor = saturate(dot(l_Nm, g_LightDirection[i]));
				float3 l_HV = normalize(normalize(l_EyePos - IN.WorldPosition)-g_LightDirection[i]);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nm,l_HV)), g_SpecularExponent);
			
				//finalColor = finalColor +(l_DiffuseColor.xyz*g_LightAmbient+l_DiffuseColor.xyz*l_LightColor*g_LightColor[i]*g_LightIntensity[i]+l_SpecularComponent*g_LightIntensity[i]);
				finalColor = finalColor +(l_DiffuseColor.xyz*l_LightColor*g_LightColor[i]*g_LightIntensity[i]+l_SpecularComponent*g_LightIntensity[i]);
			}
			else if(g_LightType[i]==2) //spot
			{
				
				float l_distance = distance(g_LightPosition[i],IN.WorldPosition);
				float l_Attenuation = 1-saturate((l_distance-g_NearAtten[i])/(g_FarAtten[i]-g_NearAtten[i]));
								
				float l_HalfFallOff = g_FallOff[i]/2.0;
				float l_HalfAngle = g_LightAngle[i]/2.0;
				float l_MaxAngle = l_HalfFallOff + l_HalfAngle;
							
				float l_AngAttenuation = /*1-*/saturate((g_LightDirection[i] - cos(l_HalfAngle))/(cos(l_HalfAngle+g_FallOff[i])-cos(l_HalfAngle)));

				float l_LightColor = saturate(dot(l_Nm, -g_LightDirection[i]));
				float3 l_HV = normalize(normalize(l_EyePos - IN.WorldPosition)-g_LightDirection[i]);
				float3 l_SpecularComponent = g_LightColor[i]*pow(saturate(dot(l_Nm,l_HV)), g_SpecularExponent);
				float3 l_DifuseContrib = l_DiffuseColor.xyz*l_LightColor*g_LightColor[i];

				if(dot(l_Nm, -g_LightDirection[i]) >= cos(l_HalfAngle+g_FallOff[i]) ){
					finalColor = finalColor +(l_DifuseContrib*g_LightIntensity[i]*l_AngAttenuation*l_Attenuation+l_SpecularComponent*g_LightIntensity[i]);
				}
				
				
			}
		}
	}
    return float4(finalColor, 1);
}


technique RenderLightDeferredShadingTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		//CullMode = NONE; // NONE - CW
		VertexShader = compile vs_3_0 GBufferVS();
		PixelShader = compile ps_3_0 DeferredLightPS();
	}
}