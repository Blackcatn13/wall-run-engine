#pragma once
#ifndef VERTEX_TYPES_H
#define VERTEX_TYPES_H

#include <d3d9.h>

#define VERTEX_TYPE_GEOMETRY    0x0001      // D3DFVF_XYZ
#define VERTEX_TYPE_NORMAL      0x0002      // D3DFVF_NORMAL       
#define VERTEX_TYPE_TANGENT     0x0004      // D3DFVF_
#define VERTEX_TYPE_BINORMAL    0x0008      // D3DFVF_
#define VERTEX_TYPE_TEXTURE1    0x0010      // D3DFVF_TEX1
#define VERTEX_TYPE_TEXTURE2    0x0020      // D3DFVF_TEX2
#define VERTEX_TYPE_DIFFUSE     0x0040      // D3DFVF_DIFFUSE
#define VERTEX_TYPE_WEIGHTS     0x0080      // CAL3D_WEIGHTS
#define VERTEX_TYPE_INDEXS      0x0100      // CAL3D_INDEXS
#define VERTEX_TYPE_CUBE        0x0200      // CUBE MAP
#define VERTEX_TYPE_RNM         0x0400      // RNM
#define VERTEX_TYPE_SCREEN      0x0800		  // DRAW_QUAD_SCREEN
#define VERTEX_TYPE_POLY		    0x1000		  // POLY
#define VERTEX_TYPE_PARTICLE    0x2000      // PARTICLE AND BILLBOARD Instanced
#define VERTEX_TYPE_BILLBOARD   0x4000      // BILLOBARD

#include "GraphicsManager.h"
#include "Utils\Defines.h"

//ColoredQuad2D
struct SCREEN_TEXTURED_COLORED_VERTEX {
  float x, y, z, w;
  DWORD color;
  float tu, tv;

  static inline unsigned int GetFVF() {
    return D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// Particle
struct TPARTICLE_VERTEX {
  float x, y, z;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_PARTICLE;
  }
  static inline unsigned int GetFVF() {
    return 0;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

struct TPARTICLE_VERTEX_INSTANCE {
  float x, y, z;
  float size;
  float visible;
};

struct TBILLBOARD_INSTANCED_VERTEX {
  float x, y, z;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_PARTICLE;
  }
  static inline unsigned int GetFVF() {
    return 0;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

struct TBILLBOARD_VERTEX_INSTANCE {
  float x, y, z;
  float sizex, sizey, tick;
  //float visible;
};

// Billboard
struct TBILLBOARD_VERTEX {
  float x, y, z;
  float tu, tv;
  float sizex, sizey;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_BILLBOARD;
  }
  static inline unsigned int GetFVF() {
    return 0;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};


// G T1
struct TTEXTURE_VERTEX {
  float x, y, z;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_TEXTURE1;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_TEX1;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G POLY T1
struct TTEXTURE_POLY_VERTEX {
  float x, y, z;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_POLY;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_TEX1;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G N T1
struct TTEXTURE_NORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

struct TTEXTURE_CUBE_NORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_CUBE;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G N POLY T1
struct TTEXTURE_POLY_NORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_POLY;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};


// G D T1
struct TCOLOR_TEXTURE_VERTEX {
  float x, y, z;
  unsigned long color;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE1;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G N D T1
struct TCOLOR_TEXTURE_NORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz;
  unsigned long color;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE1;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G T1 T2
struct TTEXTURE2_VERTEX {
  float x, y, z;
  float tu, tv;
  float tu2, tv2;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_TEX2;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};
// G N T1 T2
struct TTEXTURE2_NORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz;
  float tu, tv;
  float tu2, tv2;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G D T1 T2
struct TCOLOR_TEXTURE2_VERTEX {
  float x, y, z;
  unsigned long color;
  float tu, tv;
  float tu2, tv2;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G N D T1 T2
struct TCOLOR_NORMAL_DIFFUSE_TEXTURE2_VERTEX {
  float x, y, z;
  float nx, ny, nz;
  unsigned long color;
  float tu, tv;
  float tu2, tv2;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G N T B T1
struct TTEXTURE_NORMAL_TANGET_BINORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz, nw;
  float tnx, tny, tnz, tnw;
  float bnx, bny, bnz, bnw;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE1;
  }
  static inline unsigned int GetFVF() {
    return 0;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G N T B T1 T2
struct TTEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz, nw;
  float tnx, tny, tnz, tnw;
  float bnx, bny, bnz, bnw;
  float tu, tv;
  float tu2, tv2;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2;
  }
  static inline unsigned int GetFVF() {
    return 0;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

struct TTEXTURE2_RNORMALM_TANGET_BINORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz, nw;
  float tnx, tny, tnz, tnw;
  float bnx, bny, bnz, bnw;
  float tu, tv;
  float tu2, tv2;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2 | VERTEX_TYPE_RNM;
  }
  static inline unsigned int GetFVF() {
    return 0;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G N T B D T1
struct TCOLORED_TEXTURE_NORMAL_TANGET_BINORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz, nw;
  float tnx, tny, tnz, tnw;
  float bnx, bny, bnz, bnw;
  unsigned long color;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE1;
  }
  static inline unsigned int GetFVF() {
    return 0;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G N T B D T1 T2
struct TCOLORED_TEXTURE2_NORMAL_TANGET_BINORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz, nw;
  float tnx, tny, tnz, tnw;
  float bnx, bny, bnz, bnw;
  unsigned long color;
  float tu, tv;
  float tu2, tv2;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_DIFFUSE | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2;
  }
  static inline unsigned int GetFVF() {
    return 0;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G D
struct TCOLORED_VERTEX {
  float x, y, z;
  unsigned long color;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_DIFFUSE;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_DIFFUSE;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

// G N D
struct TCOLORED_NORMAL_VERTEX {
  float x, y, z;
  float nx, ny, nz;
  unsigned long color;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_DIFFUSE;
  }
  static inline unsigned int GetFVF() {
    return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

//CAL3D
struct CAL3D_HW_VERTEX {
  float x, y, z;
  float weights[4];
  float indices[4];
  float nx, ny, nz, nw;
  float tangentx, tangenty, tangentz, tangentw;
  float binormalx, binormaly, binormalz, binormalw;
  float tu, tv;
  static inline unsigned short GetVertexType() {
    return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_WEIGHTS | VERTEX_TYPE_INDEXS | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE1;
  }
  static inline unsigned int GetFVF() {
    return 0;
  }
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
  static LPDIRECT3DVERTEXDECLARATION9 &GetVertexDeclaration();
  static void ReleaseVertexDeclaration() {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

#endif