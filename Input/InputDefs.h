//----------------------------------------------------------------------------------
// Input Defines class
// Author: Enric Vergara
//
// Description:
// Definiciones para el codigo relacionado con el Input
//----------------------------------------------------------------------------------
#ifndef INC_INPUT_DEFINES_H
#define INC_INPUT_DEFINES_H

#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\MathTypes.h"

typedef enum INPUT_DEVICE_TYPE {
    IDV_KEYBOARD,
    IDV_GAMEPAD1,
    IDV_GAMEPAD2,
    IDV_GAMEPAD3,
    IDV_GAMEPAD4,
    IDV_MOUSE,
    IDV_NOTHING

};

typedef enum INPUT_AXIS_TYPE {
    AXIS_MOUSE_X,
    AXIS_MOUSE_Y,
    AXIS_MOUSE_Z,
    AXIS_LEFT_THUMB_X,
    AXIS_LEFT_THUMB_Y,
    AXIS_RIGHT_THUMB_X,
    AXIS_RIGHT_THUMB_Y,
    AXIS_DELTA_TRIGGER_RIGHT,
    AXIS_DELTA_TRIGGER_LEFT,
    AXIS_NOTHING
};

typedef enum INPUT_EVENT_TYPE {
    EVENT_DOWN,
    EVENT_UP_DOWN,
    EVENT_DOWN_UP,
    EVENT_NOTHING
};

typedef enum MOUSE_BUTTON {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE,
    MOUSE_BUTTON_NOTHING
};


//------------------------------------------
//----- Gamepad codes from XInput.h --------
#define PAD_DPAD_UP          				0x0001
#define PAD_DPAD_DOWN        				0x0002
#define PAD_DPAD_LEFT        				0x0004
#define PAD_DPAD_RIGHT       				0x0008
#define PAD_DPAD_START            	0x0010
#define PAD_DPAD_BACK             	0x0020
#define PAD_BUTTON_LEFT_THUMB     	0x0040
#define PAD_BUTTON_RIGHT_THUMB    	0x0080
#define PAD_BUTTON_LEFT_SHOULDER  	0x0100
#define PAD_BUTTON_RIGHT_SHOULDER 	0x0200
#define PAD_BUTTON_A              	0x1000
#define PAD_BUTTON_B              	0x2000
#define PAD_BUTTON_X              	0x4000
#define PAD_BUTTON_Y              	0x8000
//------------------------------------------



//------------------------------------------
//--- Keyboard scan codes from dinput.h ----
#define KEY_ESCAPE          0x01
#define KEY_TAB             0x0F
#define KEY_SPACE           0x39
#define KEY_RETURN          0x1C
#define KEY_BACK            0x0E
#define KEY_CAPITAL         0x3A

#define KEY_MINUS           0x0C
#define KEY_EQUALS          0x0D
#define KEY_LBRACKET        0x1A
#define KEY_RBRACKET        0x1B
#define KEY_SEMICOLON       0x27
#define KEY_APOSTROPHE      0x28
#define KEY_GRAVE           0x29
#define KEY_BACKSLASH       0x2B
#define KEY_COMMA           0x33
#define KEY_PERIOD          0x34
#define KEY_SLASH           0x35

// CHARACTER KEYS
#define KEY_A               0x1E
#define KEY_S               0x1F
#define KEY_D               0x20
#define KEY_F               0x21
#define KEY_G               0x22
#define KEY_H               0x23
#define KEY_J               0x24
#define KEY_K               0x25
#define KEY_L               0x26
#define KEY_Z               0x2C
#define KEY_X               0x2D
#define KEY_C               0x2E
#define KEY_V               0x2F
#define KEY_B               0x30
#define KEY_N               0x31
#define KEY_M               0x32
#define KEY_Q               0x10
#define KEY_W               0x11
#define KEY_E               0x12
#define KEY_R               0x13
#define KEY_T               0x14
#define KEY_Y               0x15
#define KEY_U               0x16
#define KEY_I               0x17
#define KEY_O               0x18
#define KEY_P               0x19

// MAIN NUM KEYS
#define KEY_1               0x02
#define KEY_2               0x03
#define KEY_3               0x04
#define KEY_4               0x05
#define KEY_5               0x06
#define KEY_6               0x07
#define KEY_7               0x08
#define KEY_8               0x09
#define KEY_9               0x0A
#define KEY_0               0x0B

// FUNCTION KEYS
#define KEY_F1              0x3B
#define KEY_F2              0x3C
#define KEY_F3              0x3D
#define KEY_F4              0x3E
#define KEY_F5              0x3F
#define KEY_F6              0x40
#define KEY_F7              0x41
#define KEY_F8              0x42
#define KEY_F9              0x43
#define KEY_F10             0x44
#define KEY_F11             0x57
#define KEY_F12             0x58

// ON NUMPAD
#define KEY_NPPLUS          0x4E
#define KEY_NPMINUS         0x4A
#define KEY_NPDECIMAL       0x53
#define KEY_NPCOMMA         0xB3
#define KEY_NPDIVIDE        0xB5
#define KEY_NPMULTIPLY      0x37
#define KEY_NPENTER         0x9C
#define KEY_NUMLOCK         0x45
#define KEY_NP1             0x4F
#define KEY_NP2             0x50
#define KEY_NP3             0x51
#define KEY_NP4             0x4B
#define KEY_NP5             0x4C
#define KEY_NP6             0x4D
#define KEY_NP7             0x47
#define KEY_NP8             0x48
#define KEY_NP9             0x49
#define KEY_NP0             0x52

// CTRL, ALT, SHFT and WINDWS
#define KEY_RSHIFT          0x36
#define KEY_LSHIFT          0x2A
#define KEY_RCTRL           0x9D
#define KEY_LCTRL           0x1D
#define KEY_RALT            0xB8
#define KEY_LALT            0x38
#define KEY_LWIN            0xDB
#define KEY_RWIN            0xDC

// ON ARROW KEYPAD
#define KEY_UP              0xC8
#define KEY_DOWN            0xD0
#define KEY_LEFT            0xCB
#define KEY_RIGHT           0xCD
#define KEY_INSERT          0xD2
#define KEY_DELETE          0xD3
#define KEY_HOME            0xC7
#define KEY_END             0xCF
#define KEY_PGDOWN          0xD1
#define KEY_PGUP            0xC9

#define KEY_PAUSE           0xC5    /* Pause */
#define KEY_SCROLL          0x46    /* Scroll Lock */
//------------------------------------------


#endif //INC_INPUT_DEFINES_H