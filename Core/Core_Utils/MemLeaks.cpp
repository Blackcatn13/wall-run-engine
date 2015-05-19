#include "MemLeaks.h"

#if defined( _DEBUG )

#define MEMORY_FILE_COUNT 1024
#define MEMORY_FILE_LENGTH 256

static char _vFile[MEMORY_FILE_COUNT][MEMORY_FILE_LENGTH];
static unsigned int _nFile(0);


bool MemLeaks::MemoryBegin() {
  //Si se ponen threads hay que bloquear este acceso con un mute	x
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  //_CrtSetBreakAlloc (31346);
  //_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
  //atexit(FileStringDestroy);
  //atexit(DumpLeaks);
  return true;
}

bool MemLeaks::MemoryEnd() {
  //Si se ponen threads hay que desbloquear aqui un mutex
  return true;
}

const char *MemLeaks::FileString( const char *sFile ) {
  char *pFile(0);
  unsigned int iFile(0);
  while (iFile < _nFile) {
    pFile = _vFile[iFile];
    ++iFile;
    if (strcmp(pFile, sFile) == 0) break;
  }
  if (iFile == _nFile) {
    pFile = _vFile[_nFile];
    //strncpy(pFile,sFile,MEMORY_FILE_LENGTH);
    strncpy_s(pFile, _countof(_vFile[_nFile]), sFile, MEMORY_FILE_LENGTH);
    ++_nFile;
    // Si nos pasamos del numero de ficheros, machacamos el primero
    if (_nFile == MEMORY_FILE_COUNT ) _nFile = 0;
    //wprintf(L"Files using memory leaks detection : %u\n",_nFile);
  }
  return pFile;
}

#endif // defined( _DEBUG )