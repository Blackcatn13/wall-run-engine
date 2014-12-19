#include "ScriptManager.h"

//Código de la función Alert que se llamará al generarse algún error de LUA
int Alert(/*IN */lua_State * State)
{
	std::string l_Text;
	int n = lua_gettop(State);
	int i;
	lua_getglobal(State, "tostring");
	for (i=1; i<=n; i++) 
	{
		const char *s;
		lua_pushvalue(State, -1);
		lua_pushvalue(State, i);
		lua_call(State, 1, 1);
		s = lua_tostring(State, -1);
		if (s == NULL)
			return luaL_error(State, "`tostring' must return a string to `print'");
		if (i>1) l_Text += '\t';
		l_Text += s;
		lua_pop(State, 1);
	}
	l_Text += '\n';
	//Info( l_Text.c_str() );
	LOGGER->AddNewLog(ELL_INFORMATION,  l_Text.c_str());
	return true;
}

CScriptManager::CScriptManager(void)
{
	//RegisterLUAClass();
}


CScriptManager::~CScriptManager(void)
{
	Destroy();
}

//Para inicializar el motor de LUA
void CScriptManager::Initialize()
{
	m_LS=luaL_newstate();
	
	luaL_openlibs(m_LS);
	//Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar
	//código LUA
	lua_register(m_LS,"_ALERT",Alert);
	luabind::open(m_LS);
}

//Para desinicializar el motor de LUA
void CScriptManager::Destroy()
{
	lua_close(m_LS);
}
//Para ejecutar un fragmento de código LUA
void CScriptManager::RunCode(const std::string &Code) const
{
	if(luaL_dostring(m_LS,Code.c_str()))
	{
	const char *l_Str=lua_tostring(m_LS, -1);
	//Info("%s",l_Str);
	LOGGER->AddNewLog(ELL_INFORMATION, l_Str);
	}
}
//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
	if(luaL_dofile(m_LS, FileName.c_str()))
	{
		const char *l_Str=lua_tostring(m_LS, -1);
		//Info("%s",l_Str);
		LOGGER->AddNewLog(ELL_INFORMATION, l_Str);
	}
}

void CScriptManager::RegisterLUAFunctions()
{
	luabind::module(LUA_STATE) [
	class_<CScriptManager>("CScriptManager")
	.def("load_file", & CScriptManager::LoadFile)
	.def("run_code", & CScriptManager::RunCode)
	.def("run_file", & CScriptManager::RunFile)
	.def("load", & CScriptManager::Load)
	];

	luabind::module(LUA_STATE) [
	class_<CVisible>("CVisible")
	.def("get_visible", & CVisible::getVisible)
	.def("set_visible", & CVisible::setVisible)
	];

	luabind::module(LUA_STATE) [
	class_<CVisible>("CNamed")
	.def("get_name", & CNamed::getName)
	.def("set_name", & CNamed::setName)
	];

	module(LUA_STATE) [
	class_< CRenderableObject >("CRenderableObject")
	//.def(constructor<>())
	.def("update", & CRenderableObject::Update)
	.def("render", & CRenderableObject::Render)
	];

	module(LUA_STATE) [
	class_< CRenderableObjectsManager >("CRenderableObjectsManager")
	.def(constructor<>())
	.def("update", & CRenderableObjectsManager::Update)
	.def("render", & CRenderableObjectsManager::Render)
	.def("add_mesh_instance", (CRenderableObject *(CRenderableObjectsManager::*)(const std::string &, const std::string &, const Vect3f &)) &CRenderableObjectsManager::AddMeshInstance)
	.def("add_mesh_instance", (CRenderableObject *(CRenderableObjectsManager::*)(CXMLTreeNode &Node)) &CRenderableObjectsManager::AddMeshInstance)
	.def("add_animated_instance_model", (CRenderableObject *(CRenderableObjectsManager::*)(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position)) &CRenderableObjectsManager::AddAnimatedInstanceModel)
	.def("add_animated_instance_model", (CRenderableObject *(CRenderableObjectsManager::*)(CXMLTreeNode &Node)) &CRenderableObjectsManager::AddAnimatedInstanceModel)
	.def("load", & CRenderableObjectsManager::Load)
	.def("reload", & CRenderableObjectsManager::Reload)
	
	// CRenderableObject* AddMeshInstance(CXMLTreeNode &Node);
	//.def("add_animated_instance_model",*(CRenderableObjectsManager::*)(const std::string &, const std::string &, const Vect3f &)) & CRenderableObjectsManager::AddAnimatedInstanceModel)
	
	];


	/* void Update(float ElapsedTime);
    void Render(CGraphicsManager *RM);
    CRenderableObject* AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position);
    CRenderableObject* AddMeshInstance(CXMLTreeNode &Node);
    CRenderableObject* AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position);
    CRenderableObject* AddAnimatedInstanceModel(CXMLTreeNode &Node);
    void Load(const std::string &FileName);
    void Reload();*/
}