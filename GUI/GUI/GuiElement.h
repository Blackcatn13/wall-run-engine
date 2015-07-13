//----------------------------------------------------------------------------------
// CGuiElement class
// Author: Enric Vergara
//
// Description:
// Clase Base con los datos y funcionalidad común a todos los elementos de la GUI.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_GUI_ELEMENT_H
#define INC_GUI_ELEMENT_H

#include <string>
#include <vector>
#include <assert.h>
//#include "Utils\BaseUtils.h"
#include <Math/MathTypes.h>
#include <Math/Vector3.h>
#include <Math/Vector2.h>
//---Forward Declarations---
class CGraphicsManager;
class CFontManager;
class CInputManager;
class CActionToInput;
//--------------------------

class CGuiElement {
 public:
  typedef enum {	BUTTON = 0, CHECKBUTTON, STATIC_TEXT, SLIDER, IMAGE, EDITABLE_TEXT_BOX,
                  PROGRESS_BAR, DIALOG_BOX, RADIO_BOX, BUTTON_LIST
               } TypeGuiElement;

  typedef enum {  EDITPOS, EDITSIZE, DISABLE } TypeEditMode;

 public:
  CGuiElement(	uint32 windowsHeight, uint32 windowsWidth, float height_percent, float width_percent,
                Vect2f position_percent, TypeGuiElement type, std::string lit = "",  uint32 textHeightOffset = 0,
                uint32 textWidthOffset = 0, bool isVisible = true, bool isActive = true );

  virtual ~CGuiElement() {/*Nothing*/;}

  virtual void				Render								(CGraphicsManager *render, CFontManager *fm);
  virtual void				RenderText						(CGraphicsManager *render, CFontManager *fm);
  virtual void				Update								(CInputManager *input, float elapsedTime);
  virtual void				OnClickedChild				(const std::string &name) {};

  void								OnLoadValue						();
  void								OnSaveValue						();

  void								SetOnLoadValueAction	(const std::string &inAction) {m_sLuaCode_OnLoadValue = inAction;}
  void								SetOnSaveValueAction	(const std::string &inAction) {m_sLuaCode_OnSaveValue = inAction;}


  void								CalculatePosMouse			(const Vect2i &mousePosition);

  bool								IsOver								() const 											{return m_bIsOver;}
  bool								IsInside							() const 											{return m_bIsInside;}
  bool								IsVisible							() const 											{return m_bIsVisible;}
  bool								IsActive							() const 											{return m_bIsActive;}

  void								SetVisible						(bool flag) 									{m_bIsVisible = flag;}
  void								SetActive							(bool flag) 									{m_bIsActive = flag;}

  uint32							GetWidth							() const 											{return m_uWidth;}
  float								GetWidthPercent				() const 											{return m_fWidthPercent;}
  uint32							GetHeight							() const 											{return m_uHeight;}
  float								GetHeightPercent			() const 											{return m_fHeightPercent;}

  Vect2i							GetPosition						() const 											{return m_Position;}
  const Vect2f				&GetPositionPercent		() const 											{return m_PositionPercent;}

  void								SetWindowsWidth				(uint32	w);
  void								SetWindowsHeight			(uint32	h);
  void								SetHeightPercent			(float height_percent);
  void								SetWidthPercent				(float width_percent);
  void								SetPositionPercent		(const Vect2f &pos);
  virtual void				SetPosition						(const Vect2i &pos);

  void								SetLiteral						(const std::string &lit)			{m_sLiteral = lit;}
  void								SetTextHeightOffset		(uint32 textHeightOffset)			{m_uTextHeightOffset = textHeightOffset;}
  void								SetTextWidthOffset		(uint32 textWidthOffset)			{m_uTextWidthOffset = textWidthOffset;}
  const std::string	&GetLiteral						()														{return m_sLiteral;}
  uint32							GetTextHeightOffset		()														{return m_uTextHeightOffset;}
  uint32							GetTextWidthOffset		()														{return m_uTextWidthOffset;}


  void								SetParent							(CGuiElement *parent)				 	{assert(parent); m_pParent = parent;}
  bool								AddChild							(CGuiElement *child);
  bool								RemoveChild						(CGuiElement *child);


  void								GainFocus							() 														{m_bFocus = true;}
  void								LoseFocus							() 														{m_bFocus = false;}
  bool								HasFocus							() const											{return m_bFocus;}

  void								SetName								(const std::string &name)			{m_sName = name;}
  const std::string	&GetName								() const											{return m_sName;}
  TypeGuiElement			GetType								() const {return m_eType;}


 protected:

  float											m_fWidthPercent;					//% del Ancho del GuiElement respecto el size del Window
  uint32										m_uWidth;									//Ancho en pixeles del GuiElement
  float											m_fHeightPercent;					//% de Altura del GuiElement respecto el size del Window
  uint32										m_uHeight;								//Altura en pixeles del GuiElement
  uint32										m_uWindowsWidth;					//Ancho en pixeles de la windows que contiene a este GuiElement
  uint32										m_uWindowsHeight;					//Altura en pixeles de la windows que contiene a este GuiElement
  Vect2f										m_PositionPercent;				//Posición del GuiElement (x,y) en % respecto el size del Window
  Vect2i										m_Position;								//Posición del GuiElement (x,y) en pixeles
  bool											m_bIsVisible;							//Indica si el elemento es o no visible
  bool											m_bIsActive;							//Indica si el elemento está o no activado
  bool											m_bIsOver;								//Indica si el puntero del mouse es la primera vez que ha entrado
  bool											m_bIsInside;							//Indica si el puntero del mouse está dentro
  bool											m_bFocus;									//Indica que es el guielement que tiene el foco dentro de la windows actual
  std::string								m_sName;									//Identificador del GuiElement
  std::string								m_sLuaCode_OnLoadValue;		//Codigo LUA que ha de ejecutar cuando se entre por primera vez en la ventana que contiene al GuiElement
  std::string								m_sLuaCode_OnSaveValue;		//Codigo LUA que ha de ejecutar cuando se salga de la ventana que contiene al GuiElement
  std::vector<CGuiElement *>	m_Children;								//GuiElements que estan atachados a este.
  CGuiElement							*m_pParent;								//GuiElements al que esta atachado este.
  TypeEditMode							m_eEditMode;							//Indica si esta en estado de edicion o navegacion.
  TypeGuiElement						m_eType;									//Indica el tipo de GuiElement que sera (button, checkbutton...)
  uint32										m_z;											//Indice de profundidad en la que se pintara el elemento
  std::string								m_sLiteral;
  uint32										m_uTextHeightOffset;
  uint32										m_uTextWidthOffset;
};

#endif //INC_GUI_ELEMENT_H
