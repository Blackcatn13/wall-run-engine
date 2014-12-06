#include "TextureManager.h"

void CTextureManager::Reload()
{
	for(TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it)
	{
		it->second->Reload();
	}
}