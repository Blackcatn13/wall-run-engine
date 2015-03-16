//----------------------------------------------------------------------------------
// CThPSCamera class
// Author: Enric Vergara
//
// Description:
// Clase que representa una camara de tipo Esferica, es decir, se mueve por la superficie de una esfera el centro de la
// cual es el objeto3d
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_SPHERICAL_CAMERA_H_
#define INC_SPHERICAL_CAMERA_H_
#include "Camera.h"
#include <vector>
#include <string>

class CThPSCamera : public CCamera
{
public:
	std::vector<Vect3f> m_path;
	std::string			m_pathFile;
	Vect3f				m_LastPlayerPos;
	int					m_CurrentWaypoint;
	int					m_NextWaypoint;
	
    CThPSCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, float zoom);
    CThPSCamera();
    virtual ~CThPSCamera()
    {
    }

    //---Interfaz de CCamera
    virtual Vect3f					GetDirection		() const;
    virtual Vect3f					GetLookAt				() const;
    virtual Vect3f					GetEye					() const;
    virtual Vect3f					GetVecUp				() const;


    //---Interfaz de CThPSCamera
    void										SetZoom					(float zoom);
    void										AddZoom					(float zoom);
    float										GetZoom					() const
    {
        return m_fZoom;
    }

	//---Interfaz 3DCamera y 2DCamera
	bool										LoadPathFromFile(std::string filename);
	std::vector<Vect3f>							GetMPath();
	Vect3f										GetPathPoint(int i);
	int											GetPathSize();
	Vect3f										GetLastPlayerPos();
	void										SetLastPlayerPos(Vect3f lPos);
	int											GetCurrentWaypoint();
	void										SetCurrentWaypoint(int i);
	int											GetNextWaypoint();
	void										SetNextWaypoint(int i);

private:
    float		m_fZoom;
};
#endif // INC_SPHERICAL_CAMERA_H_

