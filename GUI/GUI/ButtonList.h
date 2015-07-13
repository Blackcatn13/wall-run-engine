#pragma once
#ifndef BUTTON_LIST_H
#define BUTTON_LIST_H

#include <string>
#include "GuiElement.h"
#include "Math/Color.h"
//#include "Utils\BaseUtils.h"
#include <Math/MathTypes.h>
#include <Math/Vector3.h>
#include <Math/Vector2.h>

//---Forward Declarations---
class CTexture;
//--------------------------

class CButtonList: public CGuiElement {
 public:

  CButtonList(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
                const Vect2f position_percent, std::string lit = "", uint32 textHeightOffset = 0,
                uint32 textWidthOffsetbool = 0, bool isVisible = true, bool isActive = true);

  virtual ~CButtonList() {/*NOTHING*/;}

  //---------------CGuiElement Interface----------------------
  virtual void	Render							(CGraphicsManager *renderManager, CFontManager *fm);
  virtual void	Update							(CInputManager *intputManager, float elapsedTime);
  virtual void	OnClickedChild			(const std::string &name) {/*NOTHING*/;}
};

#endif //INC_BUTTON_H