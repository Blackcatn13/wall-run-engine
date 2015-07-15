#pragma once
#ifndef BUTTON_LIST_H
#define BUTTON_LIST_H

#include <string>
#include <vector>
#include "GuiElement.h"
#include "Math/Color.h"
//#include "Utils\BaseUtils.h"
#include <Math/MathTypes.h>
#include <Math/Vector3.h>
#include <Math/Vector2.h>

//---Forward Declarations---
class CTexture;
class CXMLTreeNode;
class CButton;
//--------------------------

class CButtonList: public CGuiElement {
 public:

  CButtonList(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
                const Vect2f position_percent, uint32 columns, uint32 rows, std::string lit = "", uint32 textHeightOffset = 0,
                uint32 textWidthOffsetbool = 0, bool isVisible = true, bool isActive = true);

  CButtonList(CXMLTreeNode &node, uint32 windowsHeight, uint32 windowsWidth);

  virtual ~CButtonList();

  //---------------CGuiElement Interface----------------------
  virtual void	Render							(CGraphicsManager *renderManager, CFontManager *fm);
  virtual void	Update							(CInputManager *intputManager, float elapsedTime);
  virtual void	OnClickedChild			(const std::string &name);

 private:
  int m_currentId;
  std::vector<CButton *> m_buttons;
  Vect2i m_lastMousePosition;
};

#endif //INC_BUTTON_H