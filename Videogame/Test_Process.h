#ifndef CTEST_H
#define CTEST_H

#include "Core/Process.h"
#include "Math/Matrix44.h"

class CObject3D;
class CFPSCamera;
class CThPSCamera;

class CTest_Process : public CProcess
{
private:
    CObject3D*		m_ObjectFPS;
    CObject3D*		m_ObjectThPS;
    CFPSCamera*		m_FPSCamera;
    CThPSCamera*	m_ThPSCamera;
    bool			m_PlayerMode;
    Mat44f			t1;
    int				skip;
public:
    CTest_Process(void);
    ~CTest_Process(void);
    void Update(float dt);
    void Render();
    void Init();
    void DeInit();
};
//coment
#endif
