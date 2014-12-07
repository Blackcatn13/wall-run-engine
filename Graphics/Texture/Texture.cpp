#include "Texture.h"
#include "Utils/Defines.h"
#include "GraphicsManager.h"
#include "Core/Core.h"

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
    HRESULT l_HR = D3DXCreateTextureFromFile(GRAPHM->GetDevice(), m_FileName.c_str(), &m_Texture);
    return m_Texture != NULL;
    return S_OK(l_HR);
}

void CTexture::Unload()
{
    CHECKED_RELEASE(m_Texture);
}