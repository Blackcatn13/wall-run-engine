
#include "Mouse.h"
#include "Utils/Logger.h"



#define BUFFER_SIZE 16

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CMouse::Done ()
{
    LOGGER->AddNewLog(ELL_INFORMATION, "Mouse:: releasing mouse");
    if (IsOk()) {
        Release();
        Inherited::Done();
    }
}

/**
* Initializes the mouse device
*/
bool CMouse::Init(LPDIRECTINPUT8 pDI, HWND hWnd, const Vect2i& screenRes, bool exclusiveMode)
{
    bool bIsOk = Inherited::Init(pDI, hWnd);
    m_ScreenResolution = screenRes;
    if (bIsOk) {
        // clear out structs for mouse buttons
        memset(m_bIsDown,  0, sizeof(bool) * 3);
        memset(m_bIsUpDown, 0, sizeof(bool) * 3);
        memset(m_bIsDownUp, 0, sizeof(bool) * 3);
        m_Pos.x = m_Pos.y = 0;
        LOGGER->AddNewLog(ELL_INFORMATION, "Mouse:: crancking up mouse");
        bIsOk = !FAILED(CrankUp(GUID_SysMouse, &c_dfDIMouse, exclusiveMode));
        if (bIsOk) {
            // activate event notification for the mouse
            m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
            bIsOk = m_hEvent != NULL;
            if (bIsOk) {
                bIsOk = !FAILED( m_pDevice->SetEventNotification(m_hEvent));
                if (bIsOk) {
                    // create buffered input for the mouse
                    DIPROPDWORD dipdw;
                    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
                    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
                    dipdw.diph.dwObj        = 0;
                    dipdw.diph.dwHow        = DIPH_DEVICE;
                    dipdw.dwData            = BUFFER_SIZE;
                    bIsOk = !FAILED( m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph));
                    if (bIsOk) {
                        // acquire the device to make it work
                        m_pDevice->Acquire();
                        LOGGER->AddNewLog(ELL_INFORMATION, "Mouse:: mouse online");
                    }
                }
            }
        }
    }
    if (!bIsOk) {
        Done();
    }
    return bIsOk;
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CMouse::Release ()
{
}

/**
* Update all input devices
*/
HRESULT CMouse::Update(void)
{
    DIDEVICEOBJECTDATA od[BUFFER_SIZE];
    DWORD dwNumElem = BUFFER_SIZE;
    m_Delta.x = m_Delta.y = m_Delta.z = 0;
    // try to get the data from the mouse
    if (FAILED(GetData(IDV_MOUSE, &od[0], &dwNumElem))) {
        //Log("InputManager ERROR:: GetData(Mouse) failed");
        return E_FAIL;
    }
    // clear those
    m_bIsDownUp[0] = m_bIsDownUp[1] = m_bIsDownUp[2] = false;
    m_bIsUpDown[0] = m_bIsUpDown[1] = m_bIsUpDown[2] = false;
    // now we have 'dwNumElem' of mouse events
    for (DWORD i = 0; i < dwNumElem; i++) {
        switch (od[i].dwOfs) {
        // MOVEMENT
        case DIMOFS_X: {
                m_Pos.x += od[i].dwData;
                m_Delta.x = od[i].dwData;
                if (m_Pos.x < 0)
                    m_Pos.x = 0;
                else if (m_Pos.x > m_ScreenResolution.x)
                    m_Pos.x = m_ScreenResolution.x;
            }
            break;
        case DIMOFS_Y: {
                m_Pos.y += od[i].dwData;
                m_Delta.y = od[i].dwData;
                if (m_Pos.y < 0)
                    m_Pos.y = 0;
                else if (m_Pos.y > m_ScreenResolution.y)
                    m_Pos.y = m_ScreenResolution.y;
            }
            break;
        case DIMOFS_Z: {
                m_Delta.z = od[i].dwData;
            }
            break;
        // BUTTON STATES
        case DIMOFS_BUTTON0: {
                if (od[i].dwData & 0x80) {
                    if (!m_bIsDown[0])
                        m_bIsUpDown[0] = true;
                    m_bIsDown[0] = true;
                } else {
                    if (m_bIsDown[0])
                        m_bIsDownUp[0] = true;
                    m_bIsDown[0] = false;
                }
            }
            break;
        case DIMOFS_BUTTON1: {
                if (od[i].dwData & 0x80) {
                    if (!m_bIsDown[1])
                        m_bIsUpDown[1] = true;
                    m_bIsDown[1] = true;
                } else {
                    if (m_bIsDown[1])
                        m_bIsDownUp[1] = true;
                    m_bIsDown[1] = false;
                }
            }
            break;
        case DIMOFS_BUTTON2: {
                if (od[i].dwData & 0x80) {
                    if (!m_bIsDown[2])
                        m_bIsUpDown[2] = true;
                    m_bIsDown[2] = true;
                } else {
                    if (m_bIsDown[2])
                        m_bIsDownUp[2] = true;
                    m_bIsDown[2] = false;
                }
            }
            break;
        }; // END switch (od[i].dwOfs)
    } // END for (DWORD i=0; i<dwNumElem; i++)
    return S_OK;
}


