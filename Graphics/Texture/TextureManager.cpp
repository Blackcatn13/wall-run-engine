#include "TextureManager.h"
#include "Texture.h"
#include "Core_Utils/MemLeaks.h"

void CTextureManager::Reload() {
  for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
    it->second->Reload();
  }
}

CTexture *CTextureManager::GetResource(const std::string &Name, bool gui) {
  TMapResource::iterator it = m_Resources.find(Name);
  if (it != m_Resources.end()) {
    return it->second;
  } else {
    CTexture *newTexture = new CTexture();
    bool loaded;
    if (gui) loaded = newTexture->LoadGUI(Name);
    else loaded = newTexture->Load(Name);
    if (loaded) {
      AddResource(Name, newTexture);
      return newTexture;
    } else {
      return NULL;
    }
  }
}

void CTextureManager::RemoveResource(const std::string &Name) {
  auto it = m_Resources.find(Name);
  if (it != m_Resources.end()) {
    CHECKED_DELETE(it->second);
    m_Resources.erase(it);
  }
}