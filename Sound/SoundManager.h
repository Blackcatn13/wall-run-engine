#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#pragma once

#include <vector>
#include <string>
#include <map>

#include <al.h>
#include <stdio.h>

#include "Math\Vector3.h"
#include "Math\LerpAnimator1D.h"
#include "Utils\SingletonPattern.h"

class CSoundManager : public CSingleton<CSoundManager>
{
private:
  typedef struct
  {
    ALuint  m_uSource;
    bool    m_bReserved;
  } tInfoSource;

  typedef std::string     tSoundFile;
  typedef std::string     tAction;
  typedef ALuint          tIdBuffer;

  bool        _initAL();
  void        _finalizeAL();
  bool        _loadSound( const std::string& file, tIdBuffer& buffer );
  void        _clear();
  int         _getSource( bool reserved = false );

  void        Release();
  std::string _getALErrorString( ALenum err );

  std::string m_XmlPath;
  bool        m_bSoundON;
  bool        m_bPause;

  std::map<uint32, Math::CLerpAnimator1D>     m_SourcesFadeInOut;
  std::map<tAction, tIdBuffer>                m_Buffers;
  std::vector<tInfoSource>                    m_Sources;

public:
  CSoundManager();
  ~CSoundManager();

  bool Init( const std::string& l_XmlPath );
  bool Reload();

  //-----------GENERAL FUNCTIONS---------------------
  void    Update( float delatTime );

  void    Reset();
  void    Pause();
  void    Stop();

  void    SoundOn()
  {
    m_bSoundON = true;
  }
  void    SoundOff()
  {
    m_bSoundON = false;
  }

  void    SetGain( float gain );
  float   GetGain();

  //---------ACTION FUNCTIONS-------------------------------
  bool    PlayAction2D( const std::string& action );
  bool    PlayAction3D( const std::string& action, const Math::Vect3f& position );

  //-----SOURCE FUNCTIONS----------------------------
  int   CreateSource();
  bool    DeleteSource( unsigned int source );

  bool    PlaySource2D( unsigned int source, const std::string& action, bool loop );
  bool    PlaySource3D( unsigned int source, const std::string& action, bool loop );

  bool    PauseSource( unsigned int source );
  bool    StopSource( unsigned int source );

  bool    SetSourceGain( unsigned int source, float inGain );
  bool    GetSourceGain( unsigned int source, float& outGain );

  bool    SetSourcePosition( unsigned int source, const Math::Vect3f& inPosition );
  bool    GetSourcePosition( unsigned int source, Math::Vect3f& outPosition );

  bool    SetSourceVelocity( unsigned int source, const Math::Vect3f& inVelocity );
  bool    GetSourceVelocity( unsigned int source, Math::Vect3f& outVelocity );

  //-----LISTENER FUNCTIONS-------------------------
  void    SetListenerPosition( const Math::Vect3f& inPosition );
  void    GetListenerPosition( Math::Vect3f& outPosition );

  void    SetListenerVelocity( const Math::Vect3f& inVelocity );
  void    GetListenerVelocity( Math::Vect3f& outVelocity );

  void    SetListenerOrientation( const Math::Vect3f& inAt, const Math::Vect3f& inUp );
  void    GetListenerOrientation( Math::Vect3f& outAt, Math::Vect3f& outUp );
  void    FadeIn( uint32 Source, float Time );
  void    FadeOut( uint32 _Source, float _Time );

};

#endif // SOUND_MANAGER_H
