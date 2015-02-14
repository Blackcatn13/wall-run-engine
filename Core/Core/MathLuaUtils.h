#ifndef CMATHLUAUTILS_H
#define CMATHLUAUTILS_H

#include "Utils\Defines.h"
#include "Math/Vector3.h"

class CCMathLuaUtils
{

public:
    CCMathLuaUtils();
    ~CCMathLuaUtils();
    Vect3f Normalize(Vect3f vector);
};

#endif
