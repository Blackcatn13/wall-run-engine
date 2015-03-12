#include "ScriptedController.h"
#include "Core\Core.h"
#include "ScriptManager.h"
#include "Core_Utils/MemLeaks.h"

CScriptedController::CScriptedController()
{
    SCRIPTM->RunFile(".\\Data\\scripted_controller.lua");	
}

CScriptedController::~CScriptedController() {}

void CScriptedController::Update(float dt)
{
    //float l_ElapsedTime=0.3f;
    char l_Text[256];
    _snprintf_s(l_Text, 256, 256, "on_update_scripted_controller(%f)", dt);
    SCRIPTM->RunCode(l_Text);
	char l_TextCameras[256];
	_snprintf_s(l_TextCameras, 256, 256, "on_update_cameras_lua(%f)", dt);
	SCRIPTM->RunCode(l_TextCameras);
	char l_TextPlayer[256];
	_snprintf_s(l_TextPlayer, 256, 256, "on_update_player_lua(%f)", dt);
	SCRIPTM->RunCode(l_TextPlayer);
}

void CScriptedController::Render(CGraphicsManager *RM)
{
}