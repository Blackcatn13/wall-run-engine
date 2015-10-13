#pragma once
#ifndef INC_CONSOLE_H
#define INC_CONSOLE_H

#include "EditableTextBox.h"
#include "Utils\Defines.h"
#include <vector>

class CInputManager;

class CConsole: public CEditableTextBox {
 public:
  CConsole( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
            const Vect2f position_percent, CColor textColor, uint32 fontID, std::string lit = "", uint32 textHeightOffset = 0,
            uint32 textWidthOffset = 0, bool isVisible = true, bool isActive = true);

  virtual ~CConsole() {
    /*NOTHING*/;
  }

  bool IsReturnPress ();
  void Update(CInputManager *intputManager, float elapsedTime);
  bool IsDuplicate(std::string input);
  std::string SearchString(int init, std::string input, bool forward = true);
  std::string DetectInitOfString(std::string input);
  void LoadWords(const std::string &FileName);
  bool GetIsFocus();

 private:
  std::vector<std::string>	m_buffers;
  int							m_bufferPos;
  std::string					m_FileName;
  std::vector<std::string>	m_words;			//Vector of words.
  std::string					m_completeBuffer;	//Original word before pressing TAB.
  int							m_completeCount;	//Number of the last word used using TAB.
  std::string					m_previousWord;		//Last word tabuled.
  bool						m_doAutoCompleteBack;   //Value to avoid the duplicate results during AutoComplete.
  bool						m_doAutoComplete;   //Value to avoid the duplicate results during AutoCompleteBack.
  bool						m_firstWord;		//Value to know if the autocompleted word is the unique one.
  bool						m_secondTry;		//Value to know if this autocomplete is the first one with this word.
};

#endif //INC_CONSOLE_H