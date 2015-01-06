#pragma once
#ifndef CAMERA_KEY_H_
#define CAMERA_KEY_H_

class CCameraInfo;

class CCameraKey
{
public:
	CCameraInfo*			m_CameraInfo;
	float					m_Time;

	CCameraKey(CCameraInfo *CameraInfo, float Time);
	
	float GetTime() { return m_Time;}
};

#endif

