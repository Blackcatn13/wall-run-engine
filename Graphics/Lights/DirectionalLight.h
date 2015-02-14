#ifndef CDIRECTIONALLIGHT_H
#define CDIRECTIONALLIGHT_H
#include "Light.h"
#include "Math\Vector3.h"
#include "Math\Vector2.h"


class CGraphicsManager;

class CDirectionalLight : public CLight
{
protected:
	/*CPoint3D*/ Vect3f m_Direction;
	Vect2f m_OrthoShadowMapSize;
	D3DXMATRIX * l_D3dMatrix; 
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

	void SetShadowMap(CGraphicsManager *RM);

	void SetOrthoShadowMapSize(Vect2f VectSize)
	{
		m_OrthoShadowMapSize = VectSize;	
	}

	Vect2f GetOrthoShadowMapSize(Vect2f VectSize)
	{
		return m_OrthoShadowMapSize;	
	}


};

#endif
