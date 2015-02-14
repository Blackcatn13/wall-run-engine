#include "TextureManager.h"
#include "Texture.h"

void CTextureManager::Reload()
{
    for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
        it->second->Reload();
    }
}

CTexture * CTextureManager::GetResource(const std::string &Name)
{
    TMapResource::iterator it = m_Resources.find(Name);
    if (it != m_Resources.end()) {
        return it->second;
    } else {
        CTexture* newTexture = new CTexture();
        bool loaded = newTexture->Load(Name);
        if (loaded) {
            AddResource(Name, newTexture);
            return newTexture;
        } else {
            return NULL;
        }
    }
}