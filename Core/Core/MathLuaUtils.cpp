#include "MathLuaUtils.h"

CCMathLuaUtils::CCMathLuaUtils(){}
CCMathLuaUtils::~CCMathLuaUtils(){}

Vect3f CCMathLuaUtils::Normalize(Vect3f vector)
{
	return vector.Normalize();
}