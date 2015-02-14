#ifndef CCORELUAWRAPPER_H
#define CCORELUAWRAPPER_H

#include "Core\Core.h"
class CCoreLuaWrapper
{
protected:
    CCore *m_CoreInstance;
public:
    CCoreLuaWrapper();
    ~CCoreLuaWrapper();

    CCore* GetCoreInstance()
    {
        return m_CoreInstance;
    }

};



#endif
