
//----------------------------------------------------------------------------------
// CSoundManager class
// Author: Enric Vergara
//
// Description:
// This secures availability of the necessary sounds functions.
// It internally uses the OpenAL library.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_SOUND_MANAGER_H_
#define INC_SOUND_MANAGER_H_
#include "OpenAL\include\al.h"
#include <map>
#include <vector>
//#include "Script/ScriptRegister.h"
#include "Utils/LerpAnimator1D.h"
#include "Math\Vector3.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\Named.h"

typedef struct StSound{
	std::string id;
	std::string path;
}Sound;

class CSoundManager :public CTemplatedVectorMapManager<Sound>//:public CScriptRegister
{
public:
	//----Init and End protocols------------------------------------
	CSoundManager(): m_bIsOk(false), m_bSoundON(true), m_bPause(false) {}
	virtual ~CSoundManager() {Done();}
	bool	Init		();
	void	Done		();
	bool	IsOk		() const { return m_bIsOk; }
	//---------------------------------------------------------------
	//----CScriptRegister interface-------------------
	//virtual void	RegisterFunctions				(CScriptManager* scriptManager);
	//-----------GENERAL FUNCTIONS---------------------
	void	Update		(float deltaTime);
	bool	LoadSounds	(const std::string& xmlSoundsFile);
	void	Reset		();
	void	Pause		();
	void	Stop		();
	void	SoundOn	() {m_bSoundON=true;}
	void	SoundOff	() {m_bSoundON=false;}
	void	SetGain	(float gain);
	float	GetGain	();
	 
	//---------ACTION FUNCTIONS-------------------------------
	bool	PlayAction2D	(const std::string& action);
	bool	PlayAction3D	(const std::string& action, const Vect3f& position);
		
	//-----SOURCE FUNCTIONS----------------------------
	uint32	CreateSource		(); 
	bool	DeleteSource		(uint32 source);
	bool	PlaySource2D		(uint32 source, const std::string& action, bool loop);
	bool	PlaySource3D		(uint32 source, const std::string& action, bool loop);
	bool	PauseSource		(uint32 source);
	bool	StopSource		(uint32 source);
	bool	SetSourceGain	(uint32 source, float inGain);
	bool	GetSourceGain	(uint32 source, float& outGain);
	bool	SetSourcePosition	(uint32 source, const Vect3f& inPosition);
	bool	GetSourcePosition	(uint32 source, Vect3f& outPosition);
	bool	SetSourceVelocity	(uint32 source, const Vect3f& inVelocity);
	bool	GetSourceVelocity	(uint32 source, Vect3f& outVelocity);
	 
		
	//-----LISTENER FUNCTIONS-------------------------
	void	SetListenerPosition		(const Vect3f& inPosition);
	void	GetListenerPosition		(Vect3f& outPosition);
	void	SetListenerVelocity		(const Vect3f& inVelocity);
	void	GetListenerVelocity		(Vect3f& outVelocity);
	void	SetListenerOrientation	(const Vect3f& inAt, const Vect3f& inUp);
	void	GetListenerOrientation	(Vect3f& outAt, Vect3f& outUp);
	
private:
	//Types
	typedef struct
	{
		ALuint	m_uSource;
		bool	m_bReserved;
	}tInfoSource;
	
	typedef std::string 	tSoundFile;
	typedef std::string	 tAction;
	typedef ALuint		tIdBuffer;
	
void		Release		();
	bool		_initAL			();
	void		_finalizeAL		();
	bool		_loadSound		(const std::string& file, tIdBuffer& buffer);
	void		_clear			();
	int		_getSource		(bool reserved=false);
	std::string	_getALErrorString	(ALenum err);
private:	
	bool	m_bIsOk;	      // Initialization boolean control
	bool	m_bSoundON;
	bool	m_bPause;
	std::map<tAction, tIdBuffer>		m_Buffers;
	std::vector<tInfoSource>		m_Sources;
	std::map<uint32,CLerpAnimator1D>	m_SourcesFadeInOut;
	
};
#endif // INC_SOUND_MANAGER_H_
