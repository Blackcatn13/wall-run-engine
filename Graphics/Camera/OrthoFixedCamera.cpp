#include "Camera\OrthoFixedCamera.h"

#include <assert.h>
#include "Utils\Defines.h"

COrthoFixedCamera::COrthoFixedCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D)
: CCamera( zn, zf, fov, aspect, object3D, TC_ORTHFIX)
{}

COrthoFixedCamera::COrthoFixedCamera()
: CCamera()
{}

Vect3f COrthoFixedCamera::GetDirection () const
{
	assert(m_pObject3D);

	return (GetLookAt() - GetEye());
}

Vect3f COrthoFixedCamera::GetLookAt () const
{
	assert(m_pObject3D);

	float yaw		= m_pObject3D->GetYaw();
	float pitch	= m_pObject3D->GetPitch();
	Vect3f pos	= m_pObject3D->GetPosition();

	//Pasamos de coordenadas esfericas a coordenadas cartesianas
	Vect3f vEyePt(	cos(yaw) * cos(pitch), 
		sin(pitch),
		sin(yaw) * cos(pitch) );

	return (pos + vEyePt); 
}

Vect3f COrthoFixedCamera::GetEye () const
{
	assert(m_pObject3D);

	return m_pObject3D->GetPosition();


}

Vect3f COrthoFixedCamera::GetVecUp () const
{
	assert(m_pObject3D);

	float yaw		= m_pObject3D->GetYaw();
	float pitch	= m_pObject3D->GetPitch();

	Vect3f vUpVec(	-cos(yaw) * sin(pitch), 
		cos(pitch), 
		-sin(yaw) * sin(pitch) );

	return vUpVec;
}

Mat44f	COrthoFixedCamera::GetViewMatrix()
{
	D3DXVECTOR3 l_Eye(GetEye().x, GetEye().y, GetEye().z);
	D3DXVECTOR3 l_LookAt(GetLookAt().x, GetLookAt().y, GetLookAt().z);
	D3DXMATRIX m_matView;
	//Vect3f l_CrossVect = CrossProduct(GetVecUp(), GetLookAt());
	D3DXVECTOR3 l_VUP(GetVecUp().x,GetVecUp().y,GetVecUp().z);
	D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP); //CroosProduct L_VUP y l_LookAt?
	return (Mat44f)m_matView;
}

Mat44f		COrthoFixedCamera::GetProjectionMatrix()
{
	D3DXMATRIX m_MatProj;
	D3DXMatrixOrthoLH(&m_MatProj,m_Width, m_Height, 0.1f,GetZf() );
	return(Mat44f)m_MatProj;
}

Vect3f CrossProduct(Vect3f vect1, Vect3f vect2)
{
	/*vec vector; 
vector.x = Ay*Bz - By*Az; 
vector.y = Bx*Az - Ax*Bz; 
vector.z = Ax*By - Ay*Bx; */
	Vect3f l_Vector;
	l_Vector.x = (vect1.y*vect2.z) - (vect2.y*vect1.z);
	l_Vector.y = (vect2.x*vect1.z) - (vect1.x*vect2.z);
	l_Vector.z = (vect1.x*vect2.y) - (vect1.y*vect2.x);

	return l_Vector;

}