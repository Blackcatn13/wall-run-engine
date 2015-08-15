#include "Globals.fxh"
#include "Samplers.fxh"
#include "VertexTypes.fxh"


float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV, float4x4 InverseProjectionMatrix)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, InverseProjectionMatrix);
	// Divide by w to get the view-space position
	return l_PositionVS.xyz / l_PositionVS.w;
}


float3 GetPositionFromZDepthView(float ZDepthView, float2 UV, float4x4 InverseViewMatrix, float4x4 InverseProjectionMatrix)
{
	float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV, InverseProjectionMatrix);
	return mul(float4(l_PositionView,1.0), InverseViewMatrix).xyz;
}

float3 Texture2Normal(float3 Color)
{
	return (Color-0.5)*2;
}

float4 DeferredLightPS(in float2 UV:TEXCOORD0) : COLOR 
{
    float3 l_EyePos = g_InverseViewMatrix[3].xyz;//g_WorldViewMatrix[3].xyz;
	//float3 l_LightColor = saturate(dot(l_Nn, -g_LightDirection[1]));
   // float3 l_HV = normalize(normalize(l_EyePos - IN.WorldPosition)-g_LightDirection[1]);
    //float3 l_SpecularComponent = g_LightColor[1]*pow(saturate(dot(l_Nn,l_HV)), g_SpecularExponent);
    //float3 finalColor = l_DiffuseColor.xyz*g_LightAmbient+l_DiffuseColor*l_LightColor+l_SpecularComponent;
	
	float4 l_RT0 = tex2D(S0LinearWrapSampler, UV);
	float3 l_DiffuseColor = l_RT0.xyz;
	float l_SpecularFactor = l_RT0.w;
	float4 l_RT1 = tex2D(S1LinearWrapSampler, UV);
	float3 l_Ambient = l_RT1.xyz;
	float l_SpecularExponent = l_RT1.w*90;
	float3 l_N = tex2D(S2LinearWrapSampler, UV).xyz;
	float3 l_Nn = normalize(Texture2Normal(l_N));
	float l_Depth = tex2D(S3LinearWrapSampler, UV).x;
	//l_SpecularFactor=1.0;
	//l_SpecularExponent=200;
	float SpecPower = tex2D(S2LinearWrapSampler, UV).a;
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, l_Depth, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, g_InverseProjectionMatrix);
	
	//return tex2D(S7LinearClampSampler, UV);
	if(l_PositionVS.w>=1.0)
		clip(-1);
	//return float4(g_LightColor[0]*g_LightIntensity[0], 1.0);
	//return float4(l_Nn.xyz, 1.0);
	
	//TODO : Con todos los parametros recogidos hacer los calculos para 1 luz, el for es a nivel de C, (como si MAXLIGHTS fuera 1, todo igual)
	// Para la pos del pixel hay que usar la l_Depth y usar la funcion que hay en el pdf para sacar la pos de la profundidad. Todo los calculos son
	// los que ya estan hechos, cambiando las variables por las de arriba.
	
	float3 l_PositionFromDepth = GetPositionFromZDepthView(l_Depth, UV, g_InverseViewMatrix, g_InverseProjectionMatrix);

	//return float4(l_DiffuseColor*dot(l_Nn, -g_LightDirection[0]), 0.0);
	//return float4(1.0, 0.0, 0.0, 0.0);
	float4 l_UABColor=float4(0,1,0,1);
	float3 finalColor = 0;//l_DiffuseColor.xyz*l_Ambient;
	float lightAmount = 1.0;
	if (g_useShadowMap) {
		l_PositionVS = l_PositionVS / l_PositionVS.w;
		float3 vPos = mul(float4(l_PositionFromDepth, 1), g_ViewMatrix).xyz;
		float4 LightPosition = mul(l_PositionVS, g_ViewToLightProjectionMatrix);
		float2 ShadowTexC = 0.5 * (LightPosition.xy / LightPosition.w) + float2(0.5, 0.5);
		ShadowTexC.y = 1.0f - ShadowTexC.y;
		lightAmount = (tex2D(S7LinearClampSampler, ShadowTexC) + g_ShadowEpsilon < LightPosition.z/LightPosition.w)? 1.0f: 0.0f;
		/*if (ShadowTexC.x < 0.0 || ShadowTexC.y < 0.0 || ShadowTexC.x > 1.0 || ShadowTexC.y > 1.0)
			lightAmount = 1.0;*/
		//lightAmount = 1.0;

		if (ShadowTexC.x <=0.0 || ShadowTexC.y<=0.0 || ShadowTexC.x >=1.0 || ShadowTexC.y>=1.0)
		{
			l_UABColor=float4(1,0,0,1);
			lightAmount=0.0;
		}
		else
			l_UABColor=tex2D(S7LinearClampSampler, ShadowTexC);
	}


	
	//return float4(lightAmount, lightAmount, lightAmount, 1);
	//return tex2D(S7LinearClampSampler, ShadowTexC);
	if(g_LightEnabled[0]==1)
	{
		if(g_LightType[0]==0) //omni
		{	
			float3 l_DirVectorNoNormal = l_PositionFromDepth - g_LightPosition[0];
			float l_distance = length(l_PositionFromDepth-g_LightPosition[0]);
			
			float l_Intensity = g_LightIntensity[0];
			if (g_DynamicIntensity[0] == 1 ){
				l_Intensity = (g_LightIntensity[0] / g_DistanceFromPlayer[0]) *10;
				if (l_Intensity > g_LightIntensity[0])
					l_Intensity = g_LightIntensity[0];
			}
			float l_Attenuation = 1-saturate((l_distance-g_NearAtten[0])/(g_FarAtten[0]-g_NearAtten[0]));
			float3 l_DirVector = normalize(l_DirVectorNoNormal);
			float3 l_DiffuseContribution = saturate(dot(l_Nn,-l_DirVector));
			float3 l_HV = normalize(normalize(l_EyePos - l_PositionFromDepth)-l_DirVector);
			float3 l_SpecularComponent = pow(saturate(dot(l_Nn,l_HV)), l_SpecularExponent) * SpecPower;
			finalColor = ((l_DiffuseColor*l_Intensity)*(l_DiffuseContribution*g_LightColor[0]*l_Attenuation) +l_SpecularComponent*l_Intensity*l_SpecularFactor*l_Attenuation);
		}
		else if(g_LightType[0]==1) //directional
		{			
			float l_LightContrib = saturate(dot(l_Nn, -g_LightDirection[0]));
			float3 l_HV = normalize(normalize(l_EyePos - l_PositionFromDepth)-g_LightDirection[0]);
			float3 l_SpecularComponent = pow(saturate(dot(l_Nn,l_HV)), l_SpecularExponent) * SpecPower;
		
			finalColor = (l_DiffuseColor*l_LightContrib*g_LightColor[0]*g_LightIntensity[0]+l_SpecularComponent*g_LightIntensity[0]*l_SpecularFactor);
		}
		else if(g_LightType[0]==2) //spot
		{
			
			float l_distance = length(l_PositionFromDepth-g_LightPosition[0]);
			float l_Attenuation = 1-saturate((l_distance-g_NearAtten[0])/(g_FarAtten[0]-g_NearAtten[0]));
			
			float l_Intensity = (g_LightIntensity[0] / g_DistanceFromPlayer[0]) *10;
			if (l_Intensity > g_LightIntensity[0])
				l_Intensity = g_LightIntensity[0];
			
			float l_HalfFallOff = g_FallOff[0]/2.0;
			float l_HalfAngle = g_LightAngle[0]/2.0;
			float l_MaxAngle = l_HalfFallOff + l_HalfAngle;
							
			float l_AngAttenuation = /*1-*/saturate((g_LightDirection[0] - cos(l_HalfAngle))/(cos(l_HalfAngle+g_FallOff[0])-cos(l_HalfAngle)));

			float l_LightContrib = saturate(dot(l_Nn, -g_LightDirection[0]));
			float3 l_HV = normalize(normalize(l_EyePos - l_PositionFromDepth)-g_LightDirection[0]);
			float3 l_SpecularComponent = pow(saturate(dot(l_Nn,l_HV)), l_SpecularExponent) * SpecPower;
			float3 l_DifuseContrib = l_DiffuseColor*l_LightContrib*g_LightColor[0];

			if(dot(l_Nn, -g_LightDirection[0]) >= cos(l_HalfAngle+g_FallOff[0]) ){
				finalColor = (l_DifuseContrib*l_Intensity*l_AngAttenuation*l_Attenuation+l_SpecularComponent*l_Intensity*l_SpecularFactor*l_Attenuation);
			}				
		}
	}
	//return l_UABColor;
    return float4(finalColor * lightAmount, 1);
}


technique RenderLightDeferredShadingTechnique
{
	pass p0
	{
		//AlphaBlendEnable = true;
		//ZEnable = false;
		//ZWriteEnable = false;
		//BlendOp=Add;
		//SrcBlend = one;
		//DestBlend = one;
		VertexShader = null;
		PixelShader = compile ps_3_0 DeferredLightPS();
	}
}