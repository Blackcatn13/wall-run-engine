#include "ScriptedController.h"
#include "Core\Core.h"
#include "ScriptManager.h"

CScriptedController::CScriptedController()
{
	SCRIPTM->RunFile(".\\Data\\scripted_controller.lua");

}

CScriptedController::~CScriptedController(){}

void CScriptedController::Update(float dt)
{
	//float l_ElapsedTime=0.3f;
	char l_Text[256];
	_snprintf_s(l_Text, 256, 256, "on_update_scripted_controller(%f)", dt);
	SCRIPTM->RunCode(l_Text);
}

void CScriptedController::Render(CGraphicsManager *RM)
{
}