#include "GraphicsManager.h"
#include <string>
#include <exception>
#include <assert.h>
#include "InputManager.h"
#include "Core/Core.h"
#include "Math/MathUtils.h"
#include "Camera/Camera.h"
#include "Math/Matrix44.h"
#include <vector>
#include "Utils/Logger.h"
#include "Utils/Exception.h"
#include "Effects\EffectManager.h"
#include "RenderableVertex\VertexTypes.h"
#include "Texture\Texture.h"
#include "Effects\EffectTechnique.h"
#include "Effects\Effect.h"
#include "Core_Utils/MemLeaks.h"

CGraphicsManager::CGraphicsManager() :
  m_speed(0.001f),
  m_posCam(Vect3f(-1.f, 1.f, -1.f)),
  m_yaw(0),
  m_pitch(0),
  m_bPaintSolid(true),
  m_bIsOk(false),
  m_Rendering(false) {
  m_BackbufferColor_debug.Set(0.49804f, 1.f, 0.83137f, 1.f);
  m_BackbufferColor_release.Set(0.f, 0.f, 0.f, 1.f);
}

CGraphicsManager::~CGraphicsManager() {
}

void CGraphicsManager::DeInit() {
}

void CGraphicsManager::Render() {
}

void CGraphicsManager::Update() {
}

bool CGraphicsManager::Init (HWND hWnd, bool fullscreenMode, uint32 widthScreen, uint32 heightScreen) {
  LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: Inicializando la libreria Direct3D");
// Create the D3D object.
  m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
  assert(m_pD3D);
  m_bIsOk = m_pD3D != NULL;
  if (m_bIsOk != NULL) {
    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    if (fullscreenMode) {
      d3dpp.Windowed          = FALSE;
      d3dpp.BackBufferWidth   = widthScreen;
      d3dpp.BackBufferHeight  = heightScreen;
      d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
    } else {
      d3dpp.Windowed          = TRUE;
      d3dpp.BackBufferFormat	= D3DFMT_UNKNOWN;
    }
    d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;
    d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
    d3dpp.EnableAutoDepthStencil	= TRUE;
    d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
    d3dpp.Flags			= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
    // Create the D3DDevice
    HRESULT aux = m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice );
    m_bIsOk = !FAILED(aux );
    if (!m_bIsOk) {
      m_bIsOk = !FAILED(m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice));
      if (m_bIsOk) {
        LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager::D3DCREATE_SOFTWARE_VERTEXPROCESSING");
      }
    } else {
      LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: D3DCREATE_HARDWARE_VERTEXPROCESSING");
    }
    if (m_bIsOk) {
      // Turn off culling, so we see the front and back of the triangle
      m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
      m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
      m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
      m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
      m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
      m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
      m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
      m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
      m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
      m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
      m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
      m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);
      m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
      m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
      // Turn off D3D lighting, since we are providing our own vertex colors
      m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
      if (fullscreenMode) {
        m_uWidth	= widthScreen;
        m_uHeight	= heightScreen;
      } else {
        GetWindowRect(hWnd);
      }
      LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: La resolucion de pantalla es (%dx%d)", m_uWidth, m_uHeight);
    }
    if (!m_bIsOk) {
      std::string msg_error = "Rendermanager::Init-> Error al inicializar Direct3D";
      LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
      //Release();
      throw CException(__FILE__, __LINE__, msg_error);
    }
    return m_bIsOk;
  }
  return m_bIsOk;
}

void CGraphicsManager::GetWindowRect( HWND hwnd ) {
  RECT rec_window;
  GetClientRect(	hwnd, &rec_window);
  m_uWidth = rec_window.right - rec_window.left;
  m_uHeight = rec_window.bottom - rec_window.top;
}

void CGraphicsManager::BeginRendering() {
#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
  uint32 red		= (uint32) (m_BackbufferColor_debug.GetRed() * 255);
  uint32 green	= (uint32) (m_BackbufferColor_debug.GetGreen() * 255);
  uint32 blue		= (uint32) (m_BackbufferColor_debug.GetBlue() * 255);
  m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
#else // Clear the backbuffer to a black color in a Release mode
  uint32 red		= (uint32) (m_BackbufferColor_release.GetRed() * 255);
  uint32 green	= (uint32) (m_BackbufferColor_release.GetGreen() * 255);
  uint32 blue		= (uint32) (m_BackbufferColor_release.GetBlue() * 255);
  m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
#endif
  // Begin the scene
  HRESULT hr = m_pD3DDevice->BeginScene();
  assert( SUCCEEDED( hr ) );
  m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
  m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);
  m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
  m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
  m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
  m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
  if (m_bPaintSolid) {
    m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
  } else {
    m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME  );
  }
}
//
void CGraphicsManager::EndRendering() {
  m_pD3DDevice->EndScene();
  // Present the backbuffer contents to the display
  m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CGraphicsManager::SetupMatrices(CCamera *camera) {
  D3DXMATRIX m_matView;
  D3DXMATRIX m_matProject;
  if (!camera) {
    //Set default view and projection matrix
    //Setup Matrix view
    D3DXVECTOR3 l_Eye(0.0f, 5.0f, -5.0f), l_LookAt(0.0f, 0.0f, 0.0f), l_VUP(0.0f, 1.0f, 0.0f);
    //1 D3DXVECTOR3 l_Eye(-10.0f,0.0f,0.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
    //2 D3DXVECTOR3 l_Eye(0.0f,10.0f,0.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,0.0f,1.0f);
    //3
    /*CInputManager* im = INPUTM;
    D3DXVECTOR3 l_Eye(0.0f,-10.0f,0.0f), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
    m_yaw -= im->GetMouseDelta().x;
    m_pitch -= im->GetMouseDelta().y;
    float yaw_aux = D3DXToRadian(m_yaw);
    float pitch_aux = D3DXToRadian(m_pitch);
    Vect3f dir = Vect3f(mathUtils::Cos(yaw_aux), 0, mathUtils::Sin(yaw_aux));
    Vect3f nor = Vect3f(mathUtils::Cos(yaw_aux + D3DX_PI * 0.5), 0, (mathUtils::Sin(yaw_aux + D3DX_PI * 0.5)));
    nor.Normalize();
    dir.Normalize();
    Vect3f look = Vect3f(mathUtils::Cos(yaw_aux) * mathUtils::Cos(pitch_aux), mathUtils::Sin(pitch_aux), mathUtils::Sin(yaw_aux) * mathUtils::Cos(pitch_aux));
    Vect3f up_aux = Vect3f(-cos(yaw_aux) * sin(pitch_aux), cos(yaw_aux), -sin(yaw_aux) * sin(pitch_aux));
    look.Normalize();
    up_aux.Normalize();
    if(im->IsDown(IDV_KEYBOARD, KEY_UP)) {
        m_posCam = m_posCam + dir * m_speed;
    }
    if(im->IsDown(IDV_KEYBOARD, KEY_DOWN)) {
        m_posCam = m_posCam - dir * m_speed;
    }
    if(im->IsDown(IDV_KEYBOARD, KEY_LEFT)) {
        m_posCam = m_posCam + nor * m_speed;
    }
    if(im->IsDown(IDV_KEYBOARD, KEY_RIGHT)) {
        m_posCam = m_posCam - nor * m_speed;
    }
    l_Eye = D3DXVECTOR3(m_posCam.x, m_posCam.y, m_posCam.z);
    l_LookAt = D3DXVECTOR3(m_posCam.x + look.x, m_posCam.y + look.y, m_posCam.z + look.z);
    l_VUP = D3DXVECTOR3(up_aux.x, up_aux.y, up_aux.z);*/
    D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);
    //Setup Matrix projection
    D3DXMatrixPerspectiveFovLH( &m_matProject, 45.0f * D3DX_PI / 180.0f, (float)(m_uWidth / m_uHeight), 1.0f, 100.0f );
  } else {
    Vect3f eye = camera->GetEye();
    D3DXVECTOR3 l_Eye(eye.x, eye.y, eye.z);
    Vect3f lookat = camera->GetLookAt();
    D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);
    Vect3f vup = camera->GetVecUp();
    D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);
    //Setup Matrix view
    D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);
    //Setup Matrix projection
    D3DXMatrixPerspectiveFovLH(	&m_matProject, camera->GetFov(), camera->GetAspectRatio(),/*(float)(m_uWidth / m_uHeight),*/
                                camera->GetZn(), camera->GetZf());
  }
  EFFECTM->ActivateCamera(m_matView, m_matProject, camera->GetEye());
  m_Frustum.Update( m_matView * m_matProject );
  m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
  m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
}

void CGraphicsManager::UpdateFrustum(const D3DXMATRIX &viewproj) {
  m_Frustum.Update(viewproj);
}

void CGraphicsManager::SetTransform	(D3DXMATRIX &matrix) {
  m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);
  EFFECTM->SetWorldMatrix(matrix);
}

void CGraphicsManager::SetTransform	(Mat44f &m) {
  D3DXMATRIX aux(	m.m00, m.m10, m.m20, m.m30
                  , m.m01, m.m11, m.m21, m.m31
                  , m.m02, m.m12, m.m22, m.m32
                  , m.m03, m.m13, m.m23, m.m33);
  m_pD3DDevice->SetTransform(D3DTS_WORLD, &aux);
  EFFECTM->SetWorldMatrix(aux);
}

void CGraphicsManager::DrawLine(const Vect3f &PosA, const Vect3f &PosB, CColor Color) {
  DWORD color_aux = Color.GetUint32Argb();
  CUSTOMVERTEX v[2] = {
    {PosA.x, PosA.y, PosA.z, color_aux},
    {PosB.x, PosB.y, PosB.z, color_aux},
  };
  m_pD3DDevice->SetTexture(0, NULL);
  m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
  m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, v, sizeof(CUSTOMVERTEX));
}

void CGraphicsManager::DrawAxis(float Size) {
  DrawLine(Vect3f(0, 0, 0), Vect3f(1 * Size, 0, 0), colRED);
  DrawLine(Vect3f(0, 0, 0), Vect3f(0, 1 * Size, 0), colGREEN);
  DrawLine(Vect3f(0, 0, 0), Vect3f(0, 0, 1 * Size), colBLUE);
}

void CGraphicsManager::DrawCube(float Size, CColor color) {
  DrawLine(Vect3f(-Size / 2, -Size / 2, -Size / 2), Vect3f(Size / 2, -Size / 2, -Size / 2), color);
  DrawLine(Vect3f(-Size / 2, -Size / 2, -Size / 2), Vect3f(-Size / 2, -Size / 2, Size / 2), color);
  DrawLine(Vect3f(Size / 2, -Size / 2, -Size / 2), Vect3f(Size / 2, -Size / 2, Size / 2), color);
  DrawLine(Vect3f(-Size / 2, -Size / 2, Size / 2), Vect3f(Size / 2, -Size / 2, Size / 2), color);
  DrawLine(Vect3f(-Size / 2, Size / 2, -Size / 2), Vect3f(Size / 2, Size / 2, -Size / 2), color);
  DrawLine(Vect3f(-Size / 2, Size / 2, -Size / 2), Vect3f(-Size / 2, Size / 2, Size / 2), color);
  DrawLine(Vect3f(Size / 2, Size / 2, -Size / 2), Vect3f(Size / 2, Size / 2, Size / 2), color);
  DrawLine(Vect3f(-Size / 2, Size / 2, Size / 2), Vect3f(Size / 2, Size / 2, Size / 2), color);
  DrawLine(Vect3f(-Size / 2, -Size / 2, -Size / 2), Vect3f(-Size / 2, Size / 2, -Size / 2), color);
  DrawLine(Vect3f(-Size / 2, -Size / 2, Size / 2), Vect3f(-Size / 2, Size / 2, Size / 2), color);
  DrawLine(Vect3f(Size / 2, -Size / 2, -Size / 2), Vect3f(Size / 2, Size / 2, -Size / 2), color);
  DrawLine(Vect3f(Size / 2, -Size / 2, Size / 2), Vect3f(Size / 2, Size / 2, Size / 2), color);
}

void CGraphicsManager::DrawBox(float SizeX, float SizeY, float SizeZ, CColor Color) {
  DrawLine(Vect3f(-SizeX / 2, -SizeY / 2, -SizeZ / 2), Vect3f(SizeX / 2, -SizeY / 2, -SizeZ / 2), Color);
  DrawLine(Vect3f(-SizeX / 2, -SizeY / 2, -SizeZ / 2), Vect3f(-SizeX / 2, -SizeY / 2, SizeZ / 2), Color);
  DrawLine(Vect3f(SizeX / 2, -SizeY / 2, -SizeZ / 2), Vect3f(SizeX / 2, -SizeY / 2, SizeZ / 2), Color);
  DrawLine(Vect3f(-SizeX / 2, -SizeY / 2, SizeZ / 2), Vect3f(SizeX / 2, -SizeY / 2, SizeZ / 2), Color);
  DrawLine(Vect3f(-SizeX / 2, SizeY / 2, -SizeZ / 2), Vect3f(SizeX / 2, SizeY / 2, -SizeZ / 2), Color);
  DrawLine(Vect3f(-SizeX / 2, SizeY / 2, -SizeZ / 2), Vect3f(-SizeX / 2, SizeY / 2, SizeZ / 2), Color);
  DrawLine(Vect3f(SizeX / 2, SizeY / 2, -SizeZ / 2), Vect3f(SizeX / 2, SizeY / 2, SizeZ / 2), Color);
  DrawLine(Vect3f(-SizeX / 2, SizeY / 2, SizeZ / 2), Vect3f(SizeX / 2, SizeY / 2, SizeZ / 2), Color);
  DrawLine(Vect3f(-SizeX / 2, -SizeY / 2, -SizeZ / 2), Vect3f(-SizeX / 2, SizeY / 2, -SizeZ / 2), Color);
  DrawLine(Vect3f(-SizeX / 2, -SizeY / 2, SizeZ / 2), Vect3f(-SizeX / 2, SizeY / 2, SizeZ / 2), Color);
  DrawLine(Vect3f(SizeX / 2, -SizeY / 2, -SizeZ / 2), Vect3f(SizeX / 2, SizeY / 2, -SizeZ / 2), Color);
  DrawLine(Vect3f(SizeX / 2, -SizeY / 2, SizeZ / 2), Vect3f(SizeX / 2, SizeY / 2, SizeZ / 2), Color);
}

void CGraphicsManager::DrawGrid(float Size, CColor Color, int GridX, int32 GridZ) {
  for (int i = 0; i <= GridX; ++i) {
    DrawLine(Vect3f(-Size / 2 + (i * Size) / GridX, 0, -Size / 2), Vect3f(-Size / 2 + (i * Size) / GridX, 0, Size / 2), Color);
  }
  for (int i = 0; i <= GridZ; ++i) {
    DrawLine(Vect3f(-Size / 2, 0, -Size / 2 + (i * Size) / GridZ), Vect3f(Size / 2, 0, -Size / 2 + (i * Size) / GridZ), Color);
  }
}

void CGraphicsManager::DrawPlane (float size, const Vect3f &normal, float distance, CColor Color, int32 GridX, int32 GridZ, ETypeModePaint mode ) {
  D3DXMATRIX matrix;
  D3DXMatrixIdentity(&matrix);
  m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);
  //Ax + By + Cz + D = 0
  //if (x,y) = (0,0) then z = -D/C
  //if (x,y) = (1,1) then z = (-D-A-B)/C
  //With two points we have a vector-->
  float A, B, C, D;
  A = normal.x;
  B = normal.y;
  C = normal.z;
  D = distance;
  Vect3f pointA, pointB;
  if ( C != 0) {
    pointA = Vect3f(0.f, 0.f, -D / C);
    pointB = Vect3f(1.f, 1.f, (D - A - B) / C);
  } else if ( B != 0) {
    pointA = Vect3f(0.f, -D / B, 0.f);
    pointB = Vect3f(1.f, (-D - A - C) / B, 1.f);
  } else if ( A != 0) {
    pointA = Vect3f(-D / A, 0.f, 0.f);
    pointB = Vect3f((-D - B - C) / A, 1.f, 1.f);
  } else {
    //error.
  }
  Vect3f vectorA = pointB - pointA;
  vectorA.Normalize();
  Vect3f vectorB;
  vectorB = vectorA ^ normal; //cross product
  vectorB.Normalize();
  Vect3f initPoint = normal * distance;
  assert(GridX > 0);
  assert(GridZ > 0);
  if (mode != PAINT_WIREFRAME) {
    Vect3f initP = normal * (distance + 0.1f);
    DrawQuad3D(initP, vectorA, vectorB,	size,	size,	Color);
  }
  if (mode != PAINT_SOLID) {
    if (mode == PAINT_BOTH) {
      //si es solid las lineas las pintamos blancas:
      Color = colWHITE;
    }
    //LINEAS EN Z
    Vect3f initPointA = initPoint - vectorB * size * 0.5;
    for (int b = 0; b <= GridX; ++b) {
      DrawLine(	initPointA + vectorA * size * 0.5, initPointA - vectorA * size * 0.5, Color );
      initPointA += vectorB * size / (float)GridX;
    }
    initPointA = initPoint - vectorA * size * 0.5;
    for (int b = 0; b <= GridX; ++b) {
      DrawLine(	initPointA + vectorB * size * 0.5, initPointA - vectorB * size * 0.5, Color);
      initPointA += vectorA * size / (float)GridX;
    }
  }
}


void CGraphicsManager::DrawSphere(float Radius, CColor Color, uint32 Aristas, ETypeModePaint mode, EtypeSphere typeSphere) {
  float l_fAngle	= 180.f;
  float l_fAngle2	= 180.f;
  for (uint32 t = 0; t < Aristas; ++t) {
    float l_fLambda		= ((float)t) / ((float)Aristas);
    float l_fLambda2	= ((float)(t + 1)) / ((float)Aristas);
    switch (typeSphere) {
      case  HALF_TOP:
        l_fAngle	= mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 90.f, l_fLambda));
        l_fAngle2	= mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 90.f, l_fLambda2));
        break;
      case HALF_BOTTOM:
        l_fAngle	= mathUtils::Deg2Rad(mathUtils::Lerp(90.f, 180.f, l_fLambda));
        l_fAngle2	= mathUtils::Deg2Rad(mathUtils::Lerp(90.f, 180.f, l_fLambda2));
        break;
      case COMPLETE:
        l_fAngle	= mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 180.f, l_fLambda));
        l_fAngle2	= mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 180.f, l_fLambda2));
        break;
    }
    std::vector<Vect3f> l_ring;
    float l_RadiusRing = Radius * sin(l_fAngle);
    Vect3f l_PosAux = v3fZERO;
    Vect3f l_PosAux2, l_PosAux3;
    for (uint32 b = 0; b < Aristas; ++b) {
      Vect3f l_PosA(	l_RadiusRing * cos(mathUtils::Deg2Rad((float)(360.0f * (float)b) / ((float)Aristas))),
                      Radius * cos(l_fAngle),
                      l_RadiusRing * sin(mathUtils::Deg2Rad((float)(360.0f * (float)b) / ((float)Aristas))));
      Vect3f l_PosB(	l_RadiusRing * cos(mathUtils::Deg2Rad((float)(360.0f * (float)(b + 1)) / ((float)Aristas))),
                      Radius * cos(l_fAngle),
                      l_RadiusRing * sin(mathUtils::Deg2Rad((float)(360.0f * (float)(b + 1)) / ((float)Aristas))));
      float l_RadiusNextRing = Radius * sin(l_fAngle2);
      Vect3f l_PosC(	l_RadiusRing * cos(mathUtils::Deg2Rad((float)(360.0f * (float)b) / ((float)Aristas))),
                      Radius * cos(l_fAngle),
                      l_RadiusRing * sin(mathUtils::Deg2Rad((float)(360.0f * (float)b) / ((float)Aristas))));
      Vect3f l_PosD(	l_RadiusNextRing * cos(mathUtils::Deg2Rad((float)(360.0f * (float)b) / ((float)Aristas))),
                      Radius * cos(l_fAngle2),
                      l_RadiusNextRing * sin(mathUtils::Deg2Rad((float)(360.0f * (float)b) / ((float)Aristas))));
      /*if (mode != PAINT_WIREFRAME)
      {
      	DrawTriangle3D(l_PosA, l_PosD, l_PosB, Color);
      	if(l_PosAux !=  v3fZERO )
      	{
      		DrawTriangle3D(l_PosA, l_PosAux, l_PosD,Color);
      	}
      	else
      	{
      		l_PosAux2 = l_PosA;
      		l_PosAux3	= l_PosD;
      	}

      	if(b == Aristas-1)
      	{
      		DrawTriangle3D(l_PosAux2, l_PosD, l_PosAux3,Color);
      	}
      	l_PosAux = l_PosD;
      }*/
      if ( mode != PAINT_SOLID) {
        CColor color_aux = Color;
        if (mode == PAINT_BOTH) {
          color_aux = colWHITE;
        }
        DrawLine(l_PosA, l_PosB, color_aux);
        DrawLine(l_PosC, l_PosD, color_aux);
      }
    }
  }
}

void CGraphicsManager::DrawCamera (CCamera *camera) {
  D3DXMATRIX matrix;
  D3DXMATRIX translation;
  if (camera->GetTypeCamera() == CCamera::TC_THPS) {
    Vect3f camera_CenterPos = camera->GetObject3D()->GetPosition();
    D3DXMatrixTranslation( &translation, camera_CenterPos.x , camera_CenterPos.y , camera_CenterPos.z );
    m_pD3DDevice->SetTransform( D3DTS_WORLD, &translation );
    DrawCube(0.5f, colCYAN);
    D3DXMatrixTranslation( &matrix, 0, 0, 0 );
    m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
  }
  //---------PINTAMOS EL FRUSTUM-------------//
  D3DXMATRIX translation2;
  Vect3f eye_aux = camera->GetEye();
  D3DXVECTOR3 eye(eye_aux.x, eye_aux.y, eye_aux.z);
  D3DXMatrixTranslation( &translation, eye.x , eye.y , eye.z );
  D3DXMATRIX rotation;
  D3DXMATRIX rotation2;
  float yaw = camera->GetObject3D()->GetYaw();
  float pitch = camera->GetObject3D()->GetPitch();
  D3DXMatrixRotationY ( &rotation,  -yaw);
  D3DXMatrixRotationZ ( &rotation2, +pitch);
  matrix = rotation2 * rotation * translation; ;
  // Cambiar el sistema de coordenadas
  m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
  CUSTOMVERTEX2 pts[9];
  float fov = camera->GetFov();
  float aspect = camera->GetAspectRatio();
  float d = camera->GetViewD();
  float zNear = camera->GetZn();
  float zFar = camera->GetZf();
  float h_near = zNear * tan ( fov * 0.5f );
  float k_near = h_near * aspect;
  float h_far = d * tan ( fov * 0.5f );
  float k_far = h_far * aspect;
  pts[ 0 ].pos = D3DXVECTOR3( 0, 0, 0 );
  pts[ 0 ].color = 0xffffffff;
  pts[ 1 ].pos = D3DXVECTOR3( d, h_far, k_far );
  pts[ 1 ].color = 0xffffffff;
  pts[ 2 ].pos = D3DXVECTOR3( d, h_far, -k_far );
  pts[ 2 ].color = 0xffffffff;
  pts[ 3 ].pos = D3DXVECTOR3( d, -h_far, -k_far );
  pts[ 3 ].color = 0xffffffff;
  pts[ 4 ].pos = D3DXVECTOR3( d, -h_far, k_far );
  pts[ 4 ].color = 0xffffffff;
  pts[ 5 ].pos = D3DXVECTOR3( zNear, h_near, k_near );
  pts[ 5 ].color = 0xffffffff;
  pts[ 6 ].pos = D3DXVECTOR3( zNear, h_near, -k_near );
  pts[ 6 ].color = 0xffffffff;
  pts[ 7 ].pos = D3DXVECTOR3( zNear, -h_near, -k_near );
  pts[ 7 ].color = 0xffffffff;
  pts[ 8 ].pos = D3DXVECTOR3( zNear, -h_near, k_near );
  pts[ 8 ].color = 0xffffffff;
  // Decimos el tipo de vertice que vamos a proporcionar...
  m_pD3DDevice->SetFVF( CUSTOMVERTEX2::getFlags() );
  // Desactivar la textura
  m_pD3DDevice->SetTexture (0, NULL);
  m_pD3DDevice->DrawPrimitiveUP( D3DPT_POINTLIST, 9, pts, sizeof( CUSTOMVERTEX2 ) );
  static short int indexes[] =  {
    0, 1, 0, 2, 0, 3, 0, 4,
    1, 2, 2, 3, 3, 4, 4, 1,
    5, 6, 6, 7, 7, 8, 8, 5
  };
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 9, 12, indexes, D3DFMT_INDEX16, pts, sizeof( CUSTOMVERTEX2 ) );
  D3DXMatrixTranslation( &matrix, 0, 0, 0 );
  m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
  //---------PINTAMOS LA BOX Y LOS EJES------------------//
  D3DXMatrixTranslation( &translation2, -1.0f, 0.0f, 0.0f );
  matrix = translation2 * rotation2 * rotation * translation;
  m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
  CUSTOMVERTEX v[6] = {
    //EJE X
    {0, 0, 0, 0xffff0000},
    {3, 0, 0, 0xffff0000},
    //EJE Y
    {0, 0, 0, 0xff00ff00},
    {0, 3, 0, 0xff00ff00},
    //EJE Z
    {0, 0, 0, 0xff0000ff},
    {0, 0, 3, 0xff0000ff},
  };
  m_pD3DDevice->SetTexture(0, NULL);
  m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
  m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 3, v, sizeof(CUSTOMVERTEX));
  DrawCube(0.5f);
  D3DXMatrixTranslation( &matrix, 0, 0, 0 );
  m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
}

//La posicion y el (w,h) esta en pixeles
//La posicion y el (w,h) esta en pixeles
void CGraphicsManager::DrawQuad2D (const Vect2i &pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color) {
  Vect2i finalPos = pos;
  CalculateAlignment(w, h, alignment, finalPos);
  // finalPos = [0]
  //
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]
  unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
  SCREEN_COLOR_VERTEX v[4] = {
    { (float)finalPos.x,		(float)finalPos.y,		0, 1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) sup_esq.
    ,	{ (float)finalPos.x,		(float)finalPos.y + h,	0, 1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) inf_esq.
    , { (float)finalPos.x + w,	(float)finalPos.y,		0, 1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) sup_dr.
    ,	{ (float)finalPos.x + w,	(float)finalPos.y + h,	0, 1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) inf_dr.
  };
  m_pD3DDevice->SetFVF( SCREEN_COLOR_VERTEX::getFlags() );
  m_pD3DDevice->SetTexture(0, NULL);
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v, sizeof( SCREEN_COLOR_VERTEX ) );
}

/*void CGraphicsManager::DrawQuad2D (const Vect2i &pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture *texture, ETypeFlip flipe){
	Vect2i finalPos = pos;
  CalculateAlignment(w, h, alignment, finalPos);
  // finalPos = [0]
  //
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]
  D3DCOLOR l_color = D3DCOLOR_COLORVALUE(255,255,255,255);
  unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
  SCREEN_TEXTURED_COLORED_VERTEX v[4] = {
    { (float)finalPos.x,		(float)finalPos.y,		0, 1, l_color, 0, 0} //(x,y) sup_esq.
    ,	{ (float)finalPos.x,		(float)finalPos.y + h,	0, 1, l_color, 1, 0} //(x,y) inf_esq.
    , { (float)finalPos.x + w,	(float)finalPos.y,		0, 1, l_color, 0, 1} //(x,y) sup_dr.
    ,	{ (float)finalPos.x + w,	(float)finalPos.y + h,	0, 1, l_color, 1, 1} //(x,y) inf_dr.
  };
  m_pD3DDevice->SetFVF( SCREEN_COLOR_VERTEX::getFlags() );
  texture->Activate(0);
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v, sizeof( SCREEN_TEXTURED_COLORED_VERTEX ) );
}*/

void CGraphicsManager::DrawQuad2D (const Vect2i &pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture *texture, ETypeFlip flip) {
  Vect2i finalPos = pos;
  CalculateAlignment(w, h, alignment, finalPos);

  // finalPos = [0]
  //
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]

  Vect2f coord_text[4];
  uint32 w_aux, h_aux;
  switch (flip) {
    case NONE_FLIP:
      coord_text[0].x = 0.01f;	coord_text[0].y = 0.01f;
      coord_text[1].x = 0.01f;	coord_text[1].y = 0.99f;
      coord_text[2].x = 0.99f;	coord_text[2].y = 0.01f;
      coord_text[3].x = 0.99f;	coord_text[3].y = 0.99f;
      break;
    case FLIP_X:
      //Pintamos primero el quad de la izquierda:
      coord_text[0].x = 0.99f;	coord_text[0].y = 0.01f;
      coord_text[1].x = 0.99f;	coord_text[1].y = 0.99f;
      coord_text[2].x = 0.01f;	coord_text[2].y = 0.01f;
      coord_text[3].x = 0.01f;	coord_text[3].y = 0.99f;
      /*w_aux = (uint32)(w*0.5);
      DrawQuad2D(finalPos, w_aux, h, UPPER_LEFT, texture, NONE_FLIP);
      finalPos.x += w_aux;
      w = w - w_aux;
      coord_text[0].x = 0.99f;	coord_text[0].y = 0.f;
      coord_text[1].x = 0.99f;	coord_text[1].y = 1.f;
      coord_text[2].x = 0.f;		coord_text[2].y = 0.f;
      coord_text[3].x = 0.f;		coord_text[3].y = 1.f;*/
      break;
    case FLIP_Y:
      coord_text[0].x = 0.01f;	coord_text[0].y = 0.99f;
      coord_text[1].x = 0.01f;	coord_text[1].y = 0.01f;
      coord_text[2].x = 0.99f;	coord_text[2].y = 0.99f;
      coord_text[3].x = 0.99f;	coord_text[3].y = 0.01f;
      /*h_aux = (uint32)(h*0.5);
      DrawQuad2D(finalPos, w, h_aux, UPPER_LEFT, texture, NONE_FLIP);
      finalPos.y += h_aux + 1;
      h = h - h_aux -1;
      coord_text[0].x = 1.f;	coord_text[0].y = 0.f;
      coord_text[1].x = 1.f;	coord_text[1].y = 1.f;
      coord_text[2].x = 0.f;	coord_text[2].y = 0.f;
      coord_text[3].x = 0.f;	coord_text[3].y = 1.f;*/
      break;
    default: {
      LOGGER->AddNewLog(ELL_ERROR, "RenderManager:: Se está intentado renderizar un quad2d con un flip desconocido");
    }

  }


  unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
  SCREEN_TEXTURE_VERTEX v[4] = {
    { (float)finalPos.x,		(float)finalPos.y,		0.f, 1.f, coord_text[0].x,	coord_text[0].y} //(x,y) sup_esq.
    ,	{ (float)finalPos.x,		(float)finalPos.y + h,	0.f, 1.f, coord_text[1].x,	coord_text[1].y} //(x,y) inf_esq.
    , { (float)finalPos.x + w,	(float)finalPos.y,		0.f, 1.f, coord_text[2].x,	coord_text[2].y} //(x,y) sup_dr.
    ,	{ (float)finalPos.x + w,	(float)finalPos.y + h,	0.f, 1.f, coord_text[3].x,	coord_text[3].y} //(x,y) inf_dr.
  };

  m_pD3DDevice->SetFVF( SCREEN_TEXTURE_VERTEX::getFlags() );
  texture->Activate(0);
  //m_pD3DDevice->SetTexture(0, texture);
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v, sizeof( SCREEN_TEXTURE_VERTEX ) );
}


void CGraphicsManager::CalculateAlignment (uint32 w, uint32 h, ETypeAlignment alignment, Vect2i &finalPos) {
  switch (alignment) {
    case CENTER: {
      finalPos.x -= (uint32)(w * 0.5f);
      finalPos.y -= (uint32)(h * 0.5f);
    }
    break;
    case UPPER_LEFT: {
      //Por defecto ya est alienado de esta manera :)
    }
    break;
    case UPPER_RIGHT: {
      finalPos.x -= w;
    }
    break;
    case LOWER_RIGHT: {
      finalPos.x -= w;
      finalPos.y -= h;
    }
    break;
    case LOWER_LEFT: {
      finalPos.y -= h;
    }
    break;
    default: {
      LOGGER->AddNewLog(ELL_ERROR, "RenderManager:: Se está intentado renderizar un quad2d con una alineacion desconocida");
    }
    break;
  }
}

void CGraphicsManager::EnableAlphaBlend (DWORD opblend, DWORD srcblend, DWORD destblend) {
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  // render el quad de difuminacion....
  m_pD3DDevice->SetRenderState ( D3DRS_BLENDOP, opblend );
  m_pD3DDevice->SetRenderState ( D3DRS_SRCBLEND, srcblend  );
  m_pD3DDevice->SetRenderState ( D3DRS_DESTBLEND, destblend );
  //// render el quad de difuminacion....
  m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
  m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
}

void CGraphicsManager::DisableAlphaBlend () {
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}


void CGraphicsManager::EnableZBuffering () {
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
}

void CGraphicsManager::DisableZBuffering () {
  m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

bool CGraphicsManager::isSphereVisible(Vect3f pos, float rad) {
  return m_Frustum.SphereVisible(D3DXVECTOR3(pos.x, pos.y, pos.z), rad);
}

bool CGraphicsManager::isBoxVisible(Vect3f min, Vect3f max) {
  return m_Frustum.BoxVisible(D3DXVECTOR3(max.x, max.y, max.z), D3DXVECTOR3(min.x, min.y, min.z));
}

void CGraphicsManager::DrawRectangle2D (	const Vect2i &pos, uint32 w, uint32 h, CColor &backGroundColor,
    uint32 edge_w, uint32 edge_h, CColor &edgeColor ) {
  //Draw background quad2D:
  DrawQuad2D(pos, w, h, UPPER_LEFT, backGroundColor);
  //Draw the four edges:
  //2 Horizontal:
  Vect2i pos_aux = pos;
  pos_aux.y -= edge_h;
  DrawQuad2D(pos_aux, w, edge_h, UPPER_LEFT, edgeColor);
  pos_aux = pos;
  pos_aux.y += h;
  DrawQuad2D(pos_aux, w, edge_h, UPPER_LEFT, edgeColor);
  //2 Vertical:
  pos_aux = pos;
  pos_aux.x -= edge_w;
  pos_aux.y -= edge_h;
  DrawQuad2D(pos_aux, edge_w, h + (2 * edge_w), UPPER_LEFT, edgeColor);
  pos_aux.x = pos.x + w;
  DrawQuad2D(pos_aux, edge_w, h + (2 * edge_w), UPPER_LEFT, edgeColor);
}

void CGraphicsManager::GetRay (const Vect2i &mousePos, Vect3f &posRay, Vect3f &dirRay) {
  D3DXMATRIX projectionMatrix, viewMatrix, worldViewInverse, worldMatrix;
  m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &projectionMatrix);
  m_pD3DDevice->GetTransform(D3DTS_VIEW, &viewMatrix);
  m_pD3DDevice->GetTransform(D3DTS_WORLD, &worldMatrix);
  float angle_x = (((2.0f * mousePos.x) / m_uWidth) - 1.0f) / projectionMatrix(0, 0);
  float angle_y = (((-2.0f * mousePos.y) / m_uHeight) + 1.0f) / projectionMatrix(1, 1);
  D3DXVECTOR3 ray_org = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
  D3DXVECTOR3 ray_dir = D3DXVECTOR3(angle_x, angle_y, 1.0f);
  D3DXMATRIX m = worldMatrix * viewMatrix;
  D3DXMatrixInverse(&worldViewInverse, 0, &m);
  D3DXVec3TransformCoord(&ray_org, &ray_org, &worldViewInverse);
  D3DXVec3TransformNormal(&ray_dir, &ray_dir, &worldViewInverse);
  D3DXVec3Normalize(&ray_dir, &ray_dir);
  posRay.x = ray_org.x;
  posRay.y = ray_org.y;
  posRay.z = ray_org.z;
  dirRay.x = ray_dir.x;
  dirRay.y = ray_dir.y;
  dirRay.z = ray_dir.z;
}

void CGraphicsManager::GetWidthAndHeight(uint32 &width, uint32 &height) {
  width = m_uWidth;
  height = m_uHeight;
}

void CGraphicsManager::DrawCylinder (float Top_Radius, float Bottom_Radius, float h, uint32 Aristas, CColor Color, ETypeModePaint mode, bool drawCover) {
  CColor color_aux = Color;
  if (mode == PAINT_BOTH) {
    color_aux = colWHITE;
  }
  std::vector< std::vector<Vect3f> > l_AllPoints;
  float currentRadius, currentHeight;
  for (uint32 i = 0; i < Aristas; ++i) {
    std::vector<Vect3f> l_currentCircle;
    currentRadius = mathUtils::Lerp(Top_Radius, Bottom_Radius, (float)i / (float)(Aristas - 1));
    currentHeight = mathUtils::Lerp(h * 0.5f, -h * 0.5f, (float)i / (float)(Aristas - 1));
    for (uint32 t = 0; t < Aristas; ++t) {
      float angle = mathUtils::Deg2Rad(360.0f * ((float)t) / ((float)Aristas));
      Vect3f l_PosA(	currentRadius * cos(angle), currentHeight, currentRadius * sin(angle) );
      if (mode != PAINT_SOLID) {
        angle = mathUtils::Deg2Rad(360.0f * ((float)t + 1) / ((float)Aristas));
        Vect3f l_PosB(	currentRadius * cos(angle), currentHeight,	currentRadius * sin(angle) );
        DrawLine(l_PosA, l_PosB, color_aux);
      }
      l_currentCircle.push_back( l_PosA );
    }//END: for(int32 t= 0; t < Aristas; ++t)
    l_AllPoints.push_back(l_currentCircle);
  }//END:for(int32 i= 0; i < Aristas; ++i)
  for (uint32 cont = 0; cont < Aristas; ++cont) {
    //Laterales:
    std::vector<Vect3f> l_TopRadiusPoints			= l_AllPoints[0];
    std::vector<Vect3f> l_BottomRadiusPoints	= l_AllPoints[Aristas - 1];
    DrawLine(l_TopRadiusPoints[cont], l_BottomRadiusPoints[cont], color_aux);
    if (drawCover) {
      if (mode != PAINT_SOLID) {
        //Tapa de arriba:
        DrawLine(l_TopRadiusPoints[cont], Vect3f(0.f, h * 0.5f, 0.f), color_aux);
        //Tapa de abajo:
        DrawLine(l_BottomRadiusPoints[cont], Vect3f(0.f, -h * 0.5f, 0.f), color_aux);
      }
      /*if (mode != PAINT_WIREFRAME)
      {
      	//Tapa de arriba:
      	DrawTriangle3D(l_TopRadiusPoints[cont],	l_TopRadiusPoints[(cont+1)%Aristas],		Vect3f(0.f,h*0.5f,0.f),			Color);
      	//Tapa de abajo:
      	DrawTriangle3D(Vect3f(0.f,-h*0.5f,0.f),	l_BottomRadiusPoints[(cont+1)%Aristas],	l_BottomRadiusPoints[cont],	Color);
      }*/
    }
  }
  //Paint Solid:
  if (mode != PAINT_WIREFRAME) {
    for (uint32 i = 0; i < Aristas - 1; i++) {
      for (uint32 cont = 0; cont < Aristas; ++cont) {
        Vect3f ul = l_AllPoints[i][cont];
        Vect3f dl = l_AllPoints[i + 1][cont];
        Vect3f ur;
        Vect3f dr;
        if ( cont == Aristas - 1 ) {
          ur = l_AllPoints[i][0];
          dr = l_AllPoints[i + 1][0];
        } else {
          ur = l_AllPoints[i][cont + 1];
          dr = l_AllPoints[i + 1][cont + 1];
        }
        DrawQuad3D(ul, ur, dl, dr, Color);
      }
    }
  }
}

void CGraphicsManager::DrawCapsule(	float radius, float h, uint32 Aristas, CColor Color, ETypeModePaint mode) {
  D3DXMATRIX matrix;
  D3DXMatrixIdentity(&matrix);
  m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);
  DrawCylinder(radius, radius, h, Aristas, Color, mode);
  D3DXMATRIX translation1, translation2;
  D3DXMatrixTranslation(&translation1, 0.f, h * 0.5f, 0.f);
  m_pD3DDevice->SetTransform(D3DTS_WORLD, &translation1);
  DrawSphere(radius, Color, Aristas, mode, HALF_TOP);
  D3DXMatrixTranslation(&translation2, 0.f, -h * 0.5f, 0.f);
  m_pD3DDevice->SetTransform(D3DTS_WORLD, &translation2);
  DrawSphere(radius, Color, Aristas, mode, HALF_BOTTOM);
}

void CGraphicsManager::DrawQuad3D (	const Vect3f &pos, const Vect3f &up, const Vect3f &right, float w, float h, CColor color) {
  Vect3f upper_left, upper_right, down_right, down_left;
  upper_left	= pos + up * h * 0.5f - right * w * 0.5f;
  upper_right	= pos + up * h * 0.5f + right * w * 0.5f;
  down_left		= pos - up * h * 0.5f - right * w * 0.5f;
  down_right	= pos - up * h * 0.5f + right * w * 0.5f;
  DrawQuad3D (	upper_left, upper_right, down_left, down_right, color);
}

void CGraphicsManager::DrawQuad3D (	const Vect3f &ul, const Vect3f &ur, const Vect3f &dl, const Vect3f &dr, CColor color) {
  unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
  CUSTOMVERTEX v[4] = {
    { ul.x,		ul.y,		ul.z,		D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) up_left.
    , { dl.x,		dl.y,		dl.z,		D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) down_left.
    ,	{ ur.x,		ur.y,		ur.z,		D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) up_right.
    ,	{ dr.x,		dr.y,		dr.z,		D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) down_right.
  };
  m_pD3DDevice->SetFVF( CUSTOMVERTEX::getFlags() );
  m_pD3DDevice->SetTexture(0, NULL);
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v, sizeof( CUSTOMVERTEX ) );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COMMANDS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGraphicsManager::ClearSceneCommand(float color, float depth, float stencil) {
  uint32 red = 0;
  uint32 green = 0;
  uint32 blue = 0;
  if (color) {
#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
    red		= (uint32) (m_BackbufferColor_debug.GetRed() * 255);
    green	= (uint32) (m_BackbufferColor_debug.GetGreen() * 255);
    blue	= (uint32) (m_BackbufferColor_debug.GetBlue() * 255);
#else // Clear the backbuffer to a black color in a Release mode
    red		= (uint32) (m_BackbufferColor_release.GetRed() * 255);
    green	= (uint32) (m_BackbufferColor_release.GetGreen() * 255);
    blue	= (uint32) (m_BackbufferColor_release.GetBlue() * 255);
#endif
  }
  DWORD flags = D3DCLEAR_TARGET;
  if (depth)
    flags = flags | D3DCLEAR_ZBUFFER;
  if (stencil)
    flags = flags | D3DCLEAR_STENCIL;
  m_pD3DDevice->Clear( 0, NULL, flags, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
}

void CGraphicsManager::BeginRenderCommand() {
  // Begin the scene
  //if (!m_Rendering) {
  HRESULT hr = m_pD3DDevice->BeginScene();
  m_Rendering = true;
  //assert( SUCCEEDED( hr ) );
  m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
  m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);
  m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
  m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
  m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
  m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
  if (m_bPaintSolid) {
    m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
  } else {
    m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME  );
  }
  // }
}

void CGraphicsManager::EndRenderCommand() {
  m_pD3DDevice->EndScene();
  m_Rendering = false;
}

void CGraphicsManager::PresentSceneCommand() {
  // Present the backbuffer contents to the display
  m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CGraphicsManager::DrawQuad3D (	const Vect3f &ul, const Vect3f &ur, const Vect3f &dl, const Vect3f &dr, CTexture *Texture, CColor Color) {
  unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
  D3DCOLOR l_Color = D3DCOLOR_COLORVALUE(Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha());
  float U0 = 0;
  float U1 = 1;
  float V0 = 0;
  float V1 = 1;
  SCREEN_TEXTURED_COLORED_VERTEX v[4] = {
    { ul.x, ul.y,	ul.z, 1, l_Color, U0, V0} //(x,y) up_left.
    , { ur.x, ur.y,	ur.z, 1, l_Color, U1, V0} //(x,y) up_right.
    , { dl.x, dl.y,	dl.z, 1, l_Color, U0, V1} //(x,y) down_left.
    , { dr.x, dr.y,	dr.z, 1, l_Color, U1, V1} //(x,y) down_right.
  };
  m_pD3DDevice->SetFVF( SCREEN_TEXTURED_COLORED_VERTEX::GetFVF() );
  //m_pD3DDevice->SetTexture(0, NULL);
  Texture->Activate(0);
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, v, sizeof( SCREEN_TEXTURED_COLORED_VERTEX ) );
  m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof( SCREEN_TEXTURED_COLORED_VERTEX ) );

}


void CGraphicsManager::DrawColoredQuad2DTexturedInPixels (RECT Rect, CColor Color, CTexture *Texture, float U0,	float V0, float U1, float V1) {
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]
  D3DCOLOR l_Color = D3DCOLOR_COLORVALUE(Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha());
  SCREEN_TEXTURED_COLORED_VERTEX v[4] = {
    { (float)Rect.left,     (float)Rect.top,    0, 1, l_Color, U0, V0} //(x,y) sup_esq.
    , { (float)Rect.right,    (float)Rect.top,    0, 1, l_Color, U1, V0} //(x,y) sup_dr.
    , { (float)Rect.left,     (float)Rect.bottom,     0, 1, l_Color, U0, V1} //(x,y) inf_esq.
    , { (float)Rect.right,    (float)Rect.bottom,     0, 1, l_Color, U1, V1} //(x,y) inf_dr.
  };
  m_pD3DDevice->SetFVF( SCREEN_TEXTURED_COLORED_VERTEX::GetFVF() );
  Texture->Activate(0);
  m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof( SCREEN_TEXTURED_COLORED_VERTEX ) );
}
void CGraphicsManager::DrawColoredQuad2DTexturedInPixelsByEffectTechnique(CGraphicsManager *RM, CEffectTechnique *EffectTechnique, RECT Rect, CColor Color, CTexture *Texture, float U0, float V0, float U1, float V1) {
  if (EffectTechnique == NULL)
    return;
  EffectTechnique->BeginRender();
  LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetD3DEffect();
  if (l_Effect != NULL) {
    l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());
    UINT l_NumPasses;
    l_Effect->Begin(&l_NumPasses, 0);
    for (UINT iPass = 0; iPass < l_NumPasses; iPass++) {
      l_Effect->BeginPass(iPass);
      RM->DrawColoredQuad2DTexturedInPixels(Rect, Color, Texture, U0, V0, U1, V1);
      l_Effect->EndPass();
    }
    l_Effect->End();
  }
}
