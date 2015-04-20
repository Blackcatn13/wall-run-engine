#include "Console.h"

//---Constructor
CConsole( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
																		const Vect2f position_percent, CColor textColor, uint32 fontID, 
																		std::string lit, uint32 textHeightOffset, uint32 textWidthOffset, 
																		bool isVisible = true,  bool isActive = true)
: CEditableTextBox(	windowsHeight, windowsWidth, height_precent, witdh_percent,	position_percent, textColor, fontID, 
				lit, textHeightOffset, textWidthOffset, isVisible, isActive)
{}

bool CConsole::IsReturnPress (){
	if( m_bReturnPress )
	{
		m_bReturnPress = false;
		return true;
	}
	else
		return false;
}

