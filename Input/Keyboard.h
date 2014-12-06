//----------------------------------------------------------------------------------
// CKeyboard class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el input de tipo keyboard
// Internamente utiliza la librería DirectInput.
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_


#include "InputDevice.h"


class CKeyboard : public CInputDevice
{
public:
    // Init and End protocols
    CKeyboard() {}
    virtual ~CKeyboard()
    {
        Done();
    }

    bool          Init            (LPDIRECTINPUT8, HWND);
    bool	        IsOk			      () const
    {
        return m_bIsOk;
    }
    void	        Done			      ();

    //----CInputDevice Interface----
    HRESULT       Init			      ();
    HRESULT       Update			    ();

    //----CMouse Interface----------
    bool					IsDown					(uint32 nID) const;
    bool					IsUpDown    		(uint32 nID) const;
    bool					IsDownUp				(uint32 nID) const;

private:
    // Tipos----
    typedef CInputDevice Inherited;
    //----------

    char          m_Keys[256];
    char          m_KeysOld[256];
};

#endif //INC_KEYBOARD_H_