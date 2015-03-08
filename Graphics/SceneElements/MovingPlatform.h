#pragma once
#ifndef MOVING_PLATFORM_H
#define MOVING_PLATFORM_H

#include <string>
#include "Math\Vector3.h"
#include "StaticPlatform.h"
#include "AI\AIController.h"


class CMovingPlatform : public CStaticPlatform, public CAIController
{
protected:

public:
    CMovingPlatform(std::string platformName, std::string coreName);

    ~ CMovingPlatform ();


};

#endif