#include "Billboard.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "Camera\CameraController.h"

CBillboard::CBillboard()
	: m_size(1)
	, m_position(v3fZERO)
{
}

CBillboard::CBillboard(int size)
	: m_size(size)
	, m_position(v3fZERO)
{
}

CBillboard::CBillboard(int size, Vect3f pos)
	: m_size(size)
	, m_position(pos)
{
}

CBillboard::~CBillboard()
{
}

void CBillboard::Render(CGraphicsManager* GM)
{
	CCamera* actCam = CAMCONTM->getActiveCamera();
	Vect3f up_cam = actCam->GetVecUp().Normalize();
	Vect3f up =  up_cam * m_size / 2;
	Vect3f direction = actCam->GetDirection().Normalize();
	Vect3f right = (up_cam ^ direction) * m_size / 2;
	Vect3f ul = m_position + up - right;
	Vect3f ur = m_position + up + right;
	Vect3f dl = m_position - up - right;
	Vect3f dr = m_position - up + right;
	GM->DrawQuad3D(ul, ur, dl, dr);
}
