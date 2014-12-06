//----------------------------------------------------------------------------------
// CFPSCamera class
// Author: Enric Vergara
//
// Description:
// Clase que representa una camara de tipo FPShooter, es decir, el ojo esta en el centro de una esfera y el lookat se mueve por la superficie de la esfera.
// El centro de la esfera es la posición del objeto3D que nos pasan por el constructor.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_FPS_CAMERA_H_
#define INC_FPS_CAMERA_H_

#include "Camera.h"

class CFPSCamera: public CCamera
{

public:

	CFPSCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D);
	CFPSCamera();
	virtual ~CFPSCamera(){/*Nothing*/;}

	//---Interfaz de CCamera
	virtual Vect3f					GetDirection		() const;
	virtual Vect3f					GetLookAt				() const;
	virtual Vect3f					GetEye					() const;
	virtual Vect3f					GetVecUp				() const;

	//---Interfaz de CFPSCamera
	//...	

};

#endif // INC_FPS_CAMERA_H_