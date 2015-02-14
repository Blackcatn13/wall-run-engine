#include "Core\CoreLuaWrapper.h"
#include "Utils\Defines.h"

CCoreLuaWrapper::CCoreLuaWrapper()
{
    m_CoreInstance = CCORE;
}

CCoreLuaWrapper::~CCoreLuaWrapper()
{
    //CHECKED_DELETE(m_CoreInstance);
}