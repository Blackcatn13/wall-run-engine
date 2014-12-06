//----------------------------------------------------------------------------------
// CKeyboard class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el input de tipo keyboard
// Internamente utiliza la librería DirectInput.
//----------------------------------------------------------------------------------

#ifndef INC_GAMEPAD_H_
#define INC_GAMEPAD_H_


#include <windows.h>
#include <XInput.h>
#include "Math/Vector3.h"


//-------------Declaracion de nuevos tipos-----------
struct PAD_CONTROLLER {
    bool								m_bConnected;
    XINPUT_STATE        m_pState;
    XINPUT_VIBRATION		m_vibration;
    WORD                m_wOldButtons;
    WORD                m_wNewButtons;
    Vect2i							m_Pos;
};

#define INPUT_DEADZONE  ( 0.2f * FLOAT(0x7FFF) )  // Default to 20% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
#define MAX_CONTROLLERS 4
//---------------------------------------------------

class CGamePad
{
public:
    CGamePad(): m_bIsOk(false), m_bDeadZoneOn(true) {}
    virtual ~CGamePad()
    {
        /*Nothing*/;
    }

    bool						Init	                    (const Vect2i& screenRes);
    void						Update										();

    bool						IsDeadZoneOn		          ()	const
    {
        return m_bDeadZoneOn;
    }
    bool						IsConnected		            (uint32 idController = 0)	const;
    bool  					IsDown                    (uint32 nBtn, uint32 idController = 0);
    bool  					IsDownUp                  (uint32 nBtn, uint32 idController = 0);
    bool  					IsUpDown                  (uint32 nBtn, uint32 idController = 0);
    void						SetLeftMotorSpeed					(uint32 speed, uint32 idController = 0); //[0-65535]
    void						SetRightMotorSpeed				(uint32 speed, uint32 idController = 0); //[0-65535]
    void						GetMotorSpeed							(uint32& left, uint32& right, uint32 idController = 0);

    bool  					GetLeftThumbDeflection    (float *pfX, float *pfY, uint32 idController = 0);
    bool  					GetRightThumbDeflection   (float *pfX, float *pfY, uint32 idController = 0);
    bool  					GetDeltaTriggers          (float *pfLeft, float *pfRight, uint32 idController = 0);
    uint32 					GetButtonsPressed         (uint32 idController = 0)	const;
    void						GetPosition								(Vect2i &pos, uint32 idController = 0);

private:
    bool            m_bDeadZoneOn;
    bool            m_bIsOk;
    PAD_CONTROLLER	m_Controllers[MAX_CONTROLLERS];
    Vect2i					m_ScreenResolution;
};

#endif // INC_GAMEPAD_H_