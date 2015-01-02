#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H
#include <string>
#include <vector>

#include "Utils\Defines.h"
#include "Utils\Logger.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Utils\Named.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Object\Object3D.h"
#include "Utils\MapManager.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "Math\Vector3.h"
#include "Math\Matrix44.h"
#include "Math\Matrix33.h"
#include "Math\Matrix34.h"
#include <d3dx9.h>

#include "Lights\Light.h"
#include "Lights\LightManager.h"
#include "Lights\DirectionalLight.h"
#include "Lights\OmniLight.h"
#include "Lights\SpotLight.h"

#include "Utils\MapManager.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include "Utils\Visible.h"
#include <map>
#include <string>

using namespace luabind;

class CScriptManager
{
	private:
		lua_State *m_LS;
		std::map<std::string,std::string>  m_ScriptsMap;
		bool m_DeleteMap;

	public:
		CScriptManager();
		~CScriptManager();
		void Initialize();
		void Destroy();
		void RunCode(const std::string &Code) const;
		void RunFile(const std::string &FileName) const;
		void Load(const std::string &XMLFile);
	//	void LoadFile(const std::string &XMLFile);
		lua_State * GetLuaState() const {return m_LS;}
		void RegisterLUAFunctions();
		
		std::map<std::string, std::string>  GetScriptsMap(){return m_ScriptsMap;}
	
};

#endif

/************************************/
/***********AUXILIAR CLASSES********/
/***********************************/
/******CAMERAS******/

class CCameraInfo
{
public:
float m_NearPlane, m_FarPlane;
float m_FOV;
/*CPoint3D*/ Vect3f m_Eye, m_LookAt;
/*CPoint3D*/ Vect3f m_Up;
CCameraInfo(){}
CCameraInfo(const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, float NearPlane, float FarPlane, float FOV){}
CCameraInfo(CXMLTreeNode &atts){}
};

class CCameraKey
{
public:
CCameraInfo m_CameraInfo;
float m_Time;
CCameraKey(CCameraInfo &CameraInfo, float Time){}
};

class CCameraKeyController
{
private:
std::vector<CCameraKey *> m_Keys;
size_t m_CurrentKey, m_NextKey;
float m_CurrentTime, m_TotalTime;
bool m_Cycle;
bool m_Reverse;
void LoadXML(const std::string &FileName);
void GetCurrentKey();
public:
	CCameraKeyController(CXMLTreeNode &atts){}
	void Update(float ElapsedTime){}
	void SetCurrentTime(float CurrentTime){}
	void ResetTime(){}
	float GetTotalTime(){return 0.0f;}
	bool IsCycle() const{return false;}
	void SetCycle(bool Cycle){}
	bool IsReverse() const{return false;}
	void SetReverse(bool Reverse){}
};



/*****CINEMATICS*****/
class CCinematicPlayer
{
protected:
bool m_Playing;
float m_CurrentTime;
float m_Duration;
bool m_Cycle;
public:
	CCinematicPlayer(){}
	virtual ~CCinematicPlayer(){}
	void Init(float Duration){}
	virtual void Update(float ElapsedTime){}
	virtual void Stop(){}
	virtual void Play(bool Cycle){}
	virtual void Pause(){}
bool IsFinished() {return m_CurrentTime>=m_Duration;}
float GetDuration() {return m_Duration;}
float GetCurrentTimes() {return m_CurrentTime;}
virtual void OnRestartCycle(){}
};

class CCinematicObjectKeyFrame : public CObject3D
{
private:
float m_KeyFrameTime;
public:
	CCinematicObjectKeyFrame(CXMLTreeNode &atts){}
//KG_GET_SET_FLOAT(KeyFrameTime);
float GetKeyFrameTime(){return m_KeyFrameTime;}
void SetKeyFrameTime(float keyFrameTime){};
};

class CCinematicObject : public CCinematicPlayer
{
private:
std::vector<CCinematicObjectKeyFrame *> m_CinematicObjectKeyFrames;
size_t m_CurrentKeyFrame;
CRenderableObject *m_RenderableObject;
public:
	CCinematicObject(CXMLTreeNode &atts){}
	virtual ~CCinematicObject(){}
	bool IsOk(){return false;}
	void AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame){}
	void Update(float ElapsedTime){}
	void Stop(){}
	void OnRestartCycle(){}
};


class CCinematic : public CRenderableObject, public CCinematicPlayer
{
protected:
std::vector<CCinematicObject *> m_CinematicObjects;
public:
	CCinematic(/*MKeyValue */ CXMLTreeNode &atts){}
	virtual ~CCinematic(){}
	virtual void Stop(){}
	virtual void Play(bool Cycle){}
	virtual void Pause(){}
	void LoadXML(const std::string &Filename){}
	void AddCinematicObject(CCinematicObject *CinematicObject){}
	void Update(float ElapsedTime){}
virtual void Render(/*CKGParams &KGParams*/CGraphicsManager *RM){}
};


/****EFFECTS*****/
#define MAX_LIGHTS_BY_SHADER 10

class CEffect
{
private:
	std::string m_FileName;
	LPD3DXEFFECT m_Effect;
	//BOOL m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
	int m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
	int m_LightsType[MAX_LIGHTS_BY_SHADER];
	float m_LightsAngle[MAX_LIGHTS_BY_SHADER];
	float m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
	float m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	float m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	/*CPoint3D*/ Vect3f m_LightsPosition[MAX_LIGHTS_BY_SHADER];
	/*CPoint3D*/ Vect3f m_LightsDirection[MAX_LIGHTS_BY_SHADER];
	/*CPoint3D*/ Vect3f m_LightsColor[MAX_LIGHTS_BY_SHADER];
	D3DXHANDLE m_WorldMatrixParameter, m_ViewMatrixParameter,
	m_ProjectionMatrixParameter;
	D3DXHANDLE m_WorldViewMatrixParameter,
	m_ViewProjectionMatrixParameter,
	m_WorldViewProjectionMatrixParameter;
	D3DXHANDLE m_ViewToLightProjectionMatrixParameter;
	D3DXHANDLE m_LightEnabledParameter, m_LightsTypeParameter,
	m_LightsPositionParameter, m_LightsDirectionParameter,
	m_LightsAngleParameter, m_LightsColorParameter;
	D3DXHANDLE m_LightsFallOffParameter,
	m_LightsStartRangeAttenuationParameter,
	m_LightsEndRangeAttenuationParameter;
	D3DXHANDLE m_CameraPositionParameter;
	D3DXHANDLE m_BonesParameter;
	D3DXHANDLE m_TimeParameter;
	void SetNullParameters();
	void GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE
	&l_Handle);
	bool LoadEffect();
	void Unload();
public:
	CEffect(){}
	~CEffect(){}
	bool SetLights(size_t NumOfLights){return false;}
	bool Load(const std::string &FileName){return false;}
	bool Reload(){return false;}
	//DirectX Methods Interface
	/*LPD3DXEFFECT*/ void GetD3DEffect() const{}
	/*D3DXHANDLE*/ void GetTechniqueByName(const std::string &TechniqueName){}
};

class CEffectTechnique
{
private:
	bool m_UseCameraPosition;
	bool m_UseInverseProjMatrix;
	bool m_UseInverseViewMatrix;
	bool m_UseInverseWorldMatrix;
	bool m_UseLights;
	int m_NumOfLights;
	bool m_UseLightAmbientColor;
	bool m_UseProjMatrix;
	bool m_UseViewMatrix;
	bool m_UseWorldMatrix;
	bool m_UseWorldViewMatrix;
	bool m_UseWorldViewProjectionMatrix;
	bool m_UseViewProjectionMatrix;
	bool m_UseViewToLightProjectionMatrix;
	bool m_UseTime;
	CEffect *m_Effect;
	D3DXHANDLE m_D3DTechnique;
	std::string m_TechniqueName;
public:
	CEffectTechnique(){}
	//~CEffectTechnique(){}
	~CEffectTechnique(){}
	inline CEffect * GetEffect() const {return m_Effect;}
	bool BeginRender(){return false;}
	bool Refresh(){return false;}
	//DirectX Methods Interface
	/*D3DXHANDLE*/ void GetD3DTechnique(){}
};

//class CEffectManager : public CMapManager<CEffectTechnique>
//{
//private:
//	typedef std::map<int,std::string> TDefaultTechniqueEffectMap;
//	TDefaultTechniqueEffectMap m_DefaultTechniqueEffectMap;
//	/*CMatrix*/ Mat44f m_WorldMatrix, m_ProjectionMatrix, m_ViewMatrix,
//	m_ViewProjectionMatrix;
//	/*CMatrix*/ Mat44f m_LightViewMatrix, m_ShadowProjectionMatrix;
//	/*CPoint3D*/ Vect3f m_CameraEye;
//	CMapManager<CEffect> m_Effects;
//	CEffectTechnique *m_StaticMeshTechnique;
//	CEffectTechnique *m_AnimatedModelTechnique;
//public:
//	//CEffectManager(){}
//	~CEffectManager(){}
//	const /*CMatrix*/ Mat44f & GetWorldMatrix() const{return m_LightViewMatrix;}
//	const /*CMatrix*/ Mat44f & GetProjectionMatrix() const{return m_LightViewMatrix;}
//	const /*CMatrix*/ Mat44f & GetViewMatrix() const{return m_LightViewMatrix;
//	const /*CMatrix*/ Mat44f & GetViewProjectionMatrix(){return m_LightViewMatrix;}
//	const /*CPoint3D*/ Vect3f & GetCameraEye(){return m_LightViewMatrix;}
//	const /*CMatrix*/ Mat44f & GetLightViewMatrix() const{return m_LightViewMatrix;}
//	const /*CMatrix*/ Mat44f & GetShadowProjectionMatrix(){return m_LightViewMatrix;}
//	void ActivateCamera(const /*CMatrix*/ Mat44f &ViewMatrix, const /*CMatrix*/ Mat44f &ProjectionMatrix, const /*CPoint3D*/ Vect3f &CameraEye){}
//	void SetWorldMatrix(const /*CMatrix*/ Mat44f &Matrix){}
//	void SetProjectionMatrix(const /*CMatrix*/ Mat44f &Matrix){}
//	void SetViewMatrix(const /*CMatrix*/ Mat44f &Matrix){}
//	void SetViewProjectionMatrix(const /*CMatrix*/ Mat44f &ViewProjectionMatrix){}
//	void SetLightViewMatrix(const /*CMatrix*/ Mat44f &Matrix){}
//	void SetShadowProjectionMatrix(const /*CMatrix*/ Mat44f &Matrix){}
//	void SetCameraEye(const /*CPoint3D*/ Vect3f &CameraEye){}
//	void Load(const std::string &FileName){}
//	void Reload(){}
//	std::string GetTechniqueEffectNameByVertexDefault(unsigned short VertexType){return "lala";}
//	size_t GetMaxLights() const{return MAX_LIGHTS_BY_SHADER;}
//	CEffect * GetEffect(const std::string &Name){return new CEffect();}
//	CEffectTechnique * GetEffectTechnique(const std::string &Name){return m_StaticMeshTechnique;}
//	CEffectTechnique * GetStaticMeshTechnique() const{return m_StaticMeshTechnique;}
//	void SetStaticMeshTechnique(CEffectTechnique *StaticMeshTechnique){}
//	CEffectTechnique * GetAnimatedModelTechnique() const{return m_StaticMeshTechnique;}
//	void SetAnimatedModelTechnique(CEffectTechnique *AnimatedModelTechnique){}
//	void CleanUp(){}
//};
