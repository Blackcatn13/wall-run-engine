#pragma once
#ifndef VERTEX_TYPES_H
#define VERTEX_TYPES_H

#include <d3d9.h>

#define VERTEX_TYPE_GEOMETRY 0x0001     // D3DFVF_XYZ
#define VERTEX_TYPE_NORMAL 0x0002       // D3DFVF_NORMAL       
#define VERTEX_TYPE_TANGENT 0x0004      // D3DFVF_
#define VERTEX_TYPE_BINORMAL 0x0008     // D3DFVF_
#define VERTEX_TYPE_TEXTURE1 0x0010     // D3DFVF_TEX1
#define VERTEX_TYPE_TEXTURE2 0x0020     // D3DFVF_TEX2
#define VERTEX_TYPE_DIFFUSE 0x0040      // D3DFVF_DIFFUSE


// G T1
struct TTEXTURE_VERTEX {
    float x, y, z;
    float tu, tv;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_TEXTURE1;
    }
    static inline unsigned int GetFVF()
    {
        return D3DFVF_XYZ | D3DFVF_TEX1;
    }
};

// G N T1
struct TTEXTURE_NORMAL_VERTEX {
    float x, y, z;
    float nx, ny, nz;
    float tu, tv;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1;
    }
    static inline unsigned int GetFVF()
    {
        return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
    }
};

// G D T1
struct TCOLOR_TEXTURE_VERTEX {
    float x, y, z;
    unsigned long color;
    float tu, tv;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE1;
    }
    static inline unsigned int GetFVF()
    {
        return D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
    }
};

// G N D T1
struct TCOLOR_TEXTURE_NORMAL_VERTEX {
    float x, y, z;
    float nx, ny, nz;
    unsigned long color;
    float tu, tv;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE1;
    }
    static inline unsigned int GetFVF()
    {
        return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
    }
};

// G T1 T2
struct TTEXTURE2_VERTEX {
    float x, y, z;
    float tu, tv;
    float tu2, tv2;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_TEXTURE2;
    }
    static inline unsigned int GetFVF()
    {
        return D3DFVF_XYZ | D3DFVF_TEX2;
    }
};

// G N T1 T2
struct TTEXTURE2_NORMAL_VERTEX {
    float x, y, z;
    float nx, ny, nz;
    float tu, tv;
    float tu2, tv2;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE2;
    }
    static inline unsigned int GetFVF()
    {
        return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;
    }
};

// G D T1 T2
struct TCOLOR_TEXTURE2_VERTEX {
    float x, y, z;
    unsigned long color;
    float tu, tv;
    float tu2, tv2;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE2;
    }
    static inline unsigned int GetFVF()
    {
        return D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2;
    }
};

// G N D T1 T2
struct TCOLOR_TEXTURE2_NORMAL_VERTEX {
    float x, y, z;
    float nx, ny, nz;
    unsigned long color;
    float tu, tv;
    float tu2, tv2;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE2;
    }
    static inline unsigned int GetFVF()
    {
        return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2;
    }
};

// G N T B T1
struct TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX {
    float x, y, z;
    float nx, ny, nz;
    float tnx, tny, tnz;
    float bnx, bny, bnz;
    float tu, tv;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE1;
    }
    static inline unsigned int GetFVF()
    {
        return 0;
    }
};

// G N T B T1 T2
struct TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX {
    float x, y, z;
    float nx, ny, nz;
    float tnx, tny, tnz;
    float bnx, bny, bnz;
    float tu, tv;
    float tu2, tv2;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE2;
    }
    static inline unsigned int GetFVF()
    {
        return 0;
    }
};

// G N T B D T1
struct TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX {
    float x, y, z;
    float nx, ny, nz;
    float tnx, tny, tnz;
    float bnx, bny, bnz;
    unsigned long color;
    float tu, tv;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE1;
    }
    static inline unsigned int GetFVF()
    {
        return 0;
    }
};

// G N T B D T1 T2
struct TCOLORED_TEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX {
    float x, y, z;
    float nx, ny, nz;
    float tnx, tny, tnz;
    float bnx, bny, bnz;
    unsigned long color;
    float tu, tv;
    float tu2, tv2;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE2;
    }
    static inline unsigned int GetFVF()
    {
        return 0;
    }
};

// G D
struct TCOLORED_VERTEX {
    float x, y, z;
    unsigned long color;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_DIFFUSE;
    }
    static inline unsigned int GetFVF()
    {
        return D3DFVF_XYZ | D3DFVF_DIFFUSE;
    }
};

// G N D
struct TCOLORED_NORMAL_VERTEX {
    float x, y, z;
    float nx, ny, nz;
    unsigned long color;
    static inline unsigned short GetVertexType()
    {
        return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_DIFFUSE;
    }
    static inline unsigned int GetFVF()
    {
        return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
    }
};

#endif