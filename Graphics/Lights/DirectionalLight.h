#ifndef CDIRECTIONALLIGHT_H
#define CDIRECTIONALLIGHT_H
#include "Light.h"
#include "Math\Vector3.h"
#include "GraphicsManager.h"

class CDirectionalLight : public CLight
{
protected:
	/*CPoint3D*/ Vect3f m_Direction;
public:
	CDirectionalLight() : CLight() {}
	void SetDirection(const /*CPoint3D*/ Vect3f &Direction)
	{
		m_Direction = Direction;
	}
	/*CPoint3D*/ Vect3f GetDirection() const
	{
		return m_Direction;
	}
	virtual void Render(CGraphicsManager *RM);
};

#endif
