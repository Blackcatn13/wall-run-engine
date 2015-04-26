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
              const Vect2f position_percent, CColor textColor, uint32 fontID, std::string lit = "", uint32 textHeightOffset = 0,
              uint32 textWidthOffset = 0, bool isVisible = true, bool isActive = true);

    virtual ~CConsole()
    {
        /*NOTHING*/;
    }

    bool IsReturnPress ();
    void Update(CInputManager* intputManager, float elapsedTime);
    bool IsDuplicate(std::string input);
    std::string SearchString(int init, std::string input);
    void LoadWords(const std::string &FileName);

private:
    std::vector<std::string>	m_buffers;
    int							m_bufferPos;
    std::string					m_FileName;
    std::vector<std::string>	m_words;			//Vector of words.
    std::string					m_completeBuffer;	//Original word before pressing TAB.
    int							m_completeCount;	//Number of the last word used using TAB.
    int							m_previousSize;		//Size to detect if any character is written using TABs.
};

#endif //INC_CONSOLE_H