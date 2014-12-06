#include "FontManager.h"
#include "Core.h"
#include "LanguageManager.h"
#include "Utils/Logger.h"
#include "Exception.h"
#include "GraphicsManager.h"
#include "xml/XMLTreeNode.h"
//#include "Script/ScriptManager.h"
//#include "luabind/luabind.hpp"
#include <assert.h>


void CFontManager::Done ()
{
    if (IsOk()) {
        Release();
        m_bIsOk = false;
    }
}

bool CFontManager::Init(CGraphicsManager* gm)
{
    LOGGER->AddNewLog(ELL_INFORMATION, "CFontManager:: Inicializando FontManager");
    m_pD3DDevice = gm->GetDevice();
    m_bIsOk = (m_pD3DDevice != NULL);
    if (m_bIsOk) {
        //Como mínimo creamos una fuente por defecto, la que estara en la posición m_Fonts[0]
        CreateFont(17, true, false, "Times New Roman");
    }
    return m_bIsOk;
}
void CFontManager::Release()
{
    std::vector<LPD3DXFONT>::iterator it = m_Fonts.begin();
    std::vector<LPD3DXFONT>::iterator itEnd = m_Fonts.end();
    while (it != itEnd) {
        LPD3DXFONT font = *it;
        CHECKED_RELEASE(font);
        ++it;
    }
    m_Fonts.clear();
    std::vector<std::string>::iterator itTTF = m_vTTFsFiles.begin();
    std::vector<std::string>::iterator itTTFEnd = m_vTTFsFiles.end();
    for (; itTTF != itTTFEnd; ++itTTF) {
        RemoveFontResource(itTTF->c_str());
    }
    m_vTTFsFiles.clear();
}

bool CFontManager::ReloadTTFs ()
{
    if (m_sPathFile != "") {
        return LoadTTFs (m_sPathFile);
    }
    return false;
}

bool CFontManager::LoadTTFs (const std::string& pathFile)
{
    CXMLTreeNode parser;
    if (!parser.LoadFile(pathFile.c_str())) {
        std::string msg_error = "CRenderManager::LoadFonts->Error al intentar leer el archivo de configuracion: " + pathFile;
        LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
        throw CException(__FILE__, __LINE__, msg_error);
    }
    m_sPathFile = pathFile;
    CXMLTreeNode  m = parser["Fonts"];
    if (m.Exists()) {
        //first of all we release the fonts
        Release();
        //Como mínimo creamos una fuente por defecto, la que estara en la posición m_Fonts[0]
        CreateFont(17, true, false, "Times New Roman");
        int count = m.GetNumChildren();
        for (int i = 0; i < count; ++i) {
            std::string fontId	= m(i).GetPszProperty("id");
            std::string name		= m(i).GetPszProperty("name");
            std::string file		= m(i).GetPszProperty("file");
            uint8 size					= m(i).GetIntProperty("size", 10);
            bool bold						= m(i).GetBoolProperty("bold", false);
            bool italica				= m(i).GetBoolProperty("italica", false);
            bool _default				= m(i).GetBoolProperty("default", false);
            std::vector<std::string>::iterator it			= m_vTTFsFiles.begin();
            std::vector<std::string>::iterator itEnd	= m_vTTFsFiles.end();
            bool exist = false;
            while (it != itEnd) {
                std::string ttfFile = *it;
                if (ttfFile == file) {
                    exist = true;
                }
                ++it;
            }
            if (!exist) {
                int id = AddFontResource(file.c_str());
                if (id == 1) {
                    m_vTTFsFiles.push_back(file);
                    m_TTFs[fontId] = CreateFont(size, bold, italica, name, _default);
                    LOGGER->AddNewLog(ELL_INFORMATION, "LoadFonts:: Add font %s (file:%s,size:%d,bold:%d,italica:%d,default:%d),",fontId.c_str(), file.c_str(), size, bold, italica, _default);
                } else {
                    LOGGER->AddNewLog(ELL_ERROR, "LoadFonts:: no se ha podido añadir el ttf file: %s", file.c_str());
                    //CORE->SetAssetError(ASSET_ERROR_TTF);
                }
            }
        }
    }
    return true;
}

int32 CFontManager::GetTTF_Id (const std::string& name)
{
    std::map<std::string, uint32>::iterator it;
    it = m_TTFs.find(name);
    if (it != m_TTFs.end()) {
        return it->second;
    }
    return -1;
}

uint32 CFontManager::CreateFont (uint32 size, bool bold, bool italica, const std::string& fontName, bool replaceDefault)
{
    LPD3DXFONT font;
    uint32 weight;
    if (bold)
        weight = FW_BOLD;
    else
        weight = FW_NORMAL;
    D3DXCreateFont(	m_pD3DDevice, size, 0, weight, 1, italica, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                    fontName.c_str(),	&font);
    uint32 id = 0;
    if (replaceDefault) {
        if (m_Fonts.size() > 0) {
            CHECKED_RELEASE(m_Fonts[0]);
            m_Fonts[0] = font;
        } else {
            m_Fonts.push_back(font);
        }
        id = 0;
    } else {
        m_Fonts.push_back(font);
        id = (uint32)m_Fonts.size() - 1;
    }
    return id;
}

uint32 CFontManager::DrawDefaultText(uint32 x, uint32 y, CColor color, const char * format, ... )
{
    va_list args;
    char* buffer;
    va_start(args, format);
    int len = _vscprintf(format, args) + 1;
    buffer = (char*)malloc(len * sizeof(char));
    vsprintf_s( buffer, len, format, args );
    RECT rect;
    rect.left = x;
    rect.top = y;
    DWORD color_aux = color.GetUint32Argb();
    // Calcular el tamaño del rectangulo necesario para pintar ese texto
    assert(m_Fonts.size() > 0);
    m_Fonts[0]->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, color_aux );
    // pintarlo
    m_Fonts[0]->DrawText( NULL, buffer, -1, &rect, DT_LEFT, color_aux );
    free( buffer );
    return (rect.bottom  - rect.top);
}

Vect2i CFontManager::GetDefaultTextSize(uint32 x, uint32 y, CColor color, const char * format, ...)
{
    va_list args;
    char* buffer;
    va_start(args, format);
    int len = _vscprintf(format, args) + 1;
    buffer = (char*)malloc(len * sizeof(char));
    vsprintf_s( buffer, len, format, args );
    RECT rect;
    rect.left = x;
    rect.top = y;
    DWORD color_aux = color.GetUint32Argb();
    assert(m_Fonts.size() > 0);
    m_Fonts[0]->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, color_aux );
	free(buffer);
    return Vect2i(rect.right - rect.left, rect.bottom - rect.top);
}

uint32 CFontManager::DrawText(uint32 x, uint32 y, CColor color, uint32 idFont, const char * format, ... )
{
    va_list args;
    char* buffer;
    va_start(args, format);
    int len = _vscprintf(format, args) + 1;
    buffer = (char*)malloc(len * sizeof(char));
    vsprintf_s( buffer, len, format, args );
    RECT rect;
    rect.left = x;
    rect.top = y;
    DWORD color_aux = color.GetUint32Argb();
    // Calcular el tamaño del rectangulo necesario para pintar ese texto
    assert(m_Fonts.size() > idFont );
    m_Fonts[idFont]->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, color_aux );
    // pintarlo
    m_Fonts[idFont]->DrawText( NULL, buffer, -1, &rect, DT_LEFT, color_aux );
    free( buffer );
    return (rect.bottom  - rect.top);
}

Vect2i CFontManager::GetTextSize(uint32 x, uint32 y, CColor color, uint32 idFont, const char * format, ... )
{
    va_list args;
    char* buffer;
    va_start(args, format);
    int len = _vscprintf(format, args) + 1;
    buffer = (char*)malloc(len * sizeof(char));
    vsprintf_s( buffer, len, format, args );
    RECT rect;
    rect.left = x;
    rect.top = y;
    DWORD color_aux = color.GetUint32Argb();
    // Calcular el tamaño del rectangulo necesario para pintar ese texto
    assert(m_Fonts.size() > idFont );
    m_Fonts[idFont]->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, color_aux );
	free(buffer);
    return Vect2i(rect.right - rect.left, rect.bottom - rect.top);
}

uint32 CFontManager::DrawLiteral (uint32 x, uint32 y, const std::string & s_lit)
{
    CLanguageManager* languageM = CCore::GetInstance()->GetLanguageManager();
    SLiteral lit;
    if (languageM->GetLiteral(s_lit, lit)) {
        int32 ttfId = GetTTF_Id(lit.m_sFontId);
        if (ttfId >= 0) {
            return DrawText(x, y, lit.m_cColor, ttfId, lit.m_value.c_str());
        } else {
            return DrawDefaultText(x, y, colGREEN, "Font Error: %s", lit.m_sFontId.c_str());
        }
    }
    return DrawDefaultText(x, y, colGREEN, "Literal Error: %s", s_lit.c_str());
}

Vect2i CFontManager::GetLiteralSize (uint32 x, uint32 y, const std::string & s_lit)
{
    CLanguageManager* languageM = CCore::GetInstance()->GetLanguageManager();
    SLiteral lit;
    if (languageM->GetLiteral(s_lit, lit)) {
        int32 ttfId = GetTTF_Id(lit.m_sFontId);
        if (ttfId >= 0) {
            return GetTextSize(x, y, lit.m_cColor, ttfId, lit.m_value.c_str());
        } else {
            return GetDefaultTextSize(x, y, colGREEN, "Font Error: %s", lit.m_sFontId.c_str());
        }
    }
    return GetDefaultTextSize(x, y, colGREEN, "Literal Error: %s", s_lit.c_str());
}


uint32 CFontManager::SizeX (const char * format, uint32 idFont)
{
    va_list args;
    char* buffer;
    va_start(args, format);
    int len = _vscprintf(format, args) + 1;
    buffer = (char*)malloc(len * sizeof(char));
    vsprintf_s( buffer, len, format, args );
    RECT rect;
    // Calcular el tamaño del rectangulo necesario para pintar ese texto
    assert(m_Fonts.size() > idFont );
    m_Fonts[idFont]->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, 0xff000000 );
    free( buffer );
    return (rect.right  - rect.left);
}

uint32 CFontManager::SizeY (const char * format, uint32 idFont)
{
    va_list args;
    char* buffer;
    va_start(args, format);
    int len = _vscprintf(format, args) + 1;
    buffer = (char*)malloc(len * sizeof(char));
    vsprintf_s( buffer, len, format, args );
    RECT rect;
    // Calcular el tamaño del rectangulo necesario para pintar ese texto
    assert(m_Fonts.size() > idFont );
    m_Fonts[idFont]->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, 0xff000000 );
    free( buffer );
    return (rect.bottom  - rect.top);
}



/*void CFontManager::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();

	using namespace luabind;

	// ahora registramos lo que querramos
  module(l_pLUAState)
    [
      // registramos la clase CRenderManager
			class_<CFontManager>("CFontManager")
      // registramos su constructor
      .def(constructor<>())

			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			//--- Font---------------------------------
			.def(	CScriptRegister::PushFunctionName("createFont" "uint32", "uint32 size, bool bold, bool italica, const std::string& fontName",
						"Crea una fuente"),
						&CFontManager::CreateFont)

			.def(	CScriptRegister::PushFunctionName("reloadTTFs" "bool", "",
						"Reload del fichero que contiene la informacion de los ttf"),
						&CFontManager::ReloadTTFs)

			//.def(CScriptRegister::PushFunctionName("DrawDefaultText"),	&CFontManager::DrawDefaultText)
			//.def(CScriptRegister::PushFunctionName("DrawText"),					&CFontManager::DrawText)

			.def(	CScriptRegister::PushFunctionName("sizeX", "uint32", "const char * format, uint32 idFont",
						"Devuelve el ancho en pixeles de un texto introducido"),
						&CFontManager::SizeX)

			.def(	CScriptRegister::PushFunctionName("sizeY", "uint32", "const char * format, uint32 idFont",
						"Devuelve la altura en pixeles de un texto introducido"),
						&CFontManager::SizeY)

    ];
}*/
