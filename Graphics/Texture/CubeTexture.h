#pragma once
#ifndef CUBE_TEXTURE_H
#define CUBE_TEXTURE_H

#include "Texture.h"

class CCubeTexture : public CTexture {
 protected:
  bool LoadFile();
};

#endif