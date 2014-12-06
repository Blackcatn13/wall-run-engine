//----------------------------------------------------------------------------------
// Extern functions
// Author: Enric Vergara
//
// Description:
// Llamando a MemoryBegin y MemoryEnd al principio y al final de la ejecución respectivamente, podremos saber
// si nos hemos dejado memory leaks. Además gracias a FileString podremos saber no sólo la posición de memoria
// donde se ha producido el memory leak sino el fichero y linea de código.
//----------------------------------------------------------------------------------
#pragma once

#if defined( _DEBUG )

#ifndef INCLUDE_MEMLEAKS_H
#define INCLUDE_MEMLEAKS_H


#include <list> 
#include <map>
#include <string>

// Activamos la capacidad de ver el file y line en los ficheros que incluyan este

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define new new(_NORMAL_BLOCK, MemLeaks::FileString(__FILE__), __LINE__)

namespace  MemLeaks
{
extern bool MemoryBegin();
extern bool MemoryEnd();
extern const char* FileString( const char* sFile );
}

#endif //INCLUDE_MEMLEAKS_H

#endif // defined( _DEBUG )