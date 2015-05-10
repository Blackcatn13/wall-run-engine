#include "CubeTexture.h"
#include "Utils\Logger.h"
#include "Core\Core.h"
#include "Core_Utils/MemLeaks.h"

bool CCubeTexture::LoadFile() {
  D3DXCreateCubeTextureFromFile(GRAPHM->GetDevice(), m_FileName.c_str(), (LPDIRECT3DCUBETEXTURE9 *)&m_Texture);
  return m_Texture != NULL;
}