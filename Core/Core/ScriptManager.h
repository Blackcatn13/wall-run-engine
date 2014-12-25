#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H
#include <string>
#include <vector>

#include "Utils\Defines.h"
#include "Utils\Logger.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Utils\Named.h"
#include "Renderable\RenderableObject.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Object\Object3D.h"
#include "Utils\MapManager.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "Math\Vector3.h"
#include "Math\Matrix44.h"
#include "Math\Matrix33.h"
#include "Math\Matrix34.h"
#include <d3dx9.h>

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
#include <map>
#include <string>

using namespace luabind;

class CScriptManager
{
	private:
		lua_State *m_LS;
		std::map<std::string,std::string>  m_ScriptsMap;
		bool m_DeleteMap;

	public:
		CScriptManager();
		~CScriptManager();
		void Initialize();
		void Destroy();
		void RunCode(const std::string &Code) const;
		void RunFile(const std::string &FileName) const;
		void Load(const std::string &XMLFile);
	//	void LoadFile(const std::string &XMLFile);
		lua_State * GetLuaState() const {return m_LS;}
		void RegisterLUAFunctions();
		
		std::map<std::string, std::string>  GetScriptsMap(){return m_ScriptsMap;}
	
};
#endif
