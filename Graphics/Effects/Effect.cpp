#include "Effect.h"

void CEffect::SetNullParameters()
{
}
void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle)
{
}
bool CEffect::LoadEffect()
{
	return false;
}
void CEffect::Unload()
{
}

CEffect::CEffect()
{
}
CEffect::~CEffect()
{
}
bool CEffect::SetLights(size_t NumOfLights)
{
	return false;
}
bool CEffect::Load(const std::string &FileName)
{
	return false;
}
bool CEffect::Reload()
{
	Unload();
	bool isOK = Load(m_FileName);
	return isOK;
}
//DirectX Methods Interface
LPD3DXEFFECT CEffect::GetD3DEffect() const
{
	return 0;
}
D3DXHANDLE CEffect::GetTechniqueByName(const std::string &TechniqueName)
{
	return 0;
}