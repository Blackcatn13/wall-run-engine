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

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED


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
  m_LastId(0x00000001),
  m_lowLevelIO(NULL)
{}

void CWWSoundManager::Done() {
#ifndef AK_OPTIMIZED
  //
  // Terminate Communication Services
  //
  AK::Comm::Term();
#endif // AK_OPTIMIZED

  AK::MusicEngine::Term();
  AK::SoundEngine::Term();
  m_lowLevelIO->Term();
  delete m_lowLevelIO;
  if ( AK::IAkStreamMgr::Get() )
    AK::IAkStreamMgr::Get()->Destroy();
  AK::MemoryMgr::Term();

  for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it) {
    delete(it->second);
  }
  m_GameObjects.clear();
  for (size_t i = 0; i < m_events.size(); ++i) {
    delete (m_events[i]);
  }
  m_events.clear();

}


bool CWWSoundManager::Init() {
  //
  // Create and initialize an instance of the default memory manager. Note
  // that you can override the default memory manager with your own. Refer
  // to the SDK documentation for more information.
  //

  AkMemSettings memSettings;
  memSettings.uMaxNumPools = 50;

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

  m_lowLevelIO->SetBasePath(L"./Data/Sounds/");

#ifndef AK_OPTIMIZED
  //
  // Initialize communications (not in release build!)
  //
  AkCommSettings commSettings;
  AK::Comm::GetDefaultInitSettings( commSettings );
  if ( AK::Comm::Init( commSettings ) != AK_Success ) {
    assert( ! "Could not initialize communication." );
    return false;
  }
#endif // AK_OPTIMIZED


  return true;
}

void CWWSoundManager::Render() {
  AK::SoundEngine::RenderAudio();
}

void CWWSoundManager::Load(const std::string &file) {
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
        CXMLTreeNode nodeChild = m.getNextChild();
        std::string name = nodeChild.GetName();
        if (name == "Bank") {
          std::string bnkName = nodeChild.GetPszISOProperty("name", "", false);
          if (AK::SoundEngine::LoadBank(bnkName.c_str(), AK_DEFAULT_POOL_ID, bankID) != AK_Success) {
            LOGGER->AddNewLog(ELL_ERROR, "Bank %s not correctly loaded", bnkName.c_str());
          }
        } else if (name == "GameObject2D") {
          std::string goName = nodeChild.GetPszISOProperty("name", "", false);
          bool register_ = nodeChild.GetBoolProperty("register", false, false);
          GameObjectInfo *go = new GameObjectInfo();
          go->GameObjectID = ++m_LastId;
          go->registered = register_;
          m_GameObjects[goName] = go;
          if (register_) {
            AK::SoundEngine::RegisterGameObj(m_GameObjects[goName]->GameObjectID);
          }
        } else if (name == "GameObject3D") {
          std::string goName = nodeChild.GetPszISOProperty("name", "", false);
          bool register_ = nodeChild.GetBoolProperty("register", false, false);
          GameObjectInfo *go = new GameObjectInfo();
          go->GameObjectID = ++m_LastId;
          go->registered = register_;
          m_GameObjects[goName] = go;
          if (register_) {
            AK::SoundEngine::RegisterGameObj(m_GameObjects[goName]->GameObjectID);
            int positions = nodeChild.GetNumChildren();
            if (positions > 0) {
              AkSoundPosition *posList = new (std::nothrow) AkSoundPosition[positions];
              for (int j = 0; j < positions; ++j) {
                CXMLTreeNode nodeChild1 = nodeChild.getNextChild();
                Vect3f pos = nodeChild1.GetVect3fProperty("pos", v3fZERO, false);
                Vect3f dir = nodeChild1.GetVect3fProperty("dir", v3fZERO, false);
                posList[j].Position.X = pos.x;
                posList[j].Position.Y = pos.y;
                posList[j].Position.Z = pos.z;
                dir.Normalize();
                posList[j].Orientation.X = dir.x;
                posList[j].Orientation.Y = dir.y;
                posList[j].Orientation.Z = dir.z;

              }
              std::string type = nodeChild.GetPszISOProperty("type", "", false);
              AK::SoundEngine::MultiPositionType type_pos;
              if (type == "MultiSource") {
                type_pos = AK::SoundEngine::MultiPositionType::MultiPositionType_MultiSources;
              } else {
                type_pos = AK::SoundEngine::MultiPositionType::MultiPositionType_MultiDirections;
              }
              AK::SoundEngine::SetMultiplePositions(m_GameObjects[goName]->GameObjectID, posList, positions, type_pos);
              delete[] posList;
            } else {
              Vect3f pos = nodeChild.GetVect3fProperty("pos", v3fZERO, false);
              Vect3f dir = nodeChild.GetVect3fProperty("dir", v3fZERO, false);
              AkSoundPosition soundPos;
              soundPos.Position.X = pos.x;
              soundPos.Position.Y = pos.y;
              soundPos.Position.Z = pos.z;
              dir.Normalize();
              soundPos.Orientation.X = dir.x;
              soundPos.Orientation.Y = dir.y;
              soundPos.Orientation.Z = dir.z;
              AK::SoundEngine::SetPosition(m_GameObjects[goName]->GameObjectID, soundPos);
            }
          }
        } else if (name == "InitEvent") {
          std::string eventName = nodeChild.GetPszISOProperty("event", "", false);
          std::string goName = nodeChild.GetPszISOProperty("GameObject", "", false);
          EventInfo *ei = new EventInfo();
          ei->Event = eventName;
          ei->GameObjectID = m_GameObjects[goName]->GameObjectID;
          m_events.push_back(ei);
        }
      }
    }
  }
  // Init all the events listed in the WWSounds.xml
  for (size_t i = 0; i < m_events.size(); ++i) {
    m_events[i]->PlayindID = AK::SoundEngine::PostEvent(m_events[i]->Event.c_str(), m_events[i]->GameObjectID);
  }
}

void CWWSoundManager::SetListenerPosition(Vect3f pos, Vect3f direction, Vect3f up) {
  AkListenerPosition lpos;
  lpos.OrientationFront.X = direction.x;
  lpos.OrientationFront.Y = direction.y;
  lpos.OrientationFront.Z = direction.z;
  lpos.OrientationTop.X = up.x;
  lpos.OrientationTop.Y = up.y;
  lpos.OrientationTop.Z = up.z;
  lpos.Position.X = pos.x;
  lpos.Position.Y = pos.y;
  lpos.Position.Z = pos.z;
  AK::SoundEngine::SetListenerPosition(lpos);
}

void CWWSoundManager::SetGameObjectPosition(const std::string &gameObject, Vect3f pos, Vect3f direction) {
  auto it = m_GameObjects.find(gameObject);
  if (it != m_GameObjects.end() && it->second->registered) {
    AkSoundPosition soundPos;
    direction.Normalize();
    soundPos.Orientation.X = direction.x;
    soundPos.Orientation.Y = direction.y;
    soundPos.Orientation.Z = direction.z;
    soundPos.Position.X = pos.x;
    soundPos.Position.Y = pos.y;
    soundPos.Position.Z = pos.z;
    AK::SoundEngine::SetPosition(it->second->GameObjectID, soundPos);
  }
}

void CWWSoundManager::StopAllPlayingEvents() {
  AK::SoundEngine::StopAll();
}

void CWWSoundManager::PlayEvent(const std::string &eventName, const std::string &GameObject) {
  auto it = m_GameObjects.find(GameObject);
  if (it != m_GameObjects.end() && it->second->registered) {
    AkPlayingID m_iPID = AK::SoundEngine::PostEvent(eventName.c_str(), it->second->GameObjectID);
  }
}

void CWWSoundManager::SetSwitch(const std::string &group, const std::string &switch_, const std::string &gameObject) {
  auto it = m_GameObjects.find(gameObject);
  if (it != m_GameObjects.end() && it->second->registered) {
    AK::SoundEngine::SetSwitch(group.c_str(), switch_.c_str(), it->second->GameObjectID);
  }
}

void CWWSoundManager::SetTrigger(const std::string &trigger, const std::string &gameObject) {
  auto it = m_GameObjects.find(gameObject);
  if (it != m_GameObjects.end() && it->second->registered) {
    AK::SoundEngine::PostTrigger(trigger.c_str(), it->second->GameObjectID);
  }
}

void CWWSoundManager::RegisterGameObject(const std::string &gameObject) {
  auto it = m_GameObjects.find(gameObject);
  if (it != m_GameObjects.end() && !it->second->registered) {
    AK::SoundEngine::RegisterGameObj(it->second->GameObjectID);
    it->second->registered = true;
  }
}

void CWWSoundManager::UnregisterGameObject(const std::string &gameObject) {
  auto it = m_GameObjects.find(gameObject);
  if (it != m_GameObjects.end() && it->second->registered) {
    AK::SoundEngine::UnregisterGameObj(it->second->GameObjectID);
    it->second->registered = false;
  }
}

void CWWSoundManager::SetState(const std::string &group, const std::string &state) {
  AK::SoundEngine::SetState(group.c_str(), state.c_str());
}

void CWWSoundManager::Reload() {
  AK::SoundEngine::UnregisterAllGameObj();
  AK::SoundEngine::ClearBanks();
  Load(m_fileName);
}