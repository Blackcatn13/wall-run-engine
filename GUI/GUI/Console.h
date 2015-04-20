#pragma once
#ifndef INC_CONSOLE_H
#define INC_CONSOLE_H

#include "EditableTextBox.h"

class CConsole: public CEditableTextBox
{
public:
	CConsole( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
			const Vect2f position_percent, CColor textColor, uint32 fontID, std::string lit, uint32 textHeightOffset, uint32 textWidthOffset, 
			bool isVisible = true,  bool isActive = true);

	virtual ~CConsole() {/*NOTHING*/;}
	
	bool IsReturnPress ();
};

#endif //INC_IMAGE_H