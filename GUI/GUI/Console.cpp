#include "Console.h"
#include "Core\Core.h"
#include "Core\ScriptManager.h"
#include "InputManager.h"
#include "Font\FontManager.h"

//---Constructor
CConsole::CConsole( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
                    const Vect2f position_percent, CColor textColor, uint32 fontID,
                    std::string lit, uint32 textHeightOffset, uint32 textWidthOffset,
                    bool isVisible,  bool isActive)
  : CEditableTextBox(	windowsHeight, windowsWidth, height_precent, witdh_percent,	position_percent, textColor, fontID,
                      lit, textHeightOffset, textWidthOffset, isVisible, isActive),
    m_bufferPos(0),
    m_FileName(""),
    m_completeBuffer(""),
    m_completeCount(0),
    m_previousWord(""),
    m_doAutoComplete(false),
    m_doAutoCompleteBack(false),
    m_firstWord(false),
    m_secondTry(false) {
  LoadWords("./Data/ConsoleDictionary.xml");
}

bool CConsole::IsReturnPress () {
  if ( m_bReturnPress && m_sBuffer != "") {
    std::string bufferAux = m_sBuffer.substr(0, m_sBuffer.size() - 1);
    m_bReturnPress = false;
    m_sBuffer = "";
    m_uCursorPos = 0;
    if (!IsDuplicate(bufferAux)) {
      SCRIPTM->RunCode(bufferAux);
      m_buffers.push_back(std::string(bufferAux));
      m_bufferPos = m_buffers.size();
      return true;
    } else {
      SCRIPTM->RunCode(bufferAux);
      return true;
    }
  } else
    return false;
}

bool CConsole::IsDuplicate(std::string input) {
  for (size_t i = 0; i < m_buffers.size(); i++) {
    if (std::string(m_buffers[i]).compare(input) == 0)
      return true;
  }
  return false;
}

std::string CConsole::SearchString(int init, std::string input, bool forward) {
  std::string result = "";
  std::string inputAux = DetectInitOfString(input);
  int initAux = init;
  if (forward) {
    if (m_doAutoCompleteBack)
      initAux = initAux + 2;

    for (size_t i = initAux; i < m_words.size(); i++) {
      //if (std::string(m_words[i]).find(input) != std::string::npos) {
      if ( std::string(m_words[i]).substr(0, inputAux.size()).compare(inputAux) == 0 ) {
        result = m_words[i];
        m_completeCount = i + 1;
        m_doAutoComplete = true;
        m_doAutoCompleteBack = false;
        return result;
      } else {
        m_completeCount += 1;
      }
    }
    if (m_completeCount > m_words.size() - 1)
      m_completeCount = 0;
    m_doAutoComplete = true;
    m_doAutoCompleteBack = false;
  } else {
    if (m_doAutoComplete)
      initAux = initAux - 2;
    if (initAux < 0)
      initAux = 0;
    for (int i = initAux; i >= 0; i--) {
      if ( std::string(m_words[i]).substr(0, inputAux.size()).compare(inputAux) == 0 ) {
        result = m_words[i];
        m_completeCount = i - 1;
        m_doAutoComplete = false;
        m_doAutoCompleteBack = true;
        return result;
      } else {
        m_completeCount -= 1;
      }
    }
    if (m_completeCount <= 0)
      m_completeCount = m_words.size() - 1;
    m_doAutoComplete = false;
    m_doAutoCompleteBack = true;
  }
  return result;
}

std::string CConsole::DetectInitOfString(std::string input) {
  std::string result = "";
  std::string inputAux;
  if (!m_secondTry)
    inputAux = input;
  else inputAux = m_completeBuffer;
  for (int i = inputAux.size() - 1; i >= 0; i--) {
    std::string aux;
    aux.push_back(inputAux[i]);
    if (std::string(aux).compare(".") == 0 || std::string(aux).compare(":") == 0 ) {
      result = inputAux.substr(i + 1, inputAux.size() - 1);
      m_sBuffer = inputAux.substr(0, (inputAux.size() - result.size()));
      m_firstWord = false;
      return result;
    }
    aux.pop_back();
  }
  m_firstWord = true;
  return input;
}

void CConsole::Update(CInputManager *intputManager, float elapsedTime) {
  std::string buffer_prev = m_sBuffer;
  if ( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive ) {
    Vect2i mousePosition;
    intputManager->GetPosition(IDV_MOUSE, mousePosition);
    CGuiElement::CalculatePosMouse(mousePosition);
    if ( CGuiElement::IsInside() ) {
      if (intputManager->IsUpDown(IDV_MOUSE, MOUSE_BUTTON_LEFT)) {
        CGuiElement::m_bFocus = true;
      }
    }
  }
  if (CGuiElement::m_bFocus) {
    m_bShift = (intputManager->IsDown(IDV_KEYBOARD, KEY_RSHIFT) || intputManager->IsDown(IDV_KEYBOARD, KEY_LSHIFT) );
    if ( intputManager->IsUpDown(IDV_KEYBOARD, KEY_LEFT) ) {
      if (m_uCursorPos > 0) {
        m_uCursorPos = m_uCursorPos - 1;
      }
    }
    if ( intputManager->IsUpDown(IDV_KEYBOARD, KEY_RIGHT) ) {
      if (m_uCursorPos < m_sBuffer.size()) {
        m_uCursorPos = m_uCursorPos + 1;
      }
    }
    if ( intputManager->IsUpDown(IDV_KEYBOARD, KEY_BACK) ) {
      Remove1character();
    } else {
      ProcessNewEntries(intputManager);
    }
    if ( intputManager->IsUpDown(IDV_KEYBOARD, KEY_RETURN) ) {
      m_bReturnPress = true;
      IsReturnPress();
    }
    if ( intputManager->IsDown(IDV_KEYBOARD, KEY_BACK) ) {
      m_BackTime1 += elapsedTime;
      if (m_BackTime1 > 0.7f) {
        m_BackTime2 += elapsedTime;
        if (m_BackTime2 > 0.05f) {
          Remove1character();
          m_BackTime2 = 0.f;
        }
      }
    } else {
      m_BackTime1 = 0.f;
      m_BackTime2 = 0.f;
    }
    if (intputManager->IsUpDown(IDV_KEYBOARD, KEY_RSHIFT)) {
      if (m_uCursorPos < m_sBuffer.size()) {
        m_sBuffer.erase( m_sBuffer.begin() + m_uCursorPos );
      }
    }
    m_fTimeCount += elapsedTime;
    if ( m_fTimeCount > m_fTime ) {
      if ( !m_sFocusObject.compare(" ") )
        m_sFocusObject = "_";
      else
        m_sFocusObject = " ";
      m_fTimeCount = 0.f;
    }
    if (ACT2IN->DoAction("KeyMoveBack") && m_buffers.size() > 0) {
      if (m_bufferPos < m_buffers.size() - 1) {
        m_bufferPos += 1;
        m_sBuffer = m_buffers[m_bufferPos];
        m_uCursorPos = m_sBuffer.size();
      } else {
        m_sBuffer = "";
        m_uCursorPos = 0;
      }
    }
    if (ACT2IN->DoAction("KeyMoveForward") && m_buffers.size() > 0) {
      if (m_bufferPos > 0)
        m_bufferPos -= 1;
      m_sBuffer = m_buffers[m_bufferPos];
      m_uCursorPos = m_sBuffer.size();
    }
    if (ACT2IN->DoAction("ConsoleStart")) {
      m_uCursorPos = 0;
    }
    if (ACT2IN->DoAction("ConsoleEnd")) {
      m_uCursorPos = m_sBuffer.size();
    }
    if (ACT2IN->DoAction("ConsoleCompleteBack")) {
      std::string bufferAux = m_sBuffer.substr(0, m_sBuffer.size() - 1);
      if (m_previousWord.compare(bufferAux) != 0) {
        m_completeBuffer = bufferAux;
        m_completeCount = m_sBuffer.size() - 1;
        m_secondTry = false;
        m_doAutoComplete = false;
        m_doAutoCompleteBack = false;
      } else
        m_secondTry = true;
      std::string search = SearchString(m_completeCount, m_completeBuffer, false);
      if (search.compare("") != 0) {
        if (!m_firstWord)
          m_sBuffer += search;
        else m_sBuffer = search;
        m_uCursorPos = m_sBuffer.size();
        m_previousWord = m_sBuffer;
      } else {
        m_sBuffer = m_completeBuffer;
        m_uCursorPos = m_sBuffer.size();
        m_previousWord = m_completeBuffer;
      }
    } else {
      if (ACT2IN->DoAction("ConsoleComplete")) {
        std::string bufferAux = m_sBuffer.substr(0, m_sBuffer.size() - 1);
        if (m_previousWord.compare(bufferAux) != 0) {
          m_completeBuffer = bufferAux;
          m_completeCount = 0;
          m_secondTry = false;
          m_doAutoComplete = false;
          m_doAutoCompleteBack = false;
        } else
          m_secondTry = true;
        std::string search = SearchString(m_completeCount, m_completeBuffer);
        if (search.compare("") != 0) {
          if (!m_firstWord)
            m_sBuffer += search;
          else m_sBuffer = search;
          m_uCursorPos = m_sBuffer.size();
          m_previousWord = m_sBuffer;
        } else {
          m_sBuffer = m_completeBuffer;
          m_uCursorPos = m_sBuffer.size();
          m_previousWord = m_completeBuffer;
        }
      }
    }
  }// End if (CGuiElement::m_bFocus)
  std::string temp = m_sBuffer + m_sFocusObject;
  CFontManager *fm = CCORE->GetFontManager();
  uint32 sizeX = fm->SizeX(temp.c_str(), m_uFontID) + (uint32)(2 * m_fWidthMargin);
  if (sizeX > CGuiElement::m_uWidth) {
    m_sBuffer = buffer_prev;
  }
}

void CConsole::LoadWords(const std::string &FileName) {
  m_FileName = FileName;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(FileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode m = newFile["ConsoleWords"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode child = m.getNextChild();
        std::string name = child.GetName();
        if (name == "word") {
          std::string word = child.GetPszProperty("string");
          m_words.push_back(std::string(word));
        }
      }
    }
  }
}

bool CConsole::GetIsFocus() {
  return m_bFocus;
}
