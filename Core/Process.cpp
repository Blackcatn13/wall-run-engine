#include "Process.h"
#include "Core.h"
#include "FontManager.h"

CProcess::CProcess(void)
{
}


CProcess::~CProcess(void)
{
}

void CProcess::Init()
{
}

void CProcess::DeInit()
{
}

Vect2i CProcess::RenderDebugInfo(bool render, float dt)
{
	Vect2i pos = Vect2i(50, 2);
	if(!render) {
        pos.Add_Max(CCore::GetInstance()->GetFontManager()->GetDefaultTextSize(pos.x, pos.y, colWHITE, "Info from %s", __FILE__));
        pos.Add_Max(CCore::GetInstance()->GetFontManager()->GetDefaultTextSize(pos.x, pos.y, colWHITE, "%f FPS", 1/dt));
	}
	else {
		pos.Add_Max(CCore::GetInstance()->GetFontManager()->DrawDefaultText(pos.x, pos.y, colWHITE, "Info from %s", __FILE__));
        pos.Add_Max(CCore::GetInstance()->GetFontManager()->DrawDefaultText(pos.x, pos.y, colWHITE, "%f FPS", 1/dt));
	}
	return pos;
}
