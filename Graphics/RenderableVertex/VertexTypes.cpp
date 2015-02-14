#include "VertexTypes.h"
#include "GraphicsManager.h"
#include "Core\Core.h"

LPDIRECT3DVERTEXDECLARATION9 TTEXTURE_VERTEX::s_VertexDeclaration = NULL;

LPDIRECT3DVERTEXDECLARATION9 & TTEXTURE_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TTEXTURE_NORMAL_VERTEX::s_VertexDeclaration = NULL;

LPDIRECT3DVERTEXDECLARATION9 & TTEXTURE_NORMAL_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_NORMAL, 0
            },
            {
                0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TCOLOR_TEXTURE_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TCOLOR_TEXTURE_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_COLOR, 0
            },
            {
                0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TCOLOR_TEXTURE_NORMAL_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TCOLOR_TEXTURE_NORMAL_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_NORMAL, 0
            },
            {
                0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_COLOR, 0
            },
            {
                0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TTEXTURE2_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TTEXTURE2_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            //TODO: CHECK TEXRECORD 0 1 0 1?
            { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TTEXTURE2_NORMAL_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TTEXTURE2_NORMAL_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            //TODO: CHECK TEXRECORD 0 1 0 1?
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_NORMAL, 0
            },
            {
                0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            {
                0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 1
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TCOLOR_TEXTURE2_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TCOLOR_TEXTURE2_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            //TODO: CHECK TEXRECORD 0 1 0 1?
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_COLOR, 0
            },
            {
                0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            {
                0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 1
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TCOLOR_NORMAL_DIFFUSE_TEXTURE2_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TCOLOR_NORMAL_DIFFUSE_TEXTURE2_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            //TODO: CHECK TEXRECORD 0 1 0 1?
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_NORMAL, 0
            },
            {
                0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_COLOR, 0
            },
            {
                0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            {
                0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 1
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            //TODO: CHECK TEXRECORD 0 1 0 1?
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_NORMAL, 0
            },
            {
                0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TANGENT, 0
            },
            {
                0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_BINORMAL, 0
            },
            {
                0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            //TODO: CHECK TEXRECORD 0 1 0 1?
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_NORMAL, 0
            },
            {
                0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TANGENT, 0
            },
            {
                0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_BINORMAL, 0
            },
            {
                0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            {
                0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 1
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            //TODO: CHECK TEXRECORD 0 1 0 1?
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_NORMAL, 0
            },
            {
                0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TANGENT, 0
            },
            {
                0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_BINORMAL, 0
            },
            {
                0, 48, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_COLOR, 0
            },
            {
                0, 52, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TCOLORED_TEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TCOLORED_TEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            //TODO: CHECK TEXRECORD 0 1 0 1?
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_NORMAL, 0
            },
            {
                0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TANGENT, 0
            },
            {
                0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_BINORMAL, 0
            },
            {
                0, 48, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_COLOR, 0
            },
            {
                0, 52, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            {
                0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 1
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TCOLORED_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TCOLORED_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_COLOR, 0
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 TCOLORED_NORMAL_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & TCOLORED_NORMAL_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_NORMAL, 0
            },
            {
                0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_COLOR, 0
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9 CAL3D_HW_VERTEX::s_VertexDeclaration = NULL;
LPDIRECT3DVERTEXDECLARATION9 & CAL3D_HW_VERTEX::GetVertexDeclaration()
{
    if (s_VertexDeclaration == NULL) {
        D3DVERTEXELEMENT9 l_VertexDeclaration[] = {
            {
                0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_POSITION, 0
            },
            {
                0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_BLENDWEIGHT, 0
            },
            {
                0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_BLENDINDICES, 0
            },
            {
                0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_NORMAL, 0
            },
            {
                0, 60, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TANGENT, 0
            },
            {
                0, 76, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_BINORMAL, 0
            },
            {
                0, 92, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                D3DDECLUSAGE_TEXCOORD, 0
            },
            D3DDECL_END()
        };
        GRAPHM->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
                &s_VertexDeclaration);
    }
    return s_VertexDeclaration;
}