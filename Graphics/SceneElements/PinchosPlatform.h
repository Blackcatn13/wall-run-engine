#pragma once
#ifndef PINCHOS_PLATFORM_H
#define PINCHOS_PLATFORM_H

#include <string>
#include "Math\Vector3.h"
#include "BreakablePlatform.h"


class CTrigger;
class CPinchosPlatform : public CBreakablePlatform
{
protected:

    // std::string		m_TriggerName;
    float	m_Offset;
    bool	m_FromZ;
    bool	m_FromX;
public:
    CPinchosPlatform(std::string platformName, std::string coreName, std::string triggerName, float offset, bool fromX, bool fromZ);

    ~ CPinchosPlatform ();
    void FallingIntoPlatform();


};

#endif