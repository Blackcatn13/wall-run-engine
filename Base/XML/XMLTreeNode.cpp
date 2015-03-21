#include "XMLTreeNode.h"
#include "Assert.h"
#include "Utils/Logger.h"

// Defines
#define MY_ENCODING "ISO-8859-1"


//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CXMLTreeNode::Done () {
  if (IsOk()) {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CXMLTreeNode::Release () {
  if (m_pDoc) {
    xmlFreeDoc(m_pDoc);
    m_pDoc = NULL;
  }
  m_pNode = NULL;
  if (m_pWriter) {
    xmlFreeTextWriter(m_pWriter);
    m_pWriter = NULL;
  }
  xmlCleanupParser();
}

//----------------------------------------------------------------------------
// Load File
//----------------------------------------------------------------------------
bool CXMLTreeNode::LoadFile (const char *_pszFileName) {
  m_bIsOk = false;
  assert(_pszFileName);
  if (_pszFileName) {
    m_pDoc = xmlParseFile(_pszFileName);
    if (m_pDoc) {
      m_pNode = xmlDocGetRootElement(m_pDoc);
      if (m_pNode) {
        m_bIsOk = true;
        return true;
      }
    }
  }
  Release();
  return false;
}

//----------------------------------------------------------------------------
// Returns a subtree node from a given key
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::GetSubTree (const char *_pszKey) const {
  assert(m_pNode && _pszKey);
  CXMLTreeNode NewTree;
  if (_pszKey && m_pNode) {
    _FindSubTree(m_pNode, _pszKey, NewTree);
  }
  return NewTree;
}

//----------------------------------------------------------------------------
// Recursive to find a key on a tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::_FindSubTree(xmlNodePtr _pNode, const char *_pszKey, CXMLTreeNode &_TreeFound) const {
  while (_pNode != NULL) {
    if (xmlStrcmp(_pNode->name, (const xmlChar *)_pszKey)) {
      if (_FindSubTree(_pNode->xmlChildrenNode, _pszKey, _TreeFound)) {
        return true;
      }
    } else {
      _TreeFound.m_pNode = _pNode;
      _TreeFound.m_pDoc = m_pDoc;
      return true;
    }
    _pNode = _pNode->next;
  }
  return false;
}

//----------------------------------------------------------------------------
// Returns the main node keyword
//----------------------------------------------------------------------------
const char *CXMLTreeNode::GetName () {
  if (m_pNode) {
    return (char *)m_pNode->name;
  }
  return NULL;
}

//----------------------------------------------------------------------------
// Operator that returns a tree node
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::operator[] (const char *_pszKey) const {
  assert(_pszKey && m_pNode);
  CXMLTreeNode TreeFound;
  if (_pszKey && m_pNode) {
    TreeFound = GetSubTree(_pszKey);
  }
  return TreeFound;
}

//----------------------------------------------------------------------------
// Operator to get children nodes
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::operator() (int _iIndex) const {
  assert(_iIndex >= 0 && m_pNode);
  CXMLTreeNode TreeFound;
  if (_iIndex >= 0 && m_pNode) {
    int iCount = 0;
    xmlNodePtr pChildren = m_pNode->children;
    while (pChildren != NULL) {
      if (pChildren->type != XML_TEXT_NODE) {
        if (_iIndex == iCount) {
          TreeFound.m_pNode = pChildren;
          TreeFound.m_pDoc = m_pDoc;
          break;
        }
        ++iCount;
      }
      pChildren = pChildren->next;
    }
  }
  return TreeFound;
}

//----------------------------------------------------------------------------
// Returns the number of children a tree has
//----------------------------------------------------------------------------
int CXMLTreeNode::GetNumChildren () {
  assert(m_pNode);
  int iCount = 0;
  if (m_pNode) {
    xmlNodePtr pChildren = m_pNode->children;
    while (pChildren != NULL) {
      if (pChildren->type != XML_TEXT_NODE)
        ++iCount;
      pChildren = pChildren->next;
    }
  }
  return iCount;
}

//----------------------------------------------------------------------------
// Returns a param of the tree from a given key
//----------------------------------------------------------------------------
xmlChar *CXMLTreeNode::GetProperty (const char *_pszKey) const {
  assert(_pszKey && m_pNode);
  xmlChar *value = NULL;
  if (_pszKey && m_pNode) {
    value = xmlGetProp(m_pNode, (const xmlChar *)_pszKey);
  }
  return value;
}


//----------------------------------------------------------------------------
// Returns an integer param if found. Else a default value
//----------------------------------------------------------------------------
int CXMLTreeNode::GetIntProperty (const char *_pszKey, int _iDefault, bool warningDefault) const {
  int iRet = _iDefault;
  xmlChar *value = GetProperty(_pszKey);
  if (value) {
    iRet = atoi((const char *)value);
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetIntProperty se ha utilizado el valor por defecto:%d para el tag <%s>", _iDefault, _pszKey);
  }
  xmlFree(value);
  return iRet;
}

//----------------------------------------------------------------------------
// Returns a float param if found. Else a default value
//----------------------------------------------------------------------------
float CXMLTreeNode::GetFloatProperty (const char *_pszKey, float _fDefault, bool warningDefault) const {
  float fRet = _fDefault;
  xmlChar *value = GetProperty(_pszKey);
  if (value) {
    fRet = static_cast<float>(atof((const char *)value));
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetFloatProperty se ha utilizado el valor por defecto:%f para el tag <%s>", _fDefault, _pszKey);
  }
  xmlFree(value);
  return fRet;
}
//----------------------------------------------------------------------------
// Returns a boolean param if found. Else a default value
//----------------------------------------------------------------------------

bool CXMLTreeNode::GetBoolProperty (const char *_pszKey, bool _bDefault, bool warningDefault) const {
  bool bRet = _bDefault;
  xmlChar *value = GetProperty(_pszKey);
  if (value) {
    const char *pszValue = (const char *)value;
    if (strcmp("TRUE", pszValue) == 0 || strcmp("true", pszValue) == 0 || strcmp("True", pszValue) == 0) {
      bRet = true;
    } else
      bRet = false;
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetBoolProperty se ha utilizado el valor por defecto:%d para el tag <%s>", _bDefault, _pszKey);
  }
  xmlFree(value);
  return bRet;
}

//----------------------------------------------------------------------------
// Returns an string param if found. Else a default value
//----------------------------------------------------------------------------
const char *CXMLTreeNode::GetPszProperty (const char *_pszKey, const char *_pszDefault, bool warningDefault) const {
  const char *pszRet = _pszDefault;
  xmlChar *value = GetProperty(_pszKey);
  if (value) {
    pszRet = (const char *)value;
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetPszProperty se ha utilizado el valor por defecto:%s para el tag <%s>", _pszDefault, _pszKey);
  }
  return pszRet;
}

//----------------------------------------------------------------------------
// Returns an string param if found. Else a default value
//----------------------------------------------------------------------------
std::string CXMLTreeNode::GetPszISOProperty (const char *_pszKey, const char *_pszDefault, bool warningDefault) const {
  std::string szRet = (char *)_pszDefault;
  xmlChar *value = GetProperty(_pszKey);
  if (value) {
    unsigned char *in = (unsigned char *)value;
    int inlen = (int)strlen((const char *)in);
    int outlen = inlen * 4;
    unsigned char *out = (unsigned char *)calloc(1, outlen);
    UTF8Toisolat1(out, &outlen, in, &inlen);
    szRet = (char *)out;
    free(out);
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetPszISOProperty se ha utilizado el valor por defecto:%s para el tag <%s>", _pszDefault, _pszKey);
  }
  return szRet;
}

//----------------------------------------------------------------------------
// Returns an Vect2f param if found. Else a default value
//----------------------------------------------------------------------------
Vect2f CXMLTreeNode::GetVect2fProperty  (const char *_pszKey, const Vect2f &_Default, bool warningDefault) const {
  xmlChar *value = GetProperty(_pszKey);
  Vect2f l_V2f(0.0f, 0.0f);
  if (value) {
    const char *pszValue = (const char *)value;
    sscanf_s(pszValue, "%f %f", &l_V2f.x, &l_V2f.y);
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetVect2fProperty se ha utilizado el vector2f por defecto:(%f,%f) para el tag <%s>", _Default.x, _Default.y, _pszKey);
  }
  xmlFree(value);
  return l_V2f;
}

//----------------------------------------------------------------------------
// Returns an Vect3f param if found. Else a default value
//----------------------------------------------------------------------------
Vect3f CXMLTreeNode::GetVect3fProperty  (const char *_pszKey, const Vect3f &_Default, bool warningDefault) const {
  xmlChar *value = GetProperty(_pszKey);
  Vect3f l_V3f(0.0f, 0.0f, 0.0f);
  if (value) {
    const char *pszValue = (const char *)value;
    sscanf_s(pszValue, "%f %f %f", &l_V3f.x, &l_V3f.y, &l_V3f.z);
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetVect3fProperty se ha utilizado el vector3f por defecto:(%f,%f,%f) para el tag <%s>", _Default.x, _Default.y, _Default.z, _pszKey);
  }
  xmlFree(value);
  return l_V3f;
}

//----------------------------------------------------------------------------
// Returns an Vect4f param if found. Else a default value
//----------------------------------------------------------------------------
Vect4f CXMLTreeNode::GetVect4fProperty  (const char *_pszKey, const Vect4f &_Default, bool warningDefault) const {
  xmlChar *value = GetProperty(_pszKey);
  Vect4f l_V4f(0.0f, 0.0f, 0.0f, 0.0f);
  if (value) {
    const char *pszValue = (const char *)value;
    sscanf_s(pszValue, "%f %f %f %f", &l_V4f.x, &l_V4f.y, &l_V4f.z, &l_V4f.w);
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetVect3fProperty se ha utilizado el vector4f por defecto:(%f,%f,%f,%f) para el tag <%s>", _Default.x, _Default.y, _Default.z, _Default.w, _pszKey);
  }
  xmlFree(value);
  return l_V4f;
}

//----------------------------------------------------------------------------
// Returns an CColor param if found. Else a default value
//----------------------------------------------------------------------------
CColor CXMLTreeNode::GetCColorProperty  (const char *_pszKey, const CColor &_Default, bool warningDefault) const {
  xmlChar *value = GetProperty(_pszKey);
  Vect4f l_V4f(0.0f, 0.0f, 0.0f, 0.0f);
  if (value) {
    const char *pszValue = (const char *)value;
    sscanf_s(pszValue, "%f %f %f %f", &l_V4f.x, &l_V4f.y, &l_V4f.z, &l_V4f.w);
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetVect3fProperty se ha utilizado el vector4f por defecto:(%f,%f,%f,%f) para el tag <%s>", _Default.GetRed(), _Default.GetGreen(), _Default.GetBlue(), _Default.GetAlpha(), _pszKey);
  }
  xmlFree(value);
  return CColor(l_V4f);
}

//----------------------------------------------------------------------------
// Returns an Vect2i param if found. Else a default value
//----------------------------------------------------------------------------
Vect2i CXMLTreeNode::GetVect2iProperty  (const char *_pszKey, const Vect2i &_Default, bool warningDefault) const {
  xmlChar *value = GetProperty(_pszKey);
  Vect2i l_V2i(0, 0);
  if (value) {
    const char *pszValue = (const char *)value;
    sscanf_s(pszValue, "%d %d", &l_V2i.x, &l_V2i.y);
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetVect3fProperty se ha utilizado el vector2i por defecto:(%d,%d) para el tag <%s>", _Default.x, _Default.y, _pszKey);
  }
  xmlFree(value);
  return l_V2i;
}

//----------------------------------------------------------------------------
// Returns an Vect3i param if found. Else a default value
//----------------------------------------------------------------------------
Vect3i CXMLTreeNode::GetVect3iProperty  (const char *_pszKey, const Vect3i &_Default, bool warningDefault) const {
  xmlChar *value = GetProperty(_pszKey);
  Vect3i l_V3i(0, 0, 0);
  if (value) {
    const char *pszValue = (const char *)value;
    sscanf_s(pszValue, "%d %d %d", &l_V3i.x, &l_V3i.y, &l_V3i.z);
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetVect3fProperty se ha utilizado el vector3i por defecto:(%d,%d,%d) para el tag <%s>", _Default.x, _Default.y, _Default.z, _pszKey);
  }
  xmlFree(value);
  return l_V3i;
}

//----------------------------------------------------------------------------
// Returns an Vect4i param if found. Else a default value
//----------------------------------------------------------------------------
Vect4i CXMLTreeNode::GetVect4iProperty  (const char *_pszKey, const Vect4i &_Default, bool warningDefault) const {
  xmlChar *value = GetProperty(_pszKey);
  Vect4i l_V4i(0, 0, 0, 0);
  if (value) {
    const char *pszValue = (const char *)value;
    sscanf_s(pszValue, "%d %d %d %d", &l_V4i.x, &l_V4i.y, &l_V4i.z, &l_V4i.w);
  } else if (warningDefault) {
    LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetVect3fProperty se ha utilizado el vector4i por defecto:(%d,%d,%d,%d) para el tag <%s>", _Default.x, _Default.y, _Default.z, _Default.w, _pszKey);
  }
  xmlFree(value);
  return l_V4i;
}


//----------------------------------------------------------------------------
// Returns an keyword from the tree from a given key
//----------------------------------------------------------------------------
xmlChar *CXMLTreeNode::GetKeyword (const char *_pszKey) const {
  assert(_pszKey && m_pNode && m_pDoc);
  xmlChar *value = NULL;
  if (_pszKey && m_pNode && m_pDoc) {
    CXMLTreeNode FoundTree;
    if (_FindSubTree(m_pNode, _pszKey, FoundTree))
      value = xmlNodeListGetString(FoundTree.m_pDoc, FoundTree.m_pNode->xmlChildrenNode, 1);
  }
  return value;
}

//----------------------------------------------------------------------------
// Returns an integer keyword if found. Else a default value
//----------------------------------------------------------------------------
int CXMLTreeNode::GetIntKeyword (const char *_pszKey, int _iDefault/*=0*/) const {
  int iRet = _iDefault;
  xmlChar *value = GetKeyword(_pszKey);
  if (value) {
    iRet = atoi((const char *)value);
  }
  return iRet;
}

//----------------------------------------------------------------------------
// Returns a float keyword if found. Else a default value
//----------------------------------------------------------------------------
float CXMLTreeNode::GetFloatKeyword (const char *_pszKey, float _fDefault/*=0.0*/) const {
  float fRet = _fDefault;
  xmlChar *value = GetKeyword(_pszKey);
  if (value) {
    fRet = static_cast<float>(atof((const char *)value));
  }
  return fRet;
}

//----------------------------------------------------------------------------
// Returns a boolean keyword if found. Else a default value
//----------------------------------------------------------------------------
bool CXMLTreeNode::GetBoolKeyword (const char *_pszKey, bool _bDefault/*=false*/) const {
  bool bRet = _bDefault;
  xmlChar *value = GetKeyword(_pszKey);
  if (value) {
    const char *pszValue = (const char *)value;
    if (strcmp("TRUE", pszValue) == 0 || strcmp("true", pszValue) == 0 || strcmp("True", pszValue) == 0) {
      bRet = true;
    } else
      bRet = false;
  }
  return bRet;
}

//----------------------------------------------------------------------------
// Returns a string keyword if found. Else a default value
//----------------------------------------------------------------------------
const char *CXMLTreeNode::GetPszKeyword (const char *_pszKey, const char *_pszDefault/*=NULL*/) const {
  const char *pszRet = _pszDefault;
  xmlChar *value = GetKeyword(_pszKey);
  if (value) {
    pszRet = (const char *)value;
  }
  return pszRet;
}

//----------------------------------------------------------------------------
// Checks if a key is on the tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::ExistsKey (const char *_pszKey) {
  assert(_pszKey);
  CXMLTreeNode TreeFound = GetSubTree(_pszKey);
  return TreeFound.Exists();
}

//----------------------------------------------------------------------------
// Starts a new file and prepares it to be written
//----------------------------------------------------------------------------
bool CXMLTreeNode::StartNewFile(const char *_pszFileName) {
  assert(_pszFileName);
  m_bIsOk = false;
  if (_pszFileName) {
    m_pszFileName = _pszFileName;
    // Create a new XmlWriter for DOM, with no compression.
    m_pWriter = xmlNewTextWriterDoc(&m_pDoc, 0);
    assert(m_pWriter);
    if (m_pWriter) {
      // Start the document with the xml default for the version, encoding ISO 8858-1 and the default for the standalone declaration.
      int rc = xmlTextWriterStartDocument(m_pWriter, NULL, MY_ENCODING, NULL);
      assert(rc >= 0);
      if (rc >= 0) {
        m_bIsOk = true;
        return true;
      }
    }
  }
  Release();
  return false;
}


//----------------------------------------------------------------------------
// Finished a file and saves it
//----------------------------------------------------------------------------
void CXMLTreeNode::EndNewFile () {
  assert(m_pWriter && m_pDoc && m_pszFileName);
  if (m_pWriter && m_pDoc && m_pszFileName) {
    xmlFreeTextWriter(m_pWriter);
    m_pWriter = NULL;
    xmlSaveFileEnc(m_pszFileName, m_pDoc, MY_ENCODING);
  }
}

//----------------------------------------------------------------------------
// Write a comment in the xml file in the current node
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteComment(const char *_pszComment) {
  assert(_pszComment && m_pWriter);
  if (_pszComment && m_pWriter) {
    int rc = xmlTextWriterWriteComment(m_pWriter, BAD_CAST _pszComment);
    assert(rc >= 0);
    if (rc < 0)
      return false;
  } else
    return false;
  return true;
}

//----------------------------------------------------------------------------
// Starts a new node on the tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::StartElement(const char *_pszKey) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    // Start an element named "EXAMPLE". Since thist is the first element, this will be the root element of the document.
    int rc = xmlTextWriterStartElement(m_pWriter, BAD_CAST _pszKey);
    assert(rc >= 0);
    if (rc < 0)
      return false;
  } else
    return false;
  return true;
}

//----------------------------------------------------------------------------
// Starts a new node on the tree
//----------------------------------------------------------------------------
bool CXMLTreeNode::EndElement() {
  assert(m_pWriter);
  if (m_pWriter) {
    /* Close the element named HEADER. */
    int rc = xmlTextWriterEndElement(m_pWriter);
    assert(rc >= 0);
    if (rc < 0)
      return false;
  } else
    return false;
  return true;
}

//----------------------------------------------------------------------------
// Writes a string keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WritePszKeyword(const char *_pszKey, const char *_pszValue) {
  assert(_pszKey && _pszValue && m_pWriter);
  if (_pszKey && _pszValue && m_pWriter) {
    // Write an element
    int rc = xmlTextWriterWriteElement(m_pWriter, BAD_CAST _pszKey, BAD_CAST _pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes an integer keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteIntKeyword(const char *_pszKey, int _iValue) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%d", _iValue);
    // Write an element
    int rc = xmlTextWriterWriteElement(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes a float keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteFloatKeyword(const char *_pszKey, float _fValue) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%d", _fValue);
    // Write an element
    int rc = xmlTextWriterWriteElement(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes a boolean keyword
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteBoolKeyword(const char *_pszKey, bool _bValue) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%d", _bValue ? "true" : "false");
    // Write an element
    int rc = xmlTextWriterWriteElement(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes a string property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WritePszProperty(const char *_pszKey, const char *_pszValue) {
  assert(_pszKey && _pszValue && m_pWriter);
  if (_pszKey && _pszValue && m_pWriter) {
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST _pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes an integer property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteIntProperty(const char *_pszKey, int _iValue) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%d", _iValue);
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes a float property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteFloatProperty(const char *_pszKey, float _fValue) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%f", _fValue);
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes a boolean property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteBoolProperty(const char *_pszKey, bool _bValue) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, _bValue ? "true" : "false");
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}


//----------------------------------------------------------------------------
// Writes a Vect2f property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect2fProperty	(const char *_pszKey, const Vect2f &_value) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%f %f", _value.x, _value.y);
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes a Vect3f property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect3fProperty	(const char *_pszKey, const Vect3f &_value) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%f %f %f", _value.x, _value.y, _value.z);
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes a Vect2f property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect4fProperty	(const char *_pszKey, const Vect4f &_value) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%f %f %f %f", _value.x, _value.y, _value.z, _value.w);
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes a Vect2i property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect2iProperty	(const char *_pszKey, const Vect2i &_value) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%i %i", _value.x, _value.y);
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes a Vect3i property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect3iProperty	(const char *_pszKey, const Vect3i &_value) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%i %i %i", _value.x, _value.y, _value.z);
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Writes a Vect2f property
//----------------------------------------------------------------------------
bool CXMLTreeNode::WriteVect4iProperty	(const char *_pszKey, const Vect4i &_value) {
  assert(_pszKey && m_pWriter);
  if (_pszKey && m_pWriter) {
    char pszValue[32];
    sprintf_s(pszValue, "%i %i %i %i", _value.x, _value.y, _value.z, _value.w);
    int rc = xmlTextWriterWriteAttribute	(m_pWriter, BAD_CAST _pszKey, BAD_CAST pszValue);
    if (rc >= 0)
      return true;
  }
  return false;
}

//----------------------------------------------------------------------------
// Return true if node is a comment
//----------------------------------------------------------------------------
bool CXMLTreeNode::IsComment () const {
  if (m_pNode) {
    return (m_pNode->type == XML_COMMENT_NODE);
  }
  return false;
}