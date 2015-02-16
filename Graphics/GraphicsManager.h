#ifndef CGRAPHICSMANAGER_H
#define CGRAPHICSMANAGER_H

#include <d3dx9.h>
#include <Windows.h>
#include "Utils/Types.h"
#include "Math/Color.h"
#include "Camera/Frustum.h"
#include "Math/Vector2.h"
#include "Texture\Texture.h"

class CColor;
class CCamera;
//class CFontManager;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct CUSTOMVERTEX {
    float x, y, z;
    DWORD color;
    static unsigned int getFlags()
    {
        return D3DFVF_CUSTOMVERTEX;
    }
};

struct CUSTOMVERTEX2 {
    D3DXVECTOR3 pos;
    DWORD color;
    static unsigned int getFlags()
    {
        return D3DFVF_CUSTOMVERTEX;
    }
};

typedef enum ETypeAlignment { CENTER, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT };

struct SCREEN_COLOR_VERTEX {
    float x, y, z, w;
    DWORD color;
    float u, v;
    static unsigned int getFlags()
    {
        return (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    }
};


class CGraphicsManager
{
private:
    HWND					m_Handler;
    LPDIRECT3D9				m_pD3D;	 // direct3d interface
    LPDIRECT3DDEVICE9		m_pD3DDevice; // direct3d device
    uint32					m_uWidth;	// width of the client windows
    uint32					m_uHeight; // height of the client windows
    CColor					m_BackbufferColor_debug;		// Clear the backbuffer with this color in debug mode
    CColor					m_BackbufferColor_release;	// Clear the backbuffer with this color in release mode
    LPDIRECT3DSURFACE9		m_pBackBuffer;
    bool					m_bIsOk;  // Initialization boolean control
    bool					m_bPaintSolid;
    Vect3f                  m_posCam;
    float                   m_yaw;
    float                   m_speed;
    float                   m_pitch;
    CFrustum                m_Frustum;
    bool					m_Rendering;

public:
    CGraphicsManager();
    ~CGraphicsManager();
    void Update();
    void Render();
    bool Init(HWND handler, bool fullscreenMode, uint32 widthScreen, uint32 heightScreen);
    void DeInit();
    void GetWindowRect(HWND handler);
    void BeginRendering();
    void SetupMatrices(CCamera* camera);
    void EndRendering();
    //----Transform Functions-----------------------------------------------
    void SetTransform (D3DXMATRIX& matrix);
    void SetTransform (Mat44f& matrix);
    LPDIRECT3DDEVICE9 GetDevice()
    {
        return m_pD3DDevice;
    }
    void DrawLine	(const Vect3f &PosA, const Vect3f &PosB, CColor Color /*= colWHITE*/);
    void DrawAxis	(float Size);
    void DrawCube	(float Size, CColor color = colWHITE);
    void DrawBox	(float SizeX, float SizeY, float SizeZ, CColor Color = colWHITE);
    void DrawGrid	(float Size, CColor Color = colWHITE, int GridX = 10, int32 GridZ = 10);
    void DrawPlane	(float Size, const Vect3f& normal, float distance, CColor Color = colWHITE, int GridX = 10, int32 GridZ = 10, ETypeModePaint mode = PAINT_WIREFRAME);
    void DrawSphere (float Radius, CColor Color = colWHITE, uint32 Aristas = 10,  ETypeModePaint mode = PAINT_WIREFRAME, EtypeSphere typeSphere = COMPLETE);
    void DrawCamera (CCamera* camera);
    void DrawQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color = colBLUE);
    void CalculateAlignment (uint32 w, uint32 h, ETypeAlignment alignment, Vect2i & finalPos);
    void EnableAlphaBlend ();
    void DisbaleAlphaBlend ();
    void EnableZBuffering ();
    void DisbaleZBuffering ();
    bool isSphereVisible(Vect3f pos, float rad);
    void DrawRectangle2D ( const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor, uint32 edge_w, uint32 edge_h, CColor& edgeColor );
    void GetRay (const Vect2i& mousePos, Vect3f& posRay, Vect3f& dirRay);
    void GetWidthAndHeight(uint32 &widthScreen, uint32 &heightScreen);
    void DrawCapsule (	float radius, float h, uint32 Aristas = 10, CColor Color = colWHITE, ETypeModePaint mode = PAINT_WIREFRAME);
    void DrawCylinder (float Top_Radius, float Bottom_Radius, float h, uint32 Aristas, CColor Color = colBLUE, ETypeModePaint mode = PAINT_WIREFRAME, bool drawCover = false);
    void DrawQuad3D	(const Vect3f& pos, const Vect3f& up, const Vect3f& right, float w, float h, CColor color = colBLUE);
    void DrawQuad3D (	const Vect3f& ul, const Vect3f& ur, const Vect3f& dl, const Vect3f& dr, CColor colo = colBLUE);
	void DrawColoredQuad2DTexturedInPixels(RECT Rect, CColor Color, CTexture Texture, float U0,	float V0, float U1, float V1);

//COMMANDS
    void ClearSceneCommand(float color, float depth, float stencil);
    void BeginRenderCommand();
    void EndRenderCommand();
    void PresentSceneCommand();
};


#endif
