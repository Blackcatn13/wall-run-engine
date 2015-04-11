#include "WWSoundManager.h"
#include "XML\XMLTreeNode.h"
#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/Plugin/AllPluginsRegistrationHelpers.h>
#include <Windows.h>
#include <assert.h>
#include "Utils\Logger.h"

namespace AK {
#ifdef WIN32
void *AllocHook( size_t in_size ) {
  return malloc( in_size );
}
void FreeHook( void *in_ptr ) {
  free( in_ptr );
}
// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
// you may implement it with a simple malloc().
void *VirtualAllocHook(
  void *in_pMemAddress,
  size_t in_size,
  DWORD in_dwAllocationType,
  DWORD in_dwProtect
) {
  return VirtualAlloc( in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect );
}
void VirtualFreeHook(
  void *in_pMemAddress,
  size_t in_size,
  DWORD in_dwFreeType
) {
  VirtualFree( in_pMemAddress, in_size, in_dwFreeType );
}
#endif
}

CWWSoundManager::CWWSoundManager() :
  m_LastId(0x00000001)
{}

void CWWSoundManager::Done() {
  AK::MusicEngine::Term();
  AK::SoundEngine::Term();
  m_lowLevelIO->Term();

  if ( AK::IAkStreamMgr::Get() )
    AK::IAkStreamMgr::Get()->Destroy();
  AK::MemoryMgr::Term();
}


bool CWWSoundManager::Init() {
  //
  // Create and initialize an instance of the default memory manager. Note
  // that you can override the default memory manager with your own. Refer
  // to the SDK documentation for more information.
  //

  AkMemSettings memSettings;
  memSettings.uMaxNumPools = 20;

  if ( AK::MemoryMgr::Init( &memSettings ) != AK_Success ) {
    assert( ! "Could not create the memory manager." );
    return false;
  }

  //
  // Create and initialize an instance of the default streaming manager. Note
  // that you can override the default streaming manager with your own. Refer
  // to the SDK documentation for more information.
  //

  AkStreamMgrSettings stmSettings;
  AK::StreamMgr::GetDefaultSettings( stmSettings );

  // Customize the Stream Manager settings here.

  if ( !AK::StreamMgr::Create( stmSettings ) ) {
    assert( ! "Could not create the Streaming Manager" );
    return false;
  }

  //
  // Create a streaming device with blocking low-level I/O handshaking.
  // Note that you can override the default low-level I/O module with your own. Refer
  // to the SDK documentation for more information.
  //
  AkDeviceSettings deviceSettings;
  AK::StreamMgr::GetDefaultDeviceSettings( deviceSettings );

  // Customize the streaming device settings here.
  m_lowLevelIO = new CAkDefaultIOHookBlocking();
  // CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
  // in the Stream Manager, and registers itself as the File Location Resolver.
  if ( m_lowLevelIO->Init( deviceSettings ) != AK_Success ) {
    assert( ! "Could not create the streaming device and Low-Level I/O system" );
    return false;
  }

  AkInitSettings initSettings;
  AkPlatformInitSettings platformInitSettings;
  AK::SoundEngine::GetDefaultInitSettings( initSettings );
  AK::SoundEngine::GetDefaultPlatformInitSettings( platformInitSettings );

  if ( AK::SoundEngine::Init( &initSettings, &platformInitSettings ) != AK_Success ) {
    assert( ! "Could not initialize the Sound Engine." );
    return false;
  }

  //
  // Initialize the music engine
  // Using default initialization parameters
  //

  AkMusicSettings musicInit;
  AK::MusicEngine::GetDefaultInitSettings( musicInit );

  if ( AK::MusicEngine::Init( &musicInit ) != AK_Success ) {
    assert( ! "Could not initialize the Music Engine." );
    return false;
  }

  AK::SoundEngine::RegisterAllPlugins();

  m_lowLevelIO->SetBasePath( L"./Data/" );
  ////////
  /*AkBankID bankID;
  if ( AK::SoundEngine::LoadBank( "Init.bnk", AK_DEFAULT_POOL_ID, bankID ) != AK_Success ) {
    //SetLoadFileErrorMessage( "Init.bnk" );
    return false;
  }

  //AkBankID bankID; // Not used
  if ( AK::SoundEngine::LoadBank( "InteractiveMusic.bnk", AK_DEFAULT_POOL_ID, bankID ) != AK_Success ) {
    //SetLoadFileErrorMessage( "InteractiveMusic.bnk" );
    return false;
  }*/

}

void CWWSoundManager::Render() {
  AK::SoundEngine::RenderAudio();
}

void CWWSoundManager::Load(std::string file) {
  m_fileName = file;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(file.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["WWise"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      AkBankID bankID;
      for (int i = 0; i < count; ++i) {
        std::string name = m(i).GetName();
        if (name == "Bank") {
          std::string bnkName = m(i).GetPszISOProperty("bnkName", "", false);
          if (AK::SoundEngine::LoadBank(bnkName.c_str(), AK_DEFAULT_POOL_ID, bankID) != AK_Success) {
            LOGGER->AddNewLog(ELL_ERROR, "Bank %s not correctly loaded", bnkName.c_str());
          }
        } else if (name == "GameObject") {
          std::string goName = m(i).GetPszISOProperty("goName", "", false);
          m_GameObjects[goName] = ++m_LastId;
        }
      }
    }
  }
  //static const AkGameObjectID GAME_OBJECT_MUSIC = 100;
  for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it) {
    AK::SoundEngine::RegisterGameObj(it->second);
  }
  AkSoundPosition soundPos;
  soundPos.Position.X = 0;
  soundPos.Position.Y = 1;
  soundPos.Position.Z = 0;
  soundPos.Orientation.X = -1;
  soundPos.Orientation.Y = soundPos.Orientation.Z = 0;
  AK::SoundEngine::SetPosition(m_GameObjects["Music"], soundPos);
  //Start the interactive music
  static const AkUniqueID IM_START = 3952084898U;
  AkPlayingID m_iPlayingID = AK::SoundEngine::PostEvent(
                               L"BackGround",
                               m_GameObjects["Music"],
                               AK_EnableGetMusicPlayPosition );
}

void CWWSoundManager::SetListenerPosition(Vect3f pos) {
  AkListenerPosition lpos;
  lpos.OrientationFront.X = 1;
  lpos.OrientationFront.Y = 0;
  lpos.OrientationFront.Z = 0;
  lpos.OrientationTop.X = 0;
  lpos.OrientationTop.Y = 1;
  lpos.OrientationTop.Z = 0;
  lpos.Position.X = pos.x;
  lpos.Position.Y = pos.y;
  lpos.Position.Z = pos.z;
  AK::SoundEngine::SetListenerPosition(lpos);
  AkSoundPosition soundPos;
  soundPos.Position.X = -pos.x;
  soundPos.Position.Y = pos.y;
  soundPos.Position.Z = -pos.z;
  soundPos.Orientation.X = -1;
  soundPos.Orientation.Y = soundPos.Orientation.Z = 0;
  AK::SoundEngine::SetPosition(m_GameObjects["Music"], soundPos);
}

void CWWSoundManager::PlayEvent(std::string eventName, std::string GameObject) {
  AkPlayingID m_iPID = AK::SoundEngine::PostEvent(eventName.c_str(), m_GameObjects[GameObject]);
}
