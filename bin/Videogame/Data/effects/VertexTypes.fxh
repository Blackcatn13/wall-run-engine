// G N T B T1
struct VertexVS_TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX {
    float3 Position	: POSITION;
    float4 Normal	: NORMAL;
    float4 Tangent	: TANGENT0;
    float4 Binormal	: BINORMAL0;
    float2 UV		: TEXCOORD0;
};

// G T1
struct VertexVS_TTEXTURE_VERTEX {
    float3 Position	: POSITION;
    float2 UV		: TEXCOORD0;
};

// G N T1
struct VertexVS_TTEXTURE_NORMAL_VERTEX {
    float3 Position	: POSITION;
    float3 Normal	: NORMAL;
    float2 UV		: TEXCOORD0;
};

// G D T1
struct VertexVS_TCOLOR_TEXTURE_VERTEX {
    float3 Position	: POSITION;
	float4 Color	: COLOR;
	float2 UV		: TEXCOORD0;
};

// G N D T1
struct VertexVS_TCOLOR_TEXTURE_NORMAL_VERTEX {
    float3 Position	: POSITION;
    float3 Normal	: NORMAL;
	float4 Color	: COLOR;
	float2 UV		: TEXCOORD0;
	
};

// G T1 T2
struct VertexVS_TTEXTURE2_VERTEX {
    float3 Position	: POSITION;
    float2 UV		: TEXCOORD0;
	float2 UV2		: TEXCOORD1;
};

// G N T1 T2
struct VertexVS_TTEXTURE2_NORMAL_VERTEX {
    float3 Position	: POSITION;
	float3 Normal	: NORMAL;
    float2 UV		: TEXCOORD0;
	float2 UV2		: TEXCOORD1;
};

// G N D T1 T2
struct VertexVS_TTEXTURE2_NORMAL_DIFFUSE_VERTEX {
    float3 Position	: POSITION;
	float3 Normal	: NORMAL;
	float4 Color	: COLOR;
    float2 UV		: TEXCOORD0;
	float2 UV2		: TEXCOORD1;
};

// G N T B T1 T2
struct VertexVS_TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX{
    float3 Position	: POSITION;
    float4 Normal	: NORMAL;
	float4 Tangent	: TANGENT0;
    float4 Binormal	: BINORMAL0;
    float2 UV		: TEXCOORD0;
	float2 UV2		: TEXCOORD1;
};

// G N T B D T1
struct VertexVS_TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX{
    float3 Position	: POSITION;
	float4 Normal	: NORMAL;
	float4 Tangent	: TANGENT0;
    float4 Binormal	: BINORMAL0;
	float4 Color	: COLOR;
	float2 UV		: TEXCOORD0;
};

// G N T B D T1 T2
struct VertexVS_TCOLORED_TEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX{
    float3 Position	: POSITION;
	float4 Normal	: NORMAL;
	float4 Tangent	: TANGENT0;
    float4 Binormal	: BINORMAL0;
	float4 Color	: COLOR;
	float2 UV		: TEXCOORD0;
	float2 UV2		: TEXCOORD1;
};

// G D
struct VertexVS_TCOLORED_VERTEX{
    float3 Position	: POSITION;
	float4 Color	: COLOR;
};

// G N D
struct VertexVS_TCOLORED_NORMAL_VERTEX{
    float3 Position	: POSITION;
	float3 Normal	: NORMAL;
	float4 Color	: COLOR;
};

struct CAL3D_HW_VERTEX_VS {
	float3 Position : POSITION;
	float4 Weight : BLENDWEIGHT;
	float4 Indices : BLENDINDICES;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT0;
	float4 BiNormal : BINORMAL0;
	float2 TexCoord : TEXCOORD0;
};

struct CAL3D_HW_VERTEX_PS
{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float3 WorldPosition : TEXCOORD2;
	// float3 WorldTangent : TEXCOORD3;
	// float3 WorldBinormal : TEXCOORD4;
};