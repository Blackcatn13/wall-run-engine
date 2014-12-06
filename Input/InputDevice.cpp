#include "InputDevice.h"
#include "Utils/Logger.h"

/**
* Set basic attributes to pointers.
*/
bool CInputDevice::Init(LPDIRECTINPUT8 pDI, HWND hWnd)
{
	if (pDI)
	{
		m_hWnd =  hWnd;
		m_pDI = pDI;
		m_bIsOk = true;
	}

	if (!m_bIsOk)
	{
		Release();
	}

	return m_bIsOk;
}

void CInputDevice::Done()
{
	if (IsOk())
	{
		Release();

		m_bIsOk = false;
	}
}


/**
* Release the object.
*/ 
void CInputDevice::Release( void )
{
	if (m_pDevice)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	LOGGER->AddNewLog(ELL_INFORMATION, "InputDevice::nput device offline (ok)");
} 

/**
* Call all stuff DirectInput needs to initialize an input device.
*/ 
HRESULT CInputDevice::CrankUp(REFGUID rguid, LPCDIDATAFORMAT pdf, bool exclusiveMode)
{

	DWORD dwFlags = DISCL_FOREGROUND;

	if (exclusiveMode)
	{
		dwFlags |= DISCL_EXCLUSIVE;
	}
	else
	{
		dwFlags |= DISCL_NONEXCLUSIVE;
	}

	// if device is already build destroy it
	if (m_pDevice) {
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// 1. Step: create device
	if ( FAILED(m_pDI->CreateDevice(rguid, &m_pDevice, NULL))) {
		LOGGER->AddNewLog(ELL_ERROR, "InputDevice: CreateDevice failed");
		return S_FALSE; 
	} 

	// set the correct device data format
	if ( FAILED(m_pDevice->SetDataFormat(pdf))) {
		LOGGER->AddNewLog(ELL_ERROR, "InputDevice: SetDataFormat failed");
		return S_FALSE; 
	}

	// set the cooperation level with windows
	if ( FAILED(m_pDevice->SetCooperativeLevel(m_hWnd, dwFlags))) {
		LOGGER->AddNewLog(ELL_ERROR, "InputDevice: SetCoopLevel failed");
		return S_FALSE; 
	}

	return S_OK;
}


/**
* Get the state or data from the device object.
* -> IN: ZFXINPUTDEV - keyboard, mouse or joystick
*        void*       - stores the data
*        DWORD*      - stores a counter (mouse only)
*/
HRESULT CInputDevice::GetData(INPUT_DEVICE_TYPE Type, void *pData,DWORD *pdwNum) 
{
	HRESULT hr=S_FALSE;
	size_t size=0;

	// is this a mouse?
	if (Type == IDV_MOUSE) 
	{
		size = sizeof(DIDEVICEOBJECTDATA);

		hr = m_pDevice->GetDeviceData((DWORD)size, (DIDEVICEOBJECTDATA*)pData, pdwNum, 0);
	}
	else 
	{
		if (Type==IDV_KEYBOARD) size = sizeof(char)*256;
		else size = sizeof(DIJOYSTATE);

		hr = m_pDevice->GetDeviceState((DWORD)size, pData);
	}


	if (FAILED(hr)) 
	{
		// if lost or not yet acquired then acquire it at all costs
		if ( (hr==DIERR_NOTACQUIRED) || (hr==DIERR_INPUTLOST) ) {
			hr = m_pDevice->Acquire();
			while (hr==DIERR_INPUTLOST)
				hr = m_pDevice->Acquire();

			// if another application is using this input device
			// we have to give up and try next frame
			if (hr==DIERR_OTHERAPPHASPRIO) return S_OK;

			// if we got back device then try again to read data
			if (SUCCEEDED(hr)) {
				if (Type == IDV_MOUSE)
					hr = m_pDevice->GetDeviceData((DWORD)size, (DIDEVICEOBJECTDATA*)
					pData, pdwNum, 0);
				else 
					hr = m_pDevice->GetDeviceState((DWORD)size, pData);
			}
			if (FAILED(hr)) return S_FALSE;
		}
		else return S_FALSE;
	}
	return S_OK;
} // GetData
/*----------------------------------------------------------------*/
