//-----------------------------------------------------------------------
// Base Utils
// Utilidades para la libreria BASE
//-----------------------------------------------------------------------
#ifndef __BASE_UTILS_H__
#define __BASE_UTILS_H__


#include <sstream>
#include <stdio.h>
#include "Windows.h"
#include "Utils\Types.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// namespace baseUtils 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace baseUtils 
{
	inline void	FormatSrting	( std::string& output, const char* format, ... )
	{
		va_list args;
		char* buffer;
		va_start(args,format);
		int len = _vscprintf(format, args) + 1;
		buffer = (char*)malloc(len*sizeof(char));
		vsprintf_s( buffer, len, format, args );
		output = buffer;
		delete buffer;
	}

	inline void GetDate (uint32& day, uint32& month, uint32& year, uint32& hour, uint32& minute, uint32& second)
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		day			=	st.wDay;
		month		= st.wMonth;
		year		= st.wYear;
		hour		= st.wHour;
		minute	= st.wMinute;
		second	= st.wSecond;
	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} //namespace baseUtils 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__BASE_UTILS_H__
