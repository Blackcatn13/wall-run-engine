#pragma once

#ifndef CINEMATIC_CONTROLLER_H
#define CINEMATIC_CONTROLLER_H

#include "Cinematic.h"
#include "Utils\MapManager.h"

class CCinematicController : public CMapManager<CCinematic>
{
private:
    std::string m_FileName;
public:
    CCinematicController();
    ~CCinematicController();
    bool Load(const std::string &FileName);
    bool Reload();
    bool Reload(const std::string &FileName);
    void Update(float ElapsedTime);
};

#endif