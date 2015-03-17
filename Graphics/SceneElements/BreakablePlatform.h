#pragma once
#ifndef BREAKABLE_PLATFORM_H
#define BREAKABLE_PLATFORM_H

#include <string>
#include "Math\Vector3.h"
#include "StaticPlatform.h"


class CTrigger;
class CBreakablePlatform : public CStaticPlatform
{
protected:

    std::string		m_TriggerName;
public:
    CBreakablePlatform(std::string platformName, std::string coreName, std::string triggerName);

    ~ CBreakablePlatform ();
    void DisablePlatform(float dt);


};

#endif