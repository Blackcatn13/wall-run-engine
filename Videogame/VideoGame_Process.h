#ifndef CVIDEOGAME_PROCESS_H
#define CVIDEOGAME_PROCESS_H

#include "Core/Process.h"

class CObject3D;
class CFPSCamera;
class CThPSCamera;

class CVideoGame_Process : public CProcess
{
private:
    CObject3D*		m_ObjectFPS;
    CObject3D*		m_ObjectThPS;
    CFPSCamera*		m_FPSCamera;
    CThPSCamera*	m_ThPSCamera;
    bool			m_PlayerMode;
public:
    CVideoGame_Process(void);
    ~CVideoGame_Process(void);
    void Update(float dt);
    void Render();
    void Init();
    void DeInit();
};

#endif
