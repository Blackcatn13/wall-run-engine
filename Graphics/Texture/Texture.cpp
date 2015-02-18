#include "Texture.h"
#include "Utils/Defines.h"
#include "GraphicsManager.h"
#include "Core/Core.h"
#include "Utils\Logger.h"

CTexture::CTexture()
	: m_DepthStencilRenderTargetTexture(NULL)
	, m_OldRenderTarget(NULL)
	, m_RenderTargetTexture(NULL)
	, m_OldDepthStencilRenderTarget(NULL)
{
}

bool CTexture::Load(const std::string &FileName)
{
    m_FileName = FileName;
    return LoadFile();
}

bool CTexture::Reload()
{
    Unload();
    return LoadFile();
}

void CTexture::Activate(size_t StageId)
{
    GRAPHM->GetDevice()->SetTexture(StageId, m_Texture);
}

bool CTexture::LoadFile()
{
    HRESULT l_HR = D3DXCreateTextureFromFile(GRAPHM->GetDevice(), m_FileName.c_str(), (LPDIRECT3DTEXTURE9*)&m_Texture);
    return m_Texture != NULL;
    return S_OK(l_HR);
}

void CTexture::Unload()
{
    CHECKED_RELEASE(m_Texture);
}

///////////////////////////////////
void CTexture::Deactivate(size_t Stage)
{
    //PSRender.GetDevice()->SetTexture((DWORD)Stage,NULL);
    GRAPHM->GetDevice()->SetTexture((DWORD)Stage, NULL);
}

bool CTexture::SetAsRenderTarget(size_t IdStage)
{
    LPDIRECT3DDEVICE9 l_Device = GRAPHM ->GetDevice();
    l_Device->GetRenderTarget((DWORD)IdStage, &m_OldRenderTarget);
    if (FAILED( ((LPDIRECT3DTEXTURE9)m_Texture)->GetSurfaceLevel( 0, &m_RenderTargetTexture )
              ) )
        return false;
    l_Device->SetRenderTarget( (DWORD)IdStage, m_RenderTargetTexture );
    CHECKED_RELEASE(m_RenderTargetTexture);
    if (FAILED( l_Device->GetDepthStencilSurface(
                    &m_OldDepthStencilRenderTarget ) ) )
        return false;
    l_Device->SetDepthStencilSurface(
        m_DepthStencilRenderTargetTexture );
    return true;
}

void CTexture::UnsetAsRenderTarget(size_t IdStage)
{
    LPDIRECT3DDEVICE9 l_Device = GRAPHM->GetDevice();
    l_Device->SetDepthStencilSurface(m_OldDepthStencilRenderTarget);
    CHECKED_RELEASE(m_OldDepthStencilRenderTarget);
    l_Device->SetRenderTarget(IdStage, m_OldRenderTarget);
    CHECKED_RELEASE(m_OldRenderTarget);
}

void CTexture::CaptureFrameBuffer(size_t IdStage)
{
    LPDIRECT3DDEVICE9 l_Device = GRAPHM->GetDevice();
    LPDIRECT3DSURFACE9 l_RenderTarget, l_Surface;
    ((LPDIRECT3DTEXTURE9)m_Texture)->GetSurfaceLevel(0, &l_Surface);
    l_Device->GetRenderTarget(IdStage, &l_RenderTarget);
    l_Device->StretchRect(l_RenderTarget, NULL, l_Surface, NULL, D3DTEXF_NONE);
    l_RenderTarget->Release();
}

CTexture::TFormatType CTexture::GetFormatTypeFromString(const std::string &FormatType)
{
    if (FormatType == "R32F")
        return CTexture::R32F;
    else if (FormatType == "A8R8G8B8")
        return CTexture::A8R8G8B8;
    else if (FormatType == "R8G8B8")
        return CTexture::R8G8B8;
    else if (FormatType == "X8R8G8B8")
        return CTexture::X8R8G8B8;
    else {
        std::string l_ErrorMsg = "Format Type" + FormatType + "not recognized";
        LOGGER->AddNewLog(ELL_ERROR,  l_ErrorMsg.c_str());
        //Info("Format Type '%s' not recognized", FormatType.c_str());
    }
    return CTexture::A8R8G8B8;
}


bool CTexture::Create(const std::string &Name, unsigned int Width, unsigned int
                      Height, unsigned int MipMaps, TUsageType UsageType, TPoolType PoolType,
                      TFormatType FormatType)
{
    setName(Name);
    D3DPOOL l_Pool = D3DPOOL_DEFAULT;
    DWORD l_UsageType = D3DUSAGE_DYNAMIC;
    D3DFORMAT l_Format = D3DFMT_A8R8G8B8;
    bool l_CreateDepthStencilSurface = false;
    switch (UsageType) {
    case DYNAMIC:
        l_UsageType = D3DUSAGE_DYNAMIC;
        break;
    case RENDERTARGET:
        l_CreateDepthStencilSurface = true;
        l_UsageType = D3DUSAGE_RENDERTARGET;
        break;
    }
    switch (PoolType) {
    case DEFAULT:
        l_Pool = D3DPOOL_DEFAULT;
        break;
    case SYSTEMMEM:
        l_Pool = D3DPOOL_SYSTEMMEM;
        break;
    }
    switch (FormatType) {
    case A8R8G8B8:
        l_Format = D3DFMT_A8R8G8B8;
        break;
    case R8G8B8:
        l_Format = D3DFMT_R8G8B8;
        break;
    case X8R8G8B8:
        l_Format = D3DFMT_X8R8G8B8;
        break;
    case R32F:
        l_Format = D3DFMT_R32F;
        break;
    }
    HRESULT hr = GRAPHM->GetDevice()->CreateTexture(
                     Width, Height, MipMaps, l_UsageType, l_Format, l_Pool,
                     (LPDIRECT3DTEXTURE9*) &m_Texture, NULL);
    if (l_CreateDepthStencilSurface) {
        GRAPHM->GetDevice()->
        CreateDepthStencilSurface(Width, Height, D3DFMT_D24S8,
                                  D3DMULTISAMPLE_NONE, 0, TRUE,
                                  &m_DepthStencilRenderTargetTexture, NULL);
        assert(m_DepthStencilRenderTargetTexture != NULL);
    }
    assert(m_Texture != NULL);
    assert(hr == D3D_OK);
    m_Width = Width;
    m_Height = Height;
    return hr != D3D_OK;
}

bool CTexture::SaveToFile(const std::string &Filename)
{
    std::string l_Extension = Filename.substr(Filename.size() - 4, Filename.size() );
    D3DXIMAGE_FILEFORMAT fileFormat = D3DXIFF_BMP;
    if (l_Extension == ".bmp")
        fileFormat = D3DXIFF_BMP;
    else if (l_Extension == ".jpg")
        fileFormat = D3DXIFF_JPG;
    else if (l_Extension == ".png")
        fileFormat = D3DXIFF_PNG;
    else if (l_Extension == ".tga")
        fileFormat = D3DXIFF_TGA;
    else
        fileFormat = D3DXIFF_DDS;
    HRESULT l_HR = D3DXSaveTextureToFile(Filename.c_str(), fileFormat, m_Texture, NULL);
    return l_HR == S_OK;
}