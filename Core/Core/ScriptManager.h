#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H
#include <string>
#include <vector>

#include "Utils\Defines.h"
#include "Utils\Logger.h"
#include "Core\Core.h"
#include "Utils\Named.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"

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
#include "Utils\Visible.h"

using namespace luabind;

class CScriptManager
{
	private:
		lua_State *m_LS;
		public:
		CScriptManager();
		~CScriptManager();
		void Initialize();
		void Destroy();
		void RunCode(const std::string &Code) const;
		void RunFile(const std::string &FileName) const;
		void Load(const std::string &XMLFile);
		void LoadFile(const std::string &XMLFile);
		lua_State * GetLuaState() const {return m_LS;}
		void RegisterLUAFunctions();

	
};
#endif
