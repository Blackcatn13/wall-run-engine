#pragma once
#ifndef ORTHO_FIXED_CAMERA_CONTROLLER_H
#define ORTHO_FIXED_CAMERA_CONTROLLER_H

#include "Utils\MapManager.h"
#include "Camera\CameraController.h"
#include "Math\Vector3.h"
#include "Camera\OrthoFixedCamera.h"


//class COrthoFixedCamera;
class COrthoFixedCameraController: public CCameraController
{
private:
	Vect3f m_Shadow, m_Position;
	float m_Width, m_Heigth, m_StartAttenuation, m_EndAttenuation; 
	COrthoFixedCamera m_Camera;

public:
	COrthoFixedCameraController(){};
	COrthoFixedCameraController(Vect3f shadow, Vect3f position, float width, float heigth, float aspectRatio, float endAttenuation);
	//(m_Position - m_Direction, 
	//	m_Position, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 1.0f, m_EndRangeAttenuation);
	~COrthoFixedCameraController();

	COrthoFixedCamera GetCamera()
	{
		return m_Camera;
	}
  
};
#endif