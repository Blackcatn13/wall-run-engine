#include "LanguageManager.h"
#include "xml\XMLTreeNode.h"
//#include "Exceptions/Exception.h"
#include "Utils\Logger.h"
#include "Utils\Exception.h"

CLanguageManager::CLanguageManager()
  : m_sCurrentLanguage("")
{}


void CLanguageManager::LoadXMLs () {
  //Tener en cuenta que se puede hacer un reload!
  std::map<std::string, TLanguage>::iterator it_1			= m_Languages.begin();
  std::map<std::string, TLanguage>::iterator itEnd_1	= m_Languages.end();
  while (it_1 != itEnd_1) {
    it_1->second.clear();
    ++it_1;
  }
  m_Languages.clear();
  std::vector<std::string>::const_iterator it			= m_vXML_Files.begin();
  std::vector<std::string>::const_iterator itEnd	= m_vXML_Files.end();
  while (it != itEnd) {
    std::string xmlPath = *it;
    LoadXML(xmlPath);
    ++it;
  }
}

void CLanguageManager::LoadXML (const std::string &pathFile) {
  CXMLTreeNode parser;
  if (!parser.LoadFile(pathFile.c_str())) {
    std::string msg_error = "LanguageManager::LoadXML->Error al intentar leer el archivo de lenguaje: " + pathFile;
    LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    throw CException(__FILE__, __LINE__, msg_error);
  }
  /*<Language id="english">
  	<literal id="xfiles"  font="X-Files"  color="0.5 0.5 0.5 0.5" value="Hi World"/>
  	<literal id="xfiles"  font="X-Files"  color="0.1 0.1 0.1 0.8" value="Exit"/>
  </Language>*/
  LOGGER->AddNewLog(ELL_INFORMATION, "LanguageManager::LoadXML-> Parseando fichero de lenguaje: %s", pathFile.c_str());
  CXMLTreeNode  m = parser["Language"];
  std::string id_language	= m.GetPszProperty("id");
  TLanguage language;
  if (m.Exists()) {
    int count = m.GetNumChildren();
    for (int i = 0; i < count; ++i) {
      CXMLTreeNode nodeChild = m.getNextChild();
      //for each literal:
      SLiteral l_literal;
      std::string id			= nodeChild.GetPszProperty("id");
      l_literal.m_sFontId	= nodeChild.GetPszProperty("font");
      Vect4f vecColor			= nodeChild.GetVect4fProperty("color", Vect4f(0.f, 0.f, 0.f, 0.f));
      l_literal.m_value		= nodeChild.GetPszISOProperty("value", "nothing");
      l_literal.m_cColor	= CColor(vecColor.x, vecColor.y, vecColor.z, vecColor.w);
      language.insert(std::pair<std::string, SLiteral>(id, l_literal));
      LOGGER->AddNewLog(ELL_INFORMATION, "LanguageManager::LoadXML-> Añadido literal(%s,%s,[%f,%f,%f,%f],%s)",
                        id.c_str(), l_literal.m_sFontId.c_str(), vecColor.x, vecColor.y, vecColor.z, vecColor.w, l_literal.m_value.c_str());
    }
  }
  if (m_Languages.find(id_language) != m_Languages.end()) {
    //Ya está registrado el identificador id_language
    LOGGER->AddNewLog(ELL_WARNING, "LanguageManager::LoadXML-> EYa se ha registrado un language con identificador %s", id_language.c_str());
  } else {
    m_Languages.insert(std::pair<std::string, TLanguage>(id_language, language));
  }
}

void CLanguageManager::SetXmlFile (const std::string &pathFile) {
  std::vector<std::string>::iterator it			= m_vXML_Files.begin();
  std::vector<std::string>::iterator itEnd	= m_vXML_Files.end();
  bool exist = false;
  while (it != itEnd) {
    std::string xmlFile = *it;
    if (xmlFile == pathFile) {
      exist = true;
      break;
    }
    ++it;
  }
  if (!exist) {
    m_vXML_Files.push_back(pathFile);
    LOGGER->AddNewLog(ELL_INFORMATION, "LanguageManager::SetXmlFile-> Se ha añadido el xml: %s", pathFile.c_str());
  }
}

bool CLanguageManager::GetLiteral (const std::string &id, SLiteral &sliteral) const {
  std::map<std::string, TLanguage>::const_iterator it = m_Languages.find(m_sCurrentLanguage);
  if (it != m_Languages.end()) {
    TLanguage currentLanguage = it->second;
    TLanguage::const_iterator it_aux = currentLanguage.find(id);
    if (it_aux != currentLanguage.end()) {
      sliteral = it_aux->second;
      return true;
    }
  }
  return false;
}

void CLanguageManager::SetCurrentLanguage (const std::string &id) {
  std::map<std::string, TLanguage>::iterator it = m_Languages.find(id);
  if (it != m_Languages.end()) {
    m_sCurrentLanguage = id;
    LOGGER->AddNewLog(ELL_INFORMATION, "LanguageManager::SetCurrentLanguage-> Se ha seteado como current el language %s", id.c_str());
  } else {
    LOGGER->AddNewLog(ELL_WARNING, "LanguageManager::SetCurrentLanguage-> El language %s no esta registrado", id.c_str());
  }
}
