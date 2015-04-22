#pragma once
#ifndef INC_CONSOLE_H
#define INC_CONSOLE_H

#include "EditableTextBox.h"
#include "Utils\Defines.h"

class CInputManager;

class CConsole: public CEditableTextBox
{
public:
	CConsole( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
			const Vect2f position_percent, CColor textColor, uint32 fontID, std::string lit="", uint32 textHeightOffset=0, 
			uint32 textWidthOffset=0, bool isVisible = true, bool isActive = true);

	virtual ~CConsole() {/*NOTHING*/;}
	
	bool IsReturnPress ();
	void Update(CInputManager* intputManager, float elapsedTime);
	bool IsDuplicate(std::string input);

private:
	std::vector<std::string>	m_buffers;
	int							m_bufferPos;
};

#endif //INC_CONSOLE_H