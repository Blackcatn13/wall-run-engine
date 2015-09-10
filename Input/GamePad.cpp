#include "GamePad.h"

bool CGamePad::Init(const Vect2i &screenRes) {
  m_bIsOk = true;
  m_ScreenResolution = screenRes;
  for ( uint32 i = 0; i < MAX_CONTROLLERS; i++ ) {
    m_Controllers[i].m_bConnected									= false;
    m_Controllers[i].m_vibration.wLeftMotorSpeed	= 0;
    m_Controllers[i].m_vibration.wRightMotorSpeed	= 0;
    m_Controllers[i].m_Pos.x											= 0;
    m_Controllers[i].m_Pos.y											= 0;
    m_Controllers[i].m_wNewButtons								= 0;
    m_Controllers[i].m_wOldButtons								= 0;
    m_Controllers[i].m_CurrentDeltaTriggers       = v2iZERO;
    m_Controllers[i].m_LastDeltaTriggers          = v2iZERO;
  }
  return m_bIsOk;

}

void CGamePad::Update() {
  for ( uint32 i = 0; i < MAX_CONTROLLERS; i++ ) {
    m_Controllers[i].m_bConnected = (XInputGetState(i, &m_Controllers[i].m_pState) == ERROR_SUCCESS);
    if (m_Controllers[i].m_bConnected) {
      if (m_bDeadZoneOn) {
        // Zero value if thumbsticks are within the dead zone
        if ( m_Controllers[i].m_pState.Gamepad.sThumbLX < INPUT_DEADZONE && m_Controllers[i].m_pState.Gamepad.sThumbLX > -INPUT_DEADZONE ) {
          m_Controllers[i].m_pState.Gamepad.sThumbLX = 0;
        }
        if ( m_Controllers[i].m_pState.Gamepad.sThumbLY < INPUT_DEADZONE && m_Controllers[i].m_pState.Gamepad.sThumbLY > -INPUT_DEADZONE ) {
          m_Controllers[i].m_pState.Gamepad.sThumbLY = 0;
        }
        if ( m_Controllers[i].m_pState.Gamepad.sThumbRX < INPUT_DEADZONE && m_Controllers[i].m_pState.Gamepad.sThumbRX > -INPUT_DEADZONE ) {
          m_Controllers[i].m_pState.Gamepad.sThumbRX = 0;
        }
        if ( m_Controllers[i].m_pState.Gamepad.sThumbRY < INPUT_DEADZONE && m_Controllers[i].m_pState.Gamepad.sThumbRY > -INPUT_DEADZONE ) {
          m_Controllers[i].m_pState.Gamepad.sThumbRY = 0;
        }
      }
      m_Controllers[i].m_Pos.x += (uint32)(m_Controllers[i].m_pState.Gamepad.sThumbRX * 0.0001f);
      if (m_Controllers[i].m_Pos.x < 0)
        m_Controllers[i].m_Pos.x = 0;
      else if (m_Controllers[i].m_Pos.x > m_ScreenResolution.x)
        m_Controllers[i].m_Pos.x = m_ScreenResolution.x;
      m_Controllers[i].m_Pos.y -= (uint32)(m_Controllers[i].m_pState.Gamepad.sThumbRY * 0.0001f);
      if (m_Controllers[i].m_Pos.y < 0)
        m_Controllers[i].m_Pos.y = 0;
      else if (m_Controllers[i].m_Pos.y > m_ScreenResolution.y)
        m_Controllers[i].m_Pos.y = m_ScreenResolution.y;
      m_Controllers[i].m_wOldButtons = m_Controllers[i].m_wNewButtons;
      m_Controllers[i].m_wNewButtons = m_Controllers[i].m_pState.Gamepad.wButtons;
      m_Controllers[i].m_LastDeltaTriggers = m_Controllers[i].m_CurrentDeltaTriggers;
      m_Controllers[i].m_CurrentDeltaTriggers = Vect2i(m_Controllers[i].m_pState.Gamepad.bLeftTrigger, m_Controllers[i].m_pState.Gamepad.bRightTrigger);
      XInputSetState( 0, &m_Controllers[i].m_vibration );
    }
  }
}

bool CGamePad::IsConnected (uint32 idController)	const {
  if (idController < MAX_CONTROLLERS) {
    return m_Controllers[idController].m_bConnected;
  } else {
    return false;
  }
}

void CGamePad::SetLeftMotorSpeed (uint32 speed, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    //speed = [0-65535]
    if (speed > 65535) {
      speed = 65535;
    }
    m_Controllers[idController].m_vibration.wLeftMotorSpeed = speed;
  }
}

void CGamePad::SetRightMotorSpeed (uint32 speed, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    //speed = [0-65535]
    if (speed > 65535) {
      speed = 65535;
    }
    m_Controllers[idController].m_vibration.wRightMotorSpeed = speed;
  }
}

void CGamePad::GetMotorSpeed (uint32 &left, uint32 &right, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    right	= (uint32) m_Controllers[idController].m_vibration.wRightMotorSpeed;
    left	= (uint32) m_Controllers[idController].m_vibration.wLeftMotorSpeed;
  }
}

bool CGamePad::IsDown(uint32 nBtn, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    if (m_Controllers[idController].m_bConnected) {
      return ((m_Controllers[idController].m_wNewButtons & nBtn) == nBtn);
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool CGamePad::IsDownUp(uint32 nBtn, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    if (m_Controllers[idController].m_bConnected) {
      return (((m_Controllers[idController].m_wOldButtons & nBtn) == nBtn) && !((m_Controllers[idController].m_wNewButtons & nBtn) == nBtn));
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool CGamePad::IsUpDown(uint32 nBtn, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    if (m_Controllers[idController].m_bConnected) {
      return ((!((m_Controllers[idController].m_wOldButtons & nBtn) == nBtn)) && ((m_Controllers[idController].m_wNewButtons & nBtn) == nBtn));
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool CGamePad::GetLeftThumbDeflection(float *pfX, float *pfY, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    (*pfX) = m_Controllers[idController].m_pState.Gamepad.sThumbLX;
    (*pfY) = m_Controllers[idController].m_pState.Gamepad.sThumbLY;
    return true;
  } else {
    return false;
  }
}

bool CGamePad::GetRightThumbDeflection(float *pfX, float *pfY, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    if (m_Controllers[idController].m_bConnected) {
      (*pfX) = m_Controllers[idController].m_pState.Gamepad.sThumbRX;
      (*pfY) = m_Controllers[idController].m_pState.Gamepad.sThumbRY;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool CGamePad::GetDeltaTriggers(float *pfLeft, float *pfRight, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    if (m_Controllers[idController].m_bConnected) {
      (*pfLeft) = m_Controllers[idController].m_pState.Gamepad.bLeftTrigger;
      (*pfRight) = m_Controllers[idController].m_pState.Gamepad.bRightTrigger;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool CGamePad::GetTriggerIsDown(uint32 nTrigger, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    if (m_Controllers[idController].m_bConnected) {
      if (nTrigger == 7) { // Right
        if (m_Controllers[idController].m_CurrentDeltaTriggers.y > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
          return true;
        else
          return false;
      } else {
        if (m_Controllers[idController].m_CurrentDeltaTriggers.x > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
          return true;
        else
          return false;
      }
    } else
      return false;
  } else
    return false;
}

bool CGamePad::GetTriggerIsDownUp(uint32 nTrigger, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    if (m_Controllers[idController].m_bConnected) {
      if (nTrigger == 7) { // Right
        if (m_Controllers[idController].m_CurrentDeltaTriggers.y < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && m_Controllers[idController].m_LastDeltaTriggers.y > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
          return true;
        else
          return false;
      } else {
        if (m_Controllers[idController].m_CurrentDeltaTriggers.x < XINPUT_GAMEPAD_TRIGGER_THRESHOLD && m_Controllers[idController].m_LastDeltaTriggers.x > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
          return true;
        else
          return false;
      }
    } else
      return false;
  } else
    return false;
}

bool CGamePad::GetTriggerIsUpDown(uint32 nTrigger, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    if (m_Controllers[idController].m_bConnected) {
      if (nTrigger == 7) { // Right
        if (m_Controllers[idController].m_CurrentDeltaTriggers.y > XINPUT_GAMEPAD_TRIGGER_THRESHOLD && m_Controllers[idController].m_LastDeltaTriggers.y < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
          return true;
        else
          return false;
      } else {
        if (m_Controllers[idController].m_CurrentDeltaTriggers.x > XINPUT_GAMEPAD_TRIGGER_THRESHOLD && m_Controllers[idController].m_LastDeltaTriggers.x < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
          return true;
        else
          return false;
      }
    } else
      return false;
  } else
    return false;
}

uint32 CGamePad::GetButtonsPressed (uint32 idController)	const {
  if (idController < MAX_CONTROLLERS) {
    return m_Controllers[idController].m_wOldButtons;
  } else {
    return 0;
  }
}
void CGamePad::GetPosition (Vect2i &pos, uint32 idController) {
  if (idController < MAX_CONTROLLERS) {
    pos = m_Controllers[idController].m_Pos;
  }
}