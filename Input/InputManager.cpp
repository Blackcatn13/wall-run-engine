#include "InputManager.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Utils/Defines.h"
#include "Utils/Logger.h"
#include "Utils/Exception.h"




//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CInputManager::Done () {
  if (IsOk()) {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CInputManager::Release () {
  LOGGER->AddNewLog(ELL_INFORMATION, "InputManager:: shutting down DirectInput");
  CHECKED_DELETE(m_pKB);
  CHECKED_DELETE(m_pMouse);
  CHECKED_DELETE(m_pGamePad);
  //CHECKED_RELEASE(m_pDI);
  LOGGER->AddNewLog(ELL_INFORMATION, "InputManager:: offline (ok)");
}


//----------------------------------------------------------------------------
// Initializes at least keyboard and mouse, otherwise returns error.
// If present joystick will also be initialized, but is not mandatory.
//-> IN: HWND      - handle to main application window
//----------------------------------------------------------------------------
bool CInputManager::Init (HWND hWnd, const Vect2i &screenRes, bool exclusiveModeinMouse) {
  HRESULT hr;
  m_hWndMain = hWnd;
  LOGGER->AddNewLog(ELL_INFORMATION, "InputManager:: calling initialization");
  // create main DirectInput object
  m_bIsOk = !FAILED (hr = DirectInput8Create(	GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                          IID_IDirectInput8, (void **)&m_pDI, NULL));
  if (m_bIsOk) {
    // create all input device objects
    m_pKB				= new CKeyboard;
    m_pMouse		= new CMouse;
    m_pGamePad	= new CGamePad;
    // initialize all input device objects
    m_bIsOk = m_pKB->Init(m_pDI, hWnd);
    if (m_bIsOk) {
      m_bIsOk	= m_pMouse->Init(m_pDI, hWnd, screenRes, exclusiveModeinMouse);
      if (m_bIsOk) {
        m_bIsOk = m_pGamePad->Init(screenRes);
        m_pGamePad->Update();
        if (m_pGamePad->IsConnected() ) {
          LOGGER->AddNewLog(ELL_INFORMATION, "InputManager:: GamePad is connected");
        } else {
          LOGGER->AddNewLog(ELL_INFORMATION, "InputManager:: GamePad is not connected");
        }
      }
    }
  }
  if (!m_bIsOk) {
    //Release();
    std::string msg_error = "Error al inicializar directinput";
    throw CException(__FILE__, __LINE__, msg_error);
  }
  return m_bIsOk;
}



//----------------------------------------------------------------------------
// Query if a Joystick is active and ready or not. Also returns
// Joytsicks name if any was found.
//----------------------------------------------------------------------------
bool CInputManager::HasGamePad (INPUT_DEVICE_TYPE device) const {
  switch (device) {
    case IDV_GAMEPAD1:
      return m_pGamePad->IsConnected(0);
      break;
    case IDV_GAMEPAD2:
      return m_pGamePad->IsConnected(1);
      break;
    case IDV_GAMEPAD3:
      return m_pGamePad->IsConnected(2);
      break;
    case IDV_GAMEPAD4:
      return m_pGamePad->IsConnected(3);
      break;
    default:
      return false;
  }
}



//----------------------------------------------------------------------------
// Update all input devices
//----------------------------------------------------------------------------
HRESULT CInputManager::Update(void) {
  HRESULT hr;
  if (!IsOk()) return E_FAIL;
  if (m_pKB) {
    if ( FAILED( hr = m_pKB->Update() ) )
      return hr;
  }
  if (m_pMouse) {
    if ( FAILED( hr = m_pMouse->Update() ) )
      return hr;
  }
  if (m_pGamePad) {
    m_pGamePad->Update();
  }
  return S_OK;
}

//----------------------------------------------------------------------------
// If mouse or joystick return current position.
//----------------------------------------------------------------------------
HRESULT CInputManager::GetPosition( INPUT_DEVICE_TYPE idType, Vect2i &pos) {
  if (!m_useFakePosition) {
    switch (idType) {
      case IDV_MOUSE:
        //pos = m_pMouse->GetPosition();
        POINT p;
        if (GetCursorPos(&p)) {
          //cursor position now in p.x and p.y
          pos = Vect2i(p.x, p.y);
        }
        return S_OK;
        break;
      case IDV_GAMEPAD1:
        m_pGamePad->GetPosition(pos, 0);
        return true;
        break;
      case IDV_GAMEPAD2:
        m_pGamePad->GetPosition(pos, 1);
        return true;
        break;
      case IDV_GAMEPAD3:
        m_pGamePad->GetPosition(pos, 2);
        return true;
        break;
      case IDV_GAMEPAD4:
        m_pGamePad->GetPosition(pos, 3);
        return true;
        break;
      default:
        return E_FAIL;
    }
  } else {
    pos = m_fakePosition;
    return true;
  }
}


//----------------------------------------------------------------------------
// If GamePad return current intensity on axes.
//----------------------------------------------------------------------------
bool CInputManager::GetGamePadLeftThumbDeflection(float *pfX, float *pfY, INPUT_DEVICE_TYPE device) {
  switch (device) {
    case IDV_GAMEPAD1:
      return m_pGamePad->GetLeftThumbDeflection(pfX, pfY, 0);
      break;
    case IDV_GAMEPAD2:
      return m_pGamePad->GetLeftThumbDeflection(pfX, pfY, 1);
      break;
    case IDV_GAMEPAD3:
      return m_pGamePad->GetLeftThumbDeflection(pfX, pfY, 2);
      break;
    case IDV_GAMEPAD4:
      return m_pGamePad->GetLeftThumbDeflection(pfX, pfY, 3);
      break;
    default:
      return false;
  }
}

bool CInputManager::GetGamePadRightThumbDeflection(float *pfX, float *pfY, INPUT_DEVICE_TYPE device) {
  switch (device) {
    case IDV_GAMEPAD1:
      return m_pGamePad->GetRightThumbDeflection(pfX, pfY, 0);
      break;
    case IDV_GAMEPAD2:
      return m_pGamePad->GetRightThumbDeflection(pfX, pfY, 1);
      break;
    case IDV_GAMEPAD3:
      return m_pGamePad->GetRightThumbDeflection(pfX, pfY, 2);
      break;
    case IDV_GAMEPAD4:
      return m_pGamePad->GetRightThumbDeflection(pfX, pfY, 3);
      break;
    default:
      return false;
  }
}

bool CInputManager::GetGamePadDeltaTriggers(float *pfLeft, float *pfRight, INPUT_DEVICE_TYPE device) {
  switch (device) {
    case IDV_GAMEPAD1:
      return m_pGamePad->GetDeltaTriggers(pfLeft, pfRight, 0);
      break;
    case IDV_GAMEPAD2:
      return m_pGamePad->GetDeltaTriggers(pfLeft, pfRight, 1);
      break;
    case IDV_GAMEPAD3:
      return m_pGamePad->GetDeltaTriggers(pfLeft, pfRight, 2);
      break;
    case IDV_GAMEPAD4:
      return m_pGamePad->GetDeltaTriggers(pfLeft, pfRight, 3);
      break;
    default:
      return false;
  }
}

void	CInputManager::SetGamePadLeftMotorSpeed (uint32 speed, INPUT_DEVICE_TYPE device) {
  switch (device) {
    case IDV_GAMEPAD1:
      m_pGamePad->SetLeftMotorSpeed(speed, 0);
      break;
    case IDV_GAMEPAD2:
      m_pGamePad->SetLeftMotorSpeed(speed, 1);
      break;
    case IDV_GAMEPAD3:
      m_pGamePad->SetLeftMotorSpeed(speed, 2);
      break;
    case IDV_GAMEPAD4:
      m_pGamePad->SetLeftMotorSpeed(speed, 3);
      break;
  }
}

void	CInputManager::SetGamePadRightMotorSpeed (uint32 speed, INPUT_DEVICE_TYPE device) {
  switch (device) {
    case IDV_GAMEPAD1:
      m_pGamePad->SetRightMotorSpeed(speed, 0);
      break;
    case IDV_GAMEPAD2:
      m_pGamePad->SetRightMotorSpeed(speed, 1);
      break;
    case IDV_GAMEPAD3:
      m_pGamePad->SetRightMotorSpeed(speed, 2);
      break;
    case IDV_GAMEPAD4:
      m_pGamePad->SetRightMotorSpeed(speed, 3);
      break;
  }
}

//----------------------------------------------------------------------------
// Return the change of mouse cursor since last call to Update().
//----------------------------------------------------------------------------
const Vect3i &CInputManager::GetMouseDelta(void) {
  return m_pMouse->GetMouseDelta();
}


//----------------------------------------------------------------------------
// Ask about button state.
//----------------------------------------------------------------------------
bool CInputManager::IsDown(INPUT_DEVICE_TYPE idType, uint32 nBtn) {
  bool result = false;
  switch (idType) {
    case IDV_MOUSE:
      result = m_pMouse->IsDown(nBtn);
      break;
    case IDV_KEYBOARD:
      result = m_pKB->IsDown(nBtn);
      break;
    case IDV_GAMEPAD1:
      result = m_pGamePad->IsDown(nBtn, 0);
      break;
    case IDV_GAMEPAD2:
      result = m_pGamePad->IsDown(nBtn, 1);
      break;
    case IDV_GAMEPAD3:
      result =  m_pGamePad->IsDown(nBtn, 2);
      break;
    case IDV_GAMEPAD4:
      result = m_pGamePad->IsDown(nBtn, 3);
      break;
  }
  return result;
}

//----------------------------------------------------------------------------
// Ask about button/key state for mouse, joytsick or keyboard.
//----------------------------------------------------------------------------
bool CInputManager::IsDownUp(INPUT_DEVICE_TYPE idType, uint32 nBtn) {
  bool result = false;
  switch (idType) {
    case IDV_MOUSE:
      result = m_pMouse->IsDownUp(nBtn);
      break;
    case IDV_KEYBOARD:
      result = m_pKB->IsDownUp(nBtn);
      break;
    case IDV_GAMEPAD1:
      result = m_pGamePad->IsDownUp(nBtn, 0);
      break;
    case IDV_GAMEPAD2:
      result = m_pGamePad->IsDownUp(nBtn, 1);
      break;
    case IDV_GAMEPAD3:
      result = m_pGamePad->IsDownUp(nBtn, 2);
      break;
    case IDV_GAMEPAD4:
      result = m_pGamePad->IsDownUp(nBtn, 3);
      break;
  }
  return result;
}

//----------------------------------------------------------------------------
// Ask about button/key state for mouse, joytsick or keyboard.
//----------------------------------------------------------------------------
bool CInputManager::IsUpDown(INPUT_DEVICE_TYPE idType, uint32 nBtn) {
  bool result = false;
  switch (idType) {
    case IDV_MOUSE:
      result = m_pMouse->IsUpDown(nBtn);
      break;
    case IDV_KEYBOARD:
      result = m_pKB->IsUpDown(nBtn);
      break;
    case IDV_GAMEPAD1:
      result = m_pGamePad->IsUpDown(nBtn, 0);
      break;
    case IDV_GAMEPAD2:
      result = m_pGamePad->IsUpDown(nBtn, 1);
      break;
    case IDV_GAMEPAD3:
      result = m_pGamePad->IsUpDown(nBtn, 2);
      break;
    case IDV_GAMEPAD4:
      result = m_pGamePad->IsUpDown(nBtn, 3);
      break;
  }
  return result;
}


int32 CInputManager::Scan2ascii (uint32 scancode, uint16 *result) {
  static HKL layout = GetKeyboardLayout(0);
  static uint8 State[256];
  if (GetKeyboardState(State) == FALSE)
    return 0;
  uint32 vk = MapVirtualKeyEx(scancode, 1, layout);
  return ToAsciiEx(vk, scancode, State, result, 0, layout);
}

float CInputManager::GetGamePadLeftThumbXDeflection(INPUT_DEVICE_TYPE device) {
  float pfX, pfY;
  switch (device) {
    case IDV_GAMEPAD1:
      m_pGamePad->GetLeftThumbDeflection(&pfX, &pfY, 0);
      return pfX;
      break;
    case IDV_GAMEPAD2:
      m_pGamePad->GetLeftThumbDeflection(&pfX, &pfY, 1);
      return pfX;
      break;
    case IDV_GAMEPAD3:
      m_pGamePad->GetLeftThumbDeflection(&pfX, &pfY, 2);
      return pfX;
      break;
    case IDV_GAMEPAD4:
      m_pGamePad->GetLeftThumbDeflection(&pfX, &pfY, 3);
      return pfX;
      break;
    default:
      return 0.f;
  }
}

float CInputManager::GetGamePadLeftThumbYDeflection(INPUT_DEVICE_TYPE device) {
  float pfX, pfY;
  switch (device) {
    case IDV_GAMEPAD1:
      m_pGamePad->GetLeftThumbDeflection(&pfX, &pfY, 0);
      return pfY;
      break;
    case IDV_GAMEPAD2:
      m_pGamePad->GetLeftThumbDeflection(&pfX, &pfY, 1);
      return pfY;
      break;
    case IDV_GAMEPAD3:
      m_pGamePad->GetLeftThumbDeflection(&pfX, &pfY, 2);
      return pfY;
      break;
    case IDV_GAMEPAD4:
      m_pGamePad->GetLeftThumbDeflection(&pfX, &pfY, 3);
      return pfY;
      break;
    default:
      return 0.f;
  }
}

float CInputManager::GetGamePadLeftTriggerDelta(INPUT_DEVICE_TYPE device) {
  float leftT, rightT;
  switch (device) {
    case IDV_GAMEPAD1:
      m_pGamePad->GetDeltaTriggers(&leftT, &rightT, 0);
      return leftT;
      break;
    case IDV_GAMEPAD2:
      m_pGamePad->GetDeltaTriggers(&leftT, &rightT, 1);
      return leftT;
      break;
    case IDV_GAMEPAD3:
      m_pGamePad->GetDeltaTriggers(&leftT, &rightT, 2);
      return leftT;
      break;
    case IDV_GAMEPAD4:
      m_pGamePad->GetDeltaTriggers(&leftT, &rightT, 3);
      return leftT;
      break;
    default:
      return 0.f;
  }
}

float CInputManager::GetGamePadRightTriggerDelta(INPUT_DEVICE_TYPE device) {
  float leftT, rightT;
  switch (device) {
    case IDV_GAMEPAD1:
      m_pGamePad->GetDeltaTriggers(&leftT, &rightT, 0);
      return rightT;
      break;
    case IDV_GAMEPAD2:
      m_pGamePad->GetDeltaTriggers(&leftT, &rightT, 1);
      return rightT;
      break;
    case IDV_GAMEPAD3:
      m_pGamePad->GetDeltaTriggers(&leftT, &rightT, 2);
      return rightT;
      break;
    case IDV_GAMEPAD4:
      m_pGamePad->GetDeltaTriggers(&leftT, &rightT, 3);
      return rightT;
      break;
    default:
      return 0.f;
  }
}


