#pragma once

#ifndef CINEMATIC_OBJECT_H
#define CINEMATIC_OBJECT_H

#include "CinematicPlayer.h"
#include "CinematicObjectKeyFrame.h"
#include "Renderable\RenderableObject.h"
#include "XML\XMLTreeNode.h"
#include <vector>

class CCinematicObject : public CCinematicPlayer
{
private:
	std::vector<CCinematicObjectKeyFrame *>		m_CinematicObjectKeyFrames;
	size_t										m_CurrentKeyFrame;
	CRenderableObject							*m_RenderableObject;

	Vect3f m_PosInterpolated;
	float m_YawInterpolated;
	float m_PitchInterpolated;
	float m_RollInterpolated;
	Vect3f m_ScaleInterpolated;

	void InterpolatePosition(Vect3f PointA, Vect3f PointB, float TimeA, float TimeB, float ElapsedTime);
	void InterpolateYaw(float YawA, float YawB, float TimeA, float TimeB, float ElapsedTime);
	void InterpolatePitch(float PitchA, float PitchB, float TimeA, float TimeB, float ElapsedTime);
	void InterpolateRoll(float RollA, float RollB, float TimeA, float TimeB, float ElapsedTime);
	void InterpolateScale(Vect3f ScaleA, Vect3f ScaleB, float TimeA, float TimeB, float ElapsedTime);
public:
	CCinematicObject(CXMLTreeNode &atts);
	virtual ~CCinematicObject();
	bool IsOk() {return (m_RenderableObject != NULL);}
	void AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame);
	void Update(float ElapsedTime);
	void Stop();
	void OnRestartCycle();
};

#endif