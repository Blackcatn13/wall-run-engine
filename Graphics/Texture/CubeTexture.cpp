#include "CubeTexture.h"
#include "Utils\Logger.h"
#include "Core\Core.h"

bool CCubeTexture::LoadFile()
{
    HRESULT l_HR = D3DXCreateCubeTextureFromFile(GRAPHM->GetDevice(), m_FileName.c_str(), (LPDIRECT3DCUBETEXTURE9*)&m_Texture);
    return m_Texture != NULL;
    return S_OK(l_HR);
}