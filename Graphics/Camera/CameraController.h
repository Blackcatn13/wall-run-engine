#pragma once
#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Utils\MapManager.h"
#include <map>
#include <string>

class CObject3D;
class CCamera;
class CCameraKeyController;

typedef std::map< std::string, CObject3D*> map2Object;
typedef std::map< std::string, CCamera*> map2Camera;
typedef std::map< std::string, CCameraKeyController*> map2CameraKey;
typedef std::pair<std::string, CCamera*> PairString2Camera;
typedef std::pair<std::string, CObject3D*> PairString2Object;

class CCameraController: public CMapManager<CCamera>
{
private:
    map2Object      m_Objects;
    map2Camera      m_Cameras;
    CCamera*        m_ActiveCamera;
    float           m_Speed;
    float			m_PanSpeed;
    bool			m_FlyMode;
    float			m_FPSMaxPitch;
    float			m_ThPSMaxPitch;
    float			m_BigZoom;
    float			m_LittleZoom;

    //CINEMATICS
    std::string		m_FileName;
public:
    CCameraController();

    void AddNewCamera(std::string camName, CCamera* cam);
    void AddNewObject(std::string objName, CObject3D* obj);
    void Update(std::string camera, float dt);
    void Update(float dt);
    CCamera* getActiveCamera()
    {
        return m_ActiveCamera;
    }
    void setActiveCamera(CCamera* cam)
    {
        m_ActiveCamera = cam;
    }
    void setActiveCamera(std::string cam)
    {
        m_ActiveCamera = m_Cameras[cam];
    }

    //CINEMATICS
    bool Load(const std::string &FileName);
    bool Reload();
    bool Reload(const std::string &FileName);
    bool IsAnyCinematicPlaying();
	void Play(bool Cycle);
	void Pause(); 
	void Stop();

};
#endif