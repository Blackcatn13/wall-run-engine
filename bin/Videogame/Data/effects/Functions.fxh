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

//Fog
float CalcAttenuation(float Depth, float StartFog, float EndFog)
{			
	return saturate((Depth-EndFog)/(StartFog-EndFog));
	//return Attenuation;
}

float4 CalcLinearFog(float Depth, float StartFog, float EndFog, float4 FogColor)
{
	return float4(FogColor.xyz, FogColor.a*(1.0-CalcAttenuation(Depth, StartFog, EndFog)));
}

float4 CalcExp2Fog(float Depth, float ExpDensityFog, float4 FogColor)
{
	//Versión de directx
	/*float l_ExpDensity=Depth*ExpDensityFog;
	float l_Fog=1.0/exp(l_ExpDensity*l_ExpDensity);
	//return float4(FogColor.xyz,1-l_Fog);
	return float4(FogColor.xyz,FogColor.a*(1.0-l_Fog));*/
	//Versión que mejora el cálculo
	const float LOG2E = 1.442695; // = 1 / log(2)
	float l_Fog = exp2(-ExpDensityFog * ExpDensityFog * Depth * Depth * LOG2E);
	return float4(FogColor.xyz,FogColor.a*(1.0-l_Fog));
}

float4 CalcExpFog(float Depth, float ExpDensityFog, float4 FogColor)
{
	//Versión de directx
	/*float l_Fog=1.0/exp(Depth*ExpDensityFog);
	return float4(FogColor.xyz,FogColor.a*(1.0-l_Fog));*/
	//Versión que mejora el cálculo
	const float LOG2E = 1.442695; // = 1 / log(2)
	float l_Fog = exp2(-ExpDensityFog * Depth * LOG2E);
	return float4(FogColor.xyz,FogColor.a*(1.0-l_Fog));
}

