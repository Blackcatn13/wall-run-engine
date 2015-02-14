#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "Utils\Defines.h"
#include "Utils\Logger.h"
#include "XML\XMLTreeNode.h"

#include <d3dx9.h>
#include <map>
#include <string>

#include "Utils\MapManager.h"
#include "Utils\TemplatedVectorMapManager.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include <luabind/scope.hpp>

using namespace luabind;


class CScriptManager
{
private:
    lua_State *m_LS;
    std::map<std::string, std::string>  m_ScriptsMap;
    bool m_DeleteMap;
    std::string m_FileName;

public:
    CScriptManager();
    ~CScriptManager();
    void Initialize();
    void Destroy();
    void RunCode(const std::string &Code) const;
    void RunFile(const std::string &FileName) const;
    void Load(const std::string &XMLFile);
    void Reload();
    void Reload(std::string File);
    //	void LoadFile(const std::string &XMLFile);
    lua_State * GetLuaState() const
    {
        return m_LS;
    }
    void RegisterLUAFunctions();

    std::map<std::string, std::string>  GetScriptsMap()
    {
        return m_ScriptsMap;
    }

};

#endif
