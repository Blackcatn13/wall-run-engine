#include "Core\CoreLuaWrapper.h"
#include "Utils\Defines.h"
#include "Core_Utils/MemLeaks.h"

CCoreLuaWrapper::CCoreLuaWrapper()
{
    m_CoreInstance = CCORE;
}

CCoreLuaWrapper::~CCoreLuaWrapper()
{
    //CHECKED_DELETE(m_CoreInstance);
}