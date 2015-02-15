#pragma once
#ifndef INC_ORTO_FIXED_CAMERA_H_
#define INC_ORTO_FIXED_CAMERA_H_

#include "Camera.h"
#include "Math\Matrix44.h"
#include "Math\Vector3.h"

class COrthoFixedCamera: public CCamera
{
	private:
		float m_Width; 
		float m_Height;

	public:

		COrthoFixedCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D);
		COrthoFixedCamera();
		virtual ~COrthoFixedCamera() {;}

		//---Interfaz de CCamera
		virtual Vect3f					GetDirection		() const;
		virtual Vect3f					GetLookAt				() const;
		virtual Vect3f					GetEye					() const;
		virtual Vect3f					GetVecUp				() const;

		//---ShadowMap

		Mat44f		GetViewMatrix();
		Mat44f		GetProjectionMatrix();
	
		void SetWidth(float width)
		{
			m_Width = width;
		}

		void SetHeight(float height)
		{
			m_Height = height;
		}
		//...	

	};

#endif // INC_FPS_CAMERA_H_