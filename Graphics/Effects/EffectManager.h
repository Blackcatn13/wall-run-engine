//----------------------------------------------------------------------------------
// CFontManager class
#pragma once
#ifndef INC_EFFECT_MANAGER_H_
#define INC_EFFECT_MANAGER_H_

//#include "EffectTechnique.h"
//#include "Utils\MapManager.h"

#include "Utils\MapManager.h"
#include "Math\Vector3.h"
#include "Math\Matrix44.h"
#include <string>

class CEffectTechnique;
class CEffect;

class CEffectManager : public CMapManager<CEffectTechnique>
{
	private:
		typedef std::map<int,std::string> TDefaultTechniqueEffectMap;
		TDefaultTechniqueEffectMap m_DefaultTechniqueEffectMap;
		Mat44f m_WorldMatrix, m_ProjectionMatrix, m_ViewMatrix, m_ViewProjectionMatrix;
		Mat44f m_LightViewMatrix, m_ShadowProjectionMatrix;
		Vect3f m_CameraEye;
		CMapManager<CEffect> m_Effects;
		std::string m_FileName;
	public:
		CEffectManager();
		~CEffectManager();
		const Mat44f & GetWorldMatrix() const;
		const Mat44f & GetProjectionMatrix() const;
		const Mat44f & GetViewMatrix() const;
		const Mat44f & GetViewProjectionMatrix();
		const Vect3f & GetCameraEye();
		const Mat44f & GetLightViewMatrix() const;
		const Mat44f & GetShadowProjectionMatrix();
		void ActivateCamera(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix,
		const Vect3f &CameraEye);
		void SetWorldMatrix(const Mat44f &Matrix);
		void SetProjectionMatrix(const Mat44f &Matrix);
		void SetViewMatrix(const Mat44f &Matrix);
		void SetViewProjectionMatrix(const Mat44f &ViewProjectionMatrix);
		void SetLightViewMatrix(const Mat44f &Matrix);
		void SetShadowProjectionMatrix(const Mat44f &Matrix);
		void SetCameraEye(const Vect3f &CameraEye);
		void Load(const std::string &FileName);
		void Reload();
		std::string GetTechniqueEffectNameByVertexDefault(unsigned short VertexType);
		size_t GetMaxLights() const;
		CEffect * GetEffect(const std::string &Name);
		CEffectTechnique * GetEffectTechnique(const std::string &Name);
		void CleanUp();
		//apaño para default vertex repetidos
		//int GetDefaultVertex(std::string TechniqueName);
};

#endif //INC_EFFECT_MANAGER_H_