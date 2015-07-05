#ifndef CACTION_TO_INPUT_H
#define CACTION_TO_INPUT_H

#include <map>
#include <vector>
#include "Utils\Types.h"
#include "InputDefs.h"

class CInputManager;

typedef std::pair<std::string, uint32> PairString2Code;
typedef struct _action_info {
  INPUT_DEVICE_TYPE   deviceType;
  INPUT_EVENT_TYPE    EventType;
  uint32              Code;
  INPUT_AXIS_TYPE     axisType;
} action_info;

typedef std::map<std::string, std::vector<action_info> > MapString2Actions;
typedef std::map<std::string, uint32> MapString2Code;
typedef std::vector<action_info> VecInfoInputs;

class CActionToInput {
 private:
  MapString2Code          m_String2Code;
  std::string             m_fileName;
  MapString2Actions       m_GlobalString2Actions;
  MapString2Actions       m_MouseKeyboardString2Actions;
  MapString2Actions       m_GamePadString2Actions;
  CInputManager          *m_InputManager;
 public:
  CActionToInput(CInputManager *input);
  bool	LoadXML (const std::string &xmlFile);
  bool	ReloadXML ();
  //bool	SaveXML (const std::string& xmlFile);

  //void	SetAction (const std::string& action_name, VecInfoInputs& vecInfoInput);

  bool	DoAction (const std::string &action_name, float &amount);
  bool	DoAction (const std::string &action_name );
  void GetActionInfo(std::string actionName, std::string &strInfo);

  void    CActionToInput::InitString2Input ();

  bool DoActionFromLua(const std::string action_name);
  bool DoActionFromLua (const std::string action_name, float amount);
};

#endif