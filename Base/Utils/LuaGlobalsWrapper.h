#pragma once
#ifndef LUA_GLOBALS_WRAPPER_H
#define LUA_GLOBALS_WRAPPER_H
#include "Utils\LuaGlobals.h"
#include <string>

class CLuaGlobalsWrapper {
 protected:
  CLuaGlobals *m_LuaWrapperInstance;
 public:
  CLuaGlobalsWrapper() {
    m_LuaWrapperInstance = CLuaGlobals().getInstance();
  }
  //Ojo que este es solo para probar el ScriptManager

  CLuaGlobals *getInstance() {
    return m_LuaWrapperInstance;
  }

};

#endif