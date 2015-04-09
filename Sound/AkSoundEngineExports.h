//////////////////////////////////////////////////////////////////////
//
// AkSoundEngineExports.h
//
// Export/import DLL macro.
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////
#ifndef _AK_SOUND_ENGINE_EXPORT_H_
#define _AK_SOUND_ENGINE_EXPORT_H_

#ifdef AKSOUNDENGINEDLL_EXPORTS
	#define AKSOUNDENGINEDLL_API __declspec(dllexport)
#else
	#define AKSOUNDENGINEDLL_API __declspec(dllimport)
#endif

#endif  //_AK_SOUND_ENGINE_EXPORT_H_
