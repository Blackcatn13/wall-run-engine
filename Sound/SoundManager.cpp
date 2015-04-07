#include "SoundManager.h"
#include "XML\XMLTreeNode.h"
#include "alc.h"
#include "alut.h"


void CSoundManager::Done(){}

bool CSoundManager::_initAL()
{
   ALenum error;
   ALCdevice* pDevice;
   ALCcontext* pContext;

   // Get handle to default device.
   pDevice = alcOpenDevice(NULL);

   // Get the device specifier.
   //const ALCubyte* deviceSpecifier = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);

   // Create audio context.
   pContext = alcCreateContext(pDevice, NULL);

   // Set active context.
   alcMakeContextCurrent(pContext);

   // Check for an error.
   if ((error=alcGetError(pDevice)) != ALC_NO_ERROR)
   {
  	 std::string description = "Can't create openAL context (" + _getALErrorString(error) + ")";
  	 LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
  	 return false;
   }
   return true;
}

void CSoundManager::_finalizeAL()
{
   _clear();
   
   ALCcontext* pCurContext;
   ALCdevice* pCurDevice;

   // Get the current context.
   pCurContext = alcGetCurrentContext();

   // Get the device used by that context.
   pCurDevice = alcGetContextsDevice(pCurContext);

   // Reset the current context to NULL.
   alcMakeContextCurrent(NULL);

   // Release the context and the device.
   alcDestroyContext(pCurContext);
   alcCloseDevice(pCurDevice);
}

bool CSoundManager::_loadSound (const std::string& file, tIdBuffer& buffer)
{
   // Variables to load into.
   FILE *fd;
   ALenum format;
   ALenum error;
   ALsizei size;
   ALvoid* data;
   ALsizei freq;
   ALboolean loop;
   
   // Load wav data into buffers.
   alGenBuffers(1, &buffer);

   if((error=alGetError()) != AL_NO_ERROR)
   {    
  	 alDeleteBuffers(1,&buffer);
  	 std::string description = "Error: Can't create openAL Buffer (" + _getALErrorString(error)  + ")";
  	 LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
  	 return false;    
   }

   // Check if the file exists
   if ((fd=fopen(file.c_str(),"r"))==NULL)
   {
  	 alDeleteBuffers(1,&buffer);
  	 std::string description = "Error: Can't open file " + file;
  	 LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
  	 return false;
   }
   else
   {
  	 fclose(fd);
   }
   alutLoadWAVFile((ALbyte*)file.c_str(), &format, &data, &size, &freq, &loop);
   alBufferData(buffer, format, data, size, freq);
   alutUnloadWAV(format, data, size, freq);
   if ((error=alGetError()) != AL_NO_ERROR)
   {   	 
  	 alDeleteBuffers(1,&buffer);
  	 std::string description = "Error: Can't load sound file " + file + " (" + _getALErrorString(error)  + ")";
  	 LOGGER->AddNewLog(ELL_ERROR, "CSoundManager:: %s", description.c_str());
  	 return false;
   }   	 
   return true;
}

std::string 	CSoundManager::_getALErrorString	(ALenum err){return "";}
void CSoundManager::_clear ()
{
   // Delete AL objets
   ALuint aux;    
   std::map<tAction, tIdBuffer>::iterator itBuffer;
   for( itBuffer = m_Buffers.begin(); itBuffer != m_Buffers.end(); ++itBuffer )
   {
  	 aux = (*itBuffer).second;
  	 alDeleteBuffers (1,&aux);
   }
   
   std::vector<tInfoSource>::iterator itSource;
   for( itSource = m_Sources.begin(); itSource != m_Sources.end(); ++itSource )
   {
  	 aux = (*itSource).m_uSource;
  	 alDeleteSources (1,&aux);
   }
   
   m_Buffers.clear();
   m_Sources.clear();
}


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