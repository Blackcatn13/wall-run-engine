//----------------------------------------------------------------------------------
// CWindows class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el pintado y update de una ventana de la GUI. Esta ventana es simplemente una agrupacion
// de GuiElements que se lee al inicio de la aplicacion de un fichero .xml
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_WINDOWS_H
#define INC_WINDOWS_H

#include <string>
#include <vector>
#include <map>
//#include "Utils\BaseUtils.h"
#include <Math/MathTypes.h>
#include <Math/Vector3.h>
#include <Math/Vector2.h>

//---Forward Declarations---
class CGraphicsManager;
class CInputManager;
class CFontManager;
class CGuiElement;
class CTexture;
class CButton;
class CCheckButton;
class CXMLTreeNode;
class CTextureManager;
class CSlider;
class CDialogBox;
class CEditableTextBox;
class CRadioBox;
class CImage;
class CProgressBar;
class CStaticText;
class CButtonList;
//--------------------------

class CWindows {

 public:
  CWindows(): m_sLuaCode_OnKeyDown (""), m_uInputKeyDown(0), m_sLuaCode_OnUpdateWindows(""), m_sWindowsName(""),
    m_sLuaCode_OnLoadWindows(""), m_sLuaCode_OnSaveWindows("") {}
  virtual ~CWindows();

  void	Render							(CGraphicsManager *renderManager, CFontManager *fm);
  void	Update							(CInputManager *intputManager, float elapsedTime);
  bool	AddGuiElement				(CGuiElement *inGuiElement);
  bool	ReleaseGuiElement		(const std::string &name);
  void	SetName							(const std::string &name) {m_sWindowsName = name;}

  //--- PARSERFILE FUNCTION ---
  bool	LoadXML							(const std::string &xmlGuiFile, const Vect2i &screenResolution);
  void	RegisterElements		(std::map<std::string, CGuiElement *> &elements);
  void	LoadWindows					();
  void	SaveWindows					();

 private:
  void	IsKeyDown						(CInputManager *intputManager);

  CButton			*LoadButton					( CXMLTreeNode &pNewNode, const Vect2i &screenResolution, CTextureManager *tm);
  CCheckButton		*LoadCheckButton			( CXMLTreeNode &pNewNode, const Vect2i &screenResolution, CTextureManager *tm);
  CSlider			*LoadSlider					( CXMLTreeNode &pNewNode, const Vect2i &screenResolution, CTextureManager *tm);
  CDialogBox			*LoadDialogBox				( CXMLTreeNode &pNewNode, const Vect2i &screenResolution, CTextureManager *tm);
  CEditableTextBox	*LoadEditableTextBox (CXMLTreeNode &pNewNode, const Vect2i &screenResolution, CTextureManager *tm);
  CRadioBox			*LoadRadioBox				( CXMLTreeNode &pNewNode, const Vect2i &screenResolution, CTextureManager *tm);
  CImage				*_LoadImage					( CXMLTreeNode &pNewNode, const Vect2i &screenResolution, CTextureManager *tm);
  CProgressBar		*LoadProgressBar			( CXMLTreeNode &pNewNode, const Vect2i &screenResolution, CTextureManager *tm);
  CStaticText		*LoadStaticText			( CXMLTreeNode &pNewNode, const Vect2i &screenResolution, CTextureManager *tm);
  CButtonList    *LoadButtonList (CXMLTreeNode &node, const Vect2i &screenResolution, CTextureManager *tm);

 private:
  std::vector<CGuiElement *>	m_GuiElementsVector;
  std::string								m_sLuaCode_OnSaveWindows;
  std::string								m_sLuaCode_OnLoadWindows;
  std::string								m_sLuaCode_OnKeyDown;
  std::string								m_sLuaCode_OnUpdateWindows;
  uint32										m_uInputKeyDown;
  std::string								m_sWindowsName;
};

#endif //INC_WINDOWS_H