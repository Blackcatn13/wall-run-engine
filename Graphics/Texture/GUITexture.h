#pragma once
#ifndef GUI_TEXTURE_H
#define GUI_TEXTURE_H

#include "Texture.h"

class CGUITexture : public CTexture
{
protected:
	bool LoadFile();
};

#endif