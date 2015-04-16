//----------------------------------------------------------------------------------
// CStaticText class
// Author: Enric Vergara
//
// Description:
// Static text may be changed by the program, but not directly by the user. You might use this for labels or
// instructions to the user.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_STATIC_TEXT_H
#define INC_STATIC_TEXT_H

#include <string>
#include "GuiElement.h"
#include "Math/Color.h"

//---Forward Declarations---
//--------------------------


class CStaticText: public CGuiElement
{

public:
	CStaticText(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
								const Vect2f position_percent,  std::string lit="", bool isVisible = true, bool isActive = true);
	
	virtual ~CStaticText() {/*NOTHING*/;}


	//---------------CGuiElement Interface----------------------
	virtual void	Render							(CGraphicsManager *renderManager, CFontManager* fm);
	virtual void	Update							(CInputManager* intputManager, float elapsedTime);
	virtual void	OnClickedChild			(const std::string& name) {/*NOTHING*/;}

	//---------------CStaticText Interface------------------
	void					SetLiteral					(const std::string& lit);

private:
};

#endif //INC_STATIC_TEXT_H