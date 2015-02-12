#include "Texture.h"
#include "Utils/Defines.h"
#include "GraphicsManager.h"
#include "Core/Core.h"
#include "Utils\Logger.h"

CTexture::CTexture()
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


