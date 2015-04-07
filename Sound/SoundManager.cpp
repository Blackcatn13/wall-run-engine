#include "SoundManager.h"
#include "XML\XMLTreeNode.h"
#include "alc.h"
#include "alut.h"


void CSoundManager::Done(){}

bool CSoundManager::LoadSounds(const std::string& xmlSoundsFile)
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(xmlSoundsFile.c_str())) {
		printf("ERROR loading the file.");
		return false;
	} 
	else 
	{
		CXMLTreeNode  m = newFile["sounds"];
		if (m.Exists()) 
		{
			int count = m.GetNumChildren();
			for (int i = 0; i < count; ++i) 
			{
				std::string l_Name = m(i).GetName();
				if (l_Name == "sound") 
				{
					Sound * newSound;
					newSound->path = m(i).GetPszISOProperty("path", "", false);
					newSound->id = m(i).GetPszISOProperty("soundId", "", false);
					AddResource(newSound->id, newSound);
				}
			}
		}

		return true;
	}
}

void CSoundManager::Update(float deltaTime){

}

void CSoundManager::Reset(){}
void CSoundManager::Pause(){}
void CSoundManager::Stop(){}

void	CSoundManager::SetGain	(float gain){}
float	CSoundManager::GetGain	(){return 0;}


bool	CSoundManager::PlayAction2D	(const std::string& action){return false;}
bool	CSoundManager::PlayAction3D	(const std::string& action, const Vect3f& position){return false;}


//-----SOURCE FUNCTIONS----------------------------
uint32	CSoundManager::CreateSource		(){return 0;} 
bool	CSoundManager::DeleteSource		(uint32 source){return false;}
bool	CSoundManager::PlaySource2D		(uint32 source, const std::string& action, bool loop){return false;}
bool	CSoundManager::PlaySource3D		(uint32 source, const std::string& action, bool loop){return false;}
bool	CSoundManager::PauseSource		(uint32 source){return false;}
bool	CSoundManager::StopSource		(uint32 source){return false;}
bool	CSoundManager::SetSourceGain	(uint32 source, float inGain){return false;}
bool	CSoundManager::GetSourceGain	(uint32 source, float& outGain){return false;}
bool	CSoundManager::SetSourcePosition	(uint32 source, const Vect3f& inPosition){return false;}
bool	CSoundManager::GetSourcePosition	(uint32 source, Vect3f& outPosition){return false;}
bool	CSoundManager::SetSourceVelocity	(uint32 source, const Vect3f& inVelocity){return false;}
bool	CSoundManager::GetSourceVelocity	(uint32 source, Vect3f& outVelocity){return false;}

//-----LISTENER FUNCTIONS-------------------------
void	CSoundManager::SetListenerPosition		(const Vect3f& inPosition){}
void	CSoundManager::GetListenerPosition		(Vect3f& outPosition){}
void	CSoundManager::SetListenerVelocity		(const Vect3f& inVelocity){}
void	CSoundManager::GetListenerVelocity		(Vect3f& outVelocity){}
void	CSoundManager::SetListenerOrientation	(const Vect3f& inAt, const Vect3f& inUp){}
void	CSoundManager::GetListenerOrientation	(Vect3f& outAt, Vect3f& outUp){}