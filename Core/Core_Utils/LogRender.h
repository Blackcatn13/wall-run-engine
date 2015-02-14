//----------------------------------------------------------------------------------
// CLogRender class
// Author: Enric Vergara
//
// Description:
// Clase encargada de pintar por pantalla la informacion del logger
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_LOG_RENDER_H_
#define INC_LOG_RENDER_H_

#include "Math/MathTypes.h"
#include "Math/Color.h"
#include "Utils/LerpAnimator1D.h"
#include "Utils/Logger.h"
#include <vector>

#define INC_X_TO_LEVEL		50
#define INC_X_TO_MESSAGE	200

//---Forward Declarations---
class CRenderManager;
class CFontManager;
//--------------------------

class CLogRender
{
public:
    CLogRender();
    ~CLogRender();

    void					Render							(CGraphicsManager* GraphicsManager, CFontManager* fm, CColor color = colWHITE);
    void					Update							(float deltaTime);
    uint32				GetNumOfLines				() const
    {
        return m_uLinesPerPage;
    }
    void					PageUp							();
    void					PageDown						();
    void					NextLine						();
    void					PrevLine						();
    void					SetWindowsPos				(const Vect2i& pos )
    {
        m_WindowsPos = pos;
    }
    const Vect2i&	GetWindowsPos				() const
    {
        return m_WindowsPos;
    }
    bool					GetVisible					() const
    {
        return m_bIsVisible;
    }
    void					SetVisible					(bool flag);
    void					SetLinePerPage			(uint32 lines);

private:
    void					RenderLines					(	CGraphicsManager* GraphicsManager, CFontManager* fm, const std::vector<SLog>& vecLogs,
            uint32 beginIndex, uint32 endIndex, bool errors, bool warnings, CColor color);
    void					RenderQuad					();
    void					ComputeBeginAndEnd	(const std::vector<SLog>& vecLogs, uint32& beginIndex, uint32& endIndex);


private:

    bool								m_bIsVisible;
    bool								m_uIsInLastLog;
    bool								m_uIsFullFirstPage;
    Vect2i							m_WindowsPos;
    uint32							m_uLinesPerPage;
    uint32							m_uCurrentBeginIndex;
    uint32							m_uCurrentEndIndex;
    std::vector<SLog>		m_VecLogsToRender;

    uint32							m_uLinesPerPageAnimation;
    bool								m_bAnimation;
    int32								m_iDirecion;
    float								m_fAnimationTime;
    float								m_fTotalAnimation;
    CColor							m_Quad2dColor;
    float								m_fAlpha;

    CLerpAnimator1D			m_LerpAnimator1D;

};
#endif //INC_LOG_RENDER_H_ 
