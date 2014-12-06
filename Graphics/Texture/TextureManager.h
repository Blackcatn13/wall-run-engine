#pragma once
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "MapManager.h"
#include "Texture.h"

class CTextureManager : public CMapManager<CTexture>
{
public:
	CTextureManager()
	{
		m_Resources = TMapResource();
	}

	~CTextureManager()
	{
		Destroy();
	}

	void Reload ();
};

#endif