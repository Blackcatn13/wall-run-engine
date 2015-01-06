#include "EffectTechnique.h"

CEffectTechnique::CEffectTechnique()
{
}
CEffectTechnique::~CEffectTechnique()
{
}
bool CEffectTechnique::BeginRender()
{
	return false;
}
bool CEffectTechnique::Refresh()
{
	return false;
}
//DirectX Methods Interface
D3DXHANDLE CEffectTechnique::GetD3DTechnique()
{
	return 0;
}