//----------------------------------------------------------------------------------
// CInputDevice class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el device del directInput comun a Mouse, Keyboard y Josytick
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_INPUT_DEVICE_H_
#define INC_INPUT_DEVICE_H_

#include <dinput.h>
#include "InputDefs.h"


class CInputDevice
{
public:
    CInputDevice() : m_bIsOk(false), m_pDevice(NULL), m_pDI(NULL) {}
    virtual ~CInputDevice()
    {
        Done();
    }

    // virtual methods for heirs
    bool                    Init	          (LPDIRECTINPUT8, HWND);
    void                    Done            ();
    bool                    IsOk            () const
    {
        return m_bIsOk;
    }

    virtual HRESULT         Update	        () = 0;

    // base class methods
    virtual HRESULT         CrankUp	        ( REFGUID rguid, LPCDIDATAFORMAT lpdf, bool exclusiveMode = false);

    // Accessor-Methods
    virtual const Vect2i&		GetPosition	    ()
    {
        return m_Pos;
    }


protected:
    virtual HRESULT         GetData	        ( INPUT_DEVICE_TYPE Type, void *pData, DWORD *dwNum);
    void                    Release          ();

protected:
    bool					          m_bIsOk;		    // Initialization boolean control
    LPDIRECTINPUTDEVICE8	  m_pDevice;
    LPDIRECTINPUT8			    m_pDI;
    HWND					          m_hWnd;
    Vect2i									m_Pos;
};

#endif //INC_INPUT_DEVICE_H_