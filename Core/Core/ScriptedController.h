#ifndef CSCRIPTEDCONTROLLER_H
#define CSCRIPTEDCONTROLLER_H

#include "Utils\Defines.h"
#include "Renderable\RenderableObject.h"

class CScriptedController : public CRenderableObject
{
public:
    CScriptedController();
    ~CScriptedController();
    void Update(float dt);
    void Render(CGraphicsManager *RM);

};


#endif