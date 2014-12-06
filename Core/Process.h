#ifndef CPROCESS_H
#define CPROCESS_H

#include "Math\Vector2.h"

class CCamera;


class CProcess
{
protected:
    CCamera*	m_Camera;
    bool        m_printInfo;
public:
    CProcess(void);
    virtual ~CProcess(void);
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Init();
    void DeInit();
    CCamera* GetCamera()
    {
        return m_Camera;
    }
    virtual Vect2i RenderDebugInfo(bool render, float dt);
    void SetPrintInfo(bool print)
    {
        m_printInfo = print;
    }
    bool getPrintInfo()
    {
        return m_printInfo;
    }
};


#endif
