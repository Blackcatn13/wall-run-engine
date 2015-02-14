#ifndef COMNILIGHT_H
#define COMNILIGHT_H

#include "Light.h"

class COmniLight : public CLight
{
public:
    COmniLight() : CLight() {}
    ////////////////////////////
    void SetShadowMap(CGraphicsManager *RM) {};
};


#endif
