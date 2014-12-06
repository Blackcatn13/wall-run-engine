//----------------------------------------------------------------------------------
// CMouse class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el input de tipo Mouse
// Internamente utiliza la libreria DirectInput.
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_MOUSE_H_
#define INC_MOUSE_H_

#include "InputDevice.h"

class CMouse : public CInputDevice
{
public:
    // Init and End protocols
    CMouse() {}
    virtual ~CMouse()
    {
        Done();
    }

    bool                Init              (LPDIRECTINPUT8, HWND, const Vect2i& screenRes, bool exclusiveMode);
    void	              Done			        ();

    //----CInputDevice Interface----
    virtual HRESULT	    Update	          ();


    //----CMouse Interface----------
    const Vect3i&       GetMouseDelta			()		const
    {
        return m_Delta;
    }

    bool	              IsDown						(uint32 nBtn)		const
    {
        if (nBtn < 3) return m_bIsDown[nBtn];
        return false;
    }
    bool	              IsUpDown					(uint32 nBtn)		const
    {
        if (nBtn < 3) return m_bIsUpDown[nBtn];
        return false;
    }
    bool	              IsDownUp					(uint32 nBtn)		const
    {
        if (nBtn < 3) return m_bIsDownUp[nBtn];
        return false;
    }

private:
    void                Release           ();

private:
    //-- Tipos---
    typedef CInputDevice Inherited;
    //------------

    HANDLE	            m_hEvent;
    bool								m_bIsDown[3];
    bool								m_bIsUpDown[3];
    bool								m_bIsDownUp[3];
    Vect3i							m_Delta;
    Vect2i							m_ScreenResolution;
};


#endif // INC_MOUSE_H_