#include "ActionToInput.h"
#include "XML\XMLTreeNode.h"
#include "InputManager.h"

CActionToInput::CActionToInput(CInputManager *input) {
  m_InputManager = input;
  InitString2Input();
}

void CActionToInput::GetActionInfo(std::string actionName, std::string &strInfo) {
  std::string str_aux;
  if (m_GlobalString2Actions.count(actionName)) {
    VecInfoInputs aux = m_GlobalString2Actions[actionName];

    int size = aux.size();
    for (size_t i = 0; i < size; ++ i) {
      std::map<std::string, uint32>::iterator it = m_String2Code.begin();
      for (it; it != m_String2Code.end(); ++it) {
        if (it->second == aux[i].Code) {
          str_aux.append(it->first);
          if (i + 1 != size )
            str_aux.append(" + ");
        }

      }
    }
  }
  strInfo = str_aux;

}
bool CActionToInput::DoAction(const std::string &action_name) {
  std::string aux1 = action_name;
  if (m_GlobalString2Actions.count(action_name)) {
    VecInfoInputs aux = m_GlobalString2Actions[aux1];
    bool action = true;
    for (uint32 i = 0; i < aux.size(); ++i) {
      switch (aux[i].EventType) {
        case EVENT_DOWN:
          action &= m_InputManager->IsDown(aux[i].deviceType, aux[i].Code);
          if (!action) return false;
          break;
        case EVENT_DOWN_UP:
          action &= m_InputManager->IsDownUp(aux[i].deviceType, aux[i].Code);
          if (!action) return false;
          break;
        case EVENT_UP_DOWN:
          action &= m_InputManager->IsUpDown(aux[i].deviceType, aux[i].Code);
          if (!action) return false;
          break;
        case EVENT_NOTHING:
          return false;
      }
    }
    return action;
  }
  return false;
}

bool CActionToInput::DoAction(const std::string &action_name, float &amount) {
  std::string aux1 = action_name;
  if (m_GlobalString2Actions.count(action_name)) {
    VecInfoInputs aux = m_GlobalString2Actions[aux1];
    bool action = true;
    for (uint32 i = 0; i < aux.size(); ++i) {
      switch (aux[i].axisType) {
        case AXIS_MOUSE_X:
          amount = (float)m_InputManager->GetMouseDelta().x;
          action &= amount != 0;
          if (!action) return false;
          break;
        case AXIS_MOUSE_Y:
          amount = (float)m_InputManager->GetMouseDelta().y;
          action &= amount != 0;
          if (!action) return false;
          break;
        case AXIS_MOUSE_Z:
          amount = (float)m_InputManager->GetMouseDelta().z;
          action &= amount != 0 ;
          if (!action) return false;
          break;
        case AXIS_LEFT_THUMB_X: {
          float aux;
          action = m_InputManager->GetGamePadLeftThumbDeflection(&amount, &aux);
          action &= amount != 0;
          if (!action) return false;
          break;
        }
        case AXIS_LEFT_THUMB_Y: {
          float aux;
          action = m_InputManager->GetGamePadLeftThumbDeflection(&aux, &amount);
          action &= amount != 0;
          if (!action) return false;
          break;
        }
        case AXIS_RIGHT_THUMB_X: {
          float aux;
          action = m_InputManager->GetGamePadRightThumbDeflection(&amount, &aux);
          action &= amount != 0;
          if (!action) return false;
          break;
        }
        case AXIS_RIGHT_THUMB_Y: {
          float aux;
          action = m_InputManager->GetGamePadRightThumbDeflection(&aux, &amount);
          action &= amount != 0;
          if (!action) return false;
          break;
        }
        case AXIS_DELTA_TRIGGER_RIGHT:
        case AXIS_DELTA_TRIGGER_LEFT:
        case AXIS_NOTHING:
          break;
      }
      switch (aux[i].EventType) {
        case EVENT_DOWN:
          action &= m_InputManager->IsDown(aux[i].deviceType, aux[i].Code);
          if (!action) return false;
          break;
        case EVENT_DOWN_UP:
          action &= m_InputManager->IsDownUp(aux[i].deviceType, aux[i].Code);
          if (!action) return false;
          break;
        case EVENT_UP_DOWN:
          action &= m_InputManager->IsUpDown(aux[i].deviceType, aux[i].Code);
          if (!action) return false;
          break;
        case EVENT_NOTHING:
          break;
      }
    }
    return action;
  }
  return false;
}

bool CActionToInput::DoActionFromLua(const std::string action_name) {
  return DoAction(action_name);
}

bool CActionToInput::DoActionFromLua(const std::string action_name,  float amount) {
  return DoAction(action_name, amount);
}

bool CActionToInput::ReloadXML() {
  m_GlobalString2Actions.clear();
  m_GamePadString2Actions.clear();
  m_MouseKeyboardString2Actions.clear();
  LoadXML(m_fileName);
  return true;
}

bool CActionToInput::LoadXML (const std::string &xmlFile) {
  m_fileName = xmlFile;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(xmlFile.c_str())) {
    printf("ERROR loading the file.");
  }
  CXMLTreeNode m = newFile["Actions"];
  if (m.Exists()) {
    int count = m.GetNumChildren();
    int counter = 0;
    for (int i = 0; i < count; ++i) {
      CXMLTreeNode childNode = m(i);
      std::string name = childNode.GetName();
      MapString2Actions *toInsert;
      if (name == "globals") {
        toInsert = &m_GlobalString2Actions;
      } else if (name == "mouse_keyboard") {
        toInsert = &m_MouseKeyboardString2Actions;
      } else if (name == "gamepad") {
        toInsert = &m_GamePadString2Actions;
      }
      int newCount = childNode.GetNumChildren();
      for (int j = 0; j < newCount; ++j) {
        CXMLTreeNode childNode1 = childNode(j);
        std::string name1 = childNode1.GetName();
        if (name1 == "action") {
          std::string nameAction = childNode1.GetPszISOProperty("name", "Action" + counter++, false);
          int actionsCount = childNode1.GetNumChildren();
          (*toInsert)[nameAction] = VecInfoInputs();
          (*toInsert)[nameAction].clear();
          for (int k = 0; k < actionsCount; ++k) {
            CXMLTreeNode childNode2 = childNode1(k);
            std::string nameDevice = childNode2.GetPszISOProperty("deviceType", "IDV_NOTHING", false);
            std::string nameEvent = childNode2.GetPszISOProperty("EventType", "EVENT_NOTHING", false);
            std::string nameKey = childNode2.GetPszISOProperty("Code", "KEY_M", false);
            std::string axisKey = childNode2.GetPszISOProperty("AxisType", "AXIS_NOTHING", false);
            action_info actAux = {static_cast<INPUT_DEVICE_TYPE>(m_String2Code[nameDevice]),
                                  static_cast<INPUT_EVENT_TYPE>(m_String2Code[nameEvent]),
                                  (uint32) m_String2Code[nameKey],
                                  static_cast<INPUT_AXIS_TYPE>(m_String2Code[axisKey])
                                 };
            (*toInsert)[nameAction].push_back(actAux);
          }
        }
      }
    }
  }
  if (m_InputManager->HasGamePad()) {
    m_GlobalString2Actions.insert(m_GamePadString2Actions.begin(), m_GamePadString2Actions.end());
  } else {
    m_GlobalString2Actions.insert(m_MouseKeyboardString2Actions.begin(), m_MouseKeyboardString2Actions.end());
  }
  return true;
}

void CActionToInput::InitString2Input () {
  //----------------INPUT_DEVICE_TYPE---------------//
  m_String2Code.insert( PairString2Code("IDV_KEYBOARD",							IDV_KEYBOARD) );
  m_String2Code.insert( PairString2Code("IDV_MOUSE",								IDV_MOUSE) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD1",							IDV_GAMEPAD1) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD2",							IDV_GAMEPAD2) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD3",							IDV_GAMEPAD3) );
  m_String2Code.insert( PairString2Code("IDV_GAMEPAD4",							IDV_GAMEPAD4) );
  m_String2Code.insert( PairString2Code("IDV_NOTHING",							IDV_NOTHING) );
  //----------------INPUT_AXIS_TYPE-----------------//
  m_String2Code.insert( PairString2Code("AXIS_MOUSE_X",							AXIS_MOUSE_X) );
  m_String2Code.insert( PairString2Code("AXIS_MOUSE_Y",							AXIS_MOUSE_Y) );
  m_String2Code.insert( PairString2Code("AXIS_MOUSE_Z",							AXIS_MOUSE_Z) );
  m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_X",				AXIS_LEFT_THUMB_X) );
  m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_Y",				AXIS_LEFT_THUMB_Y) );
  m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_X",				AXIS_RIGHT_THUMB_X) );
  m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_Y",				AXIS_RIGHT_THUMB_Y) );
  m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_RIGHT",	AXIS_DELTA_TRIGGER_RIGHT) );
  m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_LEFT",	AXIS_DELTA_TRIGGER_LEFT) );
  m_String2Code.insert( PairString2Code("AXIS_NOTHING",							AXIS_NOTHING) );
  //----------------INPUT_EVENT_TYPE-----------------//
  m_String2Code.insert( PairString2Code("EVENT_DOWN",								EVENT_DOWN) );
  m_String2Code.insert( PairString2Code("EVENT_UP_DOWN",						EVENT_UP_DOWN) );
  m_String2Code.insert( PairString2Code("EVENT_DOWN_UP",						EVENT_DOWN_UP) );
  m_String2Code.insert( PairString2Code("EVENT_NOTHING",						EVENT_NOTHING) );
  //----------------MOUSE_BUTTON---------------------//
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_LEFT",				MOUSE_BUTTON_LEFT) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_RIGHT",				MOUSE_BUTTON_RIGHT) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE",			MOUSE_BUTTON_MIDDLE) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE",			MOUSE_BUTTON_MIDDLE) );
  m_String2Code.insert( PairString2Code("MOUSE_BUTTON_NOTHING",			MOUSE_BUTTON_NOTHING) );
  //----------------Gamepad codes from XInput.h------//
  m_String2Code.insert( PairString2Code("PAD_DPAD_UP",							PAD_DPAD_UP) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_DOWN",						PAD_DPAD_DOWN) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_LEFT",						PAD_DPAD_LEFT) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_RIGHT",						PAD_DPAD_RIGHT) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_START",						PAD_DPAD_START) );
  m_String2Code.insert( PairString2Code("PAD_DPAD_BACK",						PAD_DPAD_BACK) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_THUMB",		PAD_BUTTON_LEFT_THUMB) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_THUMB",		PAD_BUTTON_RIGHT_THUMB) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_SHOULDER",	PAD_BUTTON_LEFT_SHOULDER) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_SHOULDER", PAD_BUTTON_RIGHT_SHOULDER) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_A",							PAD_BUTTON_A) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_B",							PAD_BUTTON_B) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_X",							PAD_BUTTON_X) );
  m_String2Code.insert( PairString2Code("PAD_BUTTON_Y",							PAD_BUTTON_Y) );
  //-----------Keyboard scan codes from dinput.h ------//
  m_String2Code.insert( PairString2Code("KEY_ESCAPE",								KEY_ESCAPE) );
  m_String2Code.insert( PairString2Code("KEY_TAB",									KEY_TAB) );
  m_String2Code.insert( PairString2Code("KEY_SPACE",								KEY_SPACE) );
  m_String2Code.insert( PairString2Code("KEY_RETURN",								KEY_RETURN) );
  m_String2Code.insert( PairString2Code("KEY_BACK",									KEY_BACK) );
  m_String2Code.insert( PairString2Code("KEY_CAPITAL",							KEY_CAPITAL) );
  m_String2Code.insert( PairString2Code("KEY_MINUS",								KEY_MINUS) );
  m_String2Code.insert( PairString2Code("KEY_EQUALS",								KEY_EQUALS) );
  m_String2Code.insert( PairString2Code("KEY_LBRACKET",							KEY_LBRACKET) );
  m_String2Code.insert( PairString2Code("KEY_RBRACKET",							KEY_RBRACKET) );
  m_String2Code.insert( PairString2Code("KEY_SEMICOLON",						KEY_SEMICOLON) );
  m_String2Code.insert( PairString2Code("KEY_APOSTROPHE",						KEY_APOSTROPHE) );
  m_String2Code.insert( PairString2Code("KEY_GRAVE",								KEY_GRAVE) );
  m_String2Code.insert( PairString2Code("KEY_BACKSLASH",						KEY_BACKSLASH) );
  m_String2Code.insert( PairString2Code("KEY_COMMA",								KEY_COMMA) );
  m_String2Code.insert( PairString2Code("KEY_PERIOD",								KEY_PERIOD) );
  m_String2Code.insert( PairString2Code("KEY_SLASH",								KEY_SLASH) );
  m_String2Code.insert( PairString2Code("KEY_A",										KEY_A) );
  m_String2Code.insert( PairString2Code("KEY_S",										KEY_S) );
  m_String2Code.insert( PairString2Code("KEY_D",										KEY_D) );
  m_String2Code.insert( PairString2Code("KEY_F",										KEY_F) );
  m_String2Code.insert( PairString2Code("KEY_G",										KEY_G) );
  m_String2Code.insert( PairString2Code("KEY_H",										KEY_H) );
  m_String2Code.insert( PairString2Code("KEY_J",										KEY_J) );
  m_String2Code.insert( PairString2Code("KEY_K",										KEY_K) );
  m_String2Code.insert( PairString2Code("KEY_L",										KEY_L) );
  m_String2Code.insert( PairString2Code("KEY_X",										KEY_X) );
  m_String2Code.insert( PairString2Code("KEY_C",										KEY_C) );
  m_String2Code.insert( PairString2Code("KEY_B",										KEY_B) );
  m_String2Code.insert( PairString2Code("KEY_N",										KEY_N) );
  m_String2Code.insert( PairString2Code("KEY_M",										KEY_M) );
  m_String2Code.insert( PairString2Code("KEY_Q",										KEY_Q) );
  m_String2Code.insert( PairString2Code("KEY_W",										KEY_W) );
  m_String2Code.insert( PairString2Code("KEY_E",										KEY_E) );
  m_String2Code.insert( PairString2Code("KEY_R",										KEY_R) );
  m_String2Code.insert( PairString2Code("KEY_T",										KEY_T) );
  m_String2Code.insert( PairString2Code("KEY_Y",										KEY_Y) );
  m_String2Code.insert( PairString2Code("KEY_U",										KEY_U) );
  m_String2Code.insert( PairString2Code("KEY_I",										KEY_I) );
  m_String2Code.insert( PairString2Code("KEY_O",										KEY_O) );
  m_String2Code.insert( PairString2Code("KEY_P",										KEY_P) );
  m_String2Code.insert( PairString2Code("KEY_1",										KEY_1) );
  m_String2Code.insert( PairString2Code("KEY_2",										KEY_2) );
  m_String2Code.insert( PairString2Code("KEY_3",										KEY_3) );
  m_String2Code.insert( PairString2Code("KEY_4",										KEY_4) );
  m_String2Code.insert( PairString2Code("KEY_5",										KEY_5) );
  m_String2Code.insert( PairString2Code("KEY_6",										KEY_6) );
  m_String2Code.insert( PairString2Code("KEY_7",										KEY_7) );
  m_String2Code.insert( PairString2Code("KEY_8",										KEY_8) );
  m_String2Code.insert( PairString2Code("KEY_9",										KEY_9) );
  m_String2Code.insert( PairString2Code("KEY_0",										KEY_0) );
  m_String2Code.insert( PairString2Code("KEY_F1",										KEY_F1) );
  m_String2Code.insert( PairString2Code("KEY_F2",										KEY_F2) );
  m_String2Code.insert( PairString2Code("KEY_F3",										KEY_F3) );
  m_String2Code.insert( PairString2Code("KEY_F4",										KEY_F4) );
  m_String2Code.insert( PairString2Code("KEY_F5",										KEY_F5) );
  m_String2Code.insert( PairString2Code("KEY_F6",										KEY_F6) );
  m_String2Code.insert( PairString2Code("KEY_F7",										KEY_F7) );
  m_String2Code.insert( PairString2Code("KEY_F8",										KEY_F8) );
  m_String2Code.insert( PairString2Code("KEY_F9",										KEY_F9) );
  m_String2Code.insert( PairString2Code("KEY_F10",									KEY_F10) );
  m_String2Code.insert( PairString2Code("KEY_F11",									KEY_F11) );
  m_String2Code.insert( PairString2Code("KEY_F12",									KEY_F12) );
  m_String2Code.insert( PairString2Code("KEY_NPPLUS",								KEY_NPPLUS) );
  m_String2Code.insert( PairString2Code("KEY_NPMINUS",							KEY_NPMINUS) );
  m_String2Code.insert( PairString2Code("KEY_NPDECIMAL",						KEY_NPDECIMAL) );
  m_String2Code.insert( PairString2Code("KEY_NPCOMMA",							KEY_NPCOMMA) );
  m_String2Code.insert( PairString2Code("KEY_NPDIVIDE",							KEY_NPDIVIDE) );
  m_String2Code.insert( PairString2Code("KEY_NPMULTIPLY",						KEY_NPMULTIPLY) );
  m_String2Code.insert( PairString2Code("KEY_NPENTER",							KEY_NPENTER) );
  m_String2Code.insert( PairString2Code("KEY_NUMLOCK",							KEY_NUMLOCK) );
  m_String2Code.insert( PairString2Code("KEY_NP1",									KEY_NP1) );
  m_String2Code.insert( PairString2Code("KEY_NP2",									KEY_NP2) );
  m_String2Code.insert( PairString2Code("KEY_NP3",									KEY_NP3) );
  m_String2Code.insert( PairString2Code("KEY_NP4",									KEY_NP4) );
  m_String2Code.insert( PairString2Code("KEY_NP5",									KEY_NP5) );
  m_String2Code.insert( PairString2Code("KEY_NP6",									KEY_NP6) );
  m_String2Code.insert( PairString2Code("KEY_NP7",									KEY_NP7) );
  m_String2Code.insert( PairString2Code("KEY_NP8",									KEY_NP8) );
  m_String2Code.insert( PairString2Code("KEY_NP9",									KEY_NP9) );
  m_String2Code.insert( PairString2Code("KEY_NP0",									KEY_NP0) );
  m_String2Code.insert( PairString2Code("KEY_RSHIFT",								KEY_RSHIFT) );
  m_String2Code.insert( PairString2Code("KEY_LSHIFT",								KEY_LSHIFT) );
  m_String2Code.insert( PairString2Code("KEY_RCTRL",								KEY_RCTRL) );
  m_String2Code.insert( PairString2Code("KEY_LCTRL",								KEY_LCTRL) );
  m_String2Code.insert( PairString2Code("KEY_RALT",									KEY_RALT) );
  m_String2Code.insert( PairString2Code("KEY_LALT",									KEY_LALT) );
  m_String2Code.insert( PairString2Code("KEY_LWIN",									KEY_LWIN) );
  m_String2Code.insert( PairString2Code("KEY_RWIN",									KEY_RWIN) );
  m_String2Code.insert( PairString2Code("KEY_UP",										KEY_UP) );
  m_String2Code.insert( PairString2Code("KEY_DOWN",									KEY_DOWN) );
  m_String2Code.insert( PairString2Code("KEY_LEFT",									KEY_LEFT) );
  m_String2Code.insert( PairString2Code("KEY_RIGHT",								KEY_RIGHT) );
  m_String2Code.insert( PairString2Code("KEY_INSERT",								KEY_INSERT) );
  m_String2Code.insert( PairString2Code("KEY_DELETE",								KEY_DELETE) );
  m_String2Code.insert( PairString2Code("KEY_HOME",									KEY_HOME) );
  m_String2Code.insert( PairString2Code("KEY_END",									KEY_END) );
  m_String2Code.insert( PairString2Code("KEY_PGDOWN",								KEY_PGDOWN) );
  m_String2Code.insert( PairString2Code("KEY_PGUP",									KEY_PGUP) );
  m_String2Code.insert( PairString2Code("KEY_PAUSE",								KEY_PAUSE) );
  m_String2Code.insert( PairString2Code("KEY_SCROLL",								KEY_SCROLL) );
}
