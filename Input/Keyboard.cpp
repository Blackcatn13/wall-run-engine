

#include "Keyboard.h"
#include "Utils/Logger.h"


//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CKeyboard::Done ()
{
    LOGGER->AddNewLog(ELL_INFORMATION, "Keyboard:: releasing keyboard");
    if (IsOk()) {
        Release();
        m_bIsOk = false;
    }
}



/**
* Initializes the keyboard device.
*/
bool CKeyboard::Init(LPDIRECTINPUT8 pDI, HWND hWnd)
{
    bool bIsOk = Inherited::Init(pDI, hWnd);
    if (bIsOk) {
        LOGGER->AddNewLog(ELL_INFORMATION, "Keyboard:: crancking up keyboard");
        bIsOk = !FAILED(CrankUp(GUID_SysKeyboard, &c_dfDIKeyboard));
        if (bIsOk) {
            // clear out the structs
            memset(m_Keys, 0, sizeof(m_Keys));
            memset(m_KeysOld, 0, sizeof(m_KeysOld));
            // acquire the device to make it work
            m_pDevice->Acquire();
            LOGGER->AddNewLog(ELL_INFORMATION, "Keyboard:: keyboard online");
        }
    }
    if (!bIsOk) {
        Done();
    }
    return bIsOk;
}

/**
* Update the keyboard device.
*/
HRESULT CKeyboard::Update(void)
{
    // copy keyboard state from last frame
    memcpy(m_KeysOld, m_Keys, sizeof(m_Keys));
    // try to get the data from the keyboard
    if (FAILED(GetData(IDV_KEYBOARD, &m_Keys[0], NULL))) {
        LOGGER->AddNewLog(ELL_ERROR, "Keyboard:: GetData(Keyboard) failed");
        return E_FAIL;
    }
    return S_OK;
}


/**
* Check if the given key is pressed. Use WinAPI VK_ defines here.
*/
bool CKeyboard::IsDown(uint32 nID) const
{
    // translate virtual code to scan code
    if (m_Keys[nID] & 0x80)
        return true;
    return false;
} // IsPressed

/**
* Check if the given key is released. Use WinAPI VK_ defines here.
*/
bool CKeyboard::IsDownUp(uint32 nID)  const
{
    // translate virtual code to scan code
    if ( (m_KeysOld[nID] & 0x80) && !(m_Keys[nID] & 0x80) )
        return true;
    return false;
}

/**
* Check if the given key is pressed and before wasn't. Use WinAPI VK_ defines here.
*/
bool CKeyboard::IsUpDown(uint32 nID) const
{
    // translate virtual code to scan code
    if ( !(m_KeysOld[nID] & 0x80) && (m_Keys[nID] & 0x80) )
        return true;
    else return false;
}
