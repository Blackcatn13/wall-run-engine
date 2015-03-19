#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include <string>
#include "Utils\TemplatedVectorMapManager.h"

class CParcitleEmitter;
class CGraphicsManager;

class CParticleManager : public CTemplatedVectorMapManager<CParcitleEmitter>
{
private:
	std::string m_fileName;
	void Load();

public:
    CParticleManager();
    ~CParticleManager();
	void Load(std::string file);
	void Reload();
	void Update(float ElapsedTime);
    void Render(CGraphicsManager *RM);
};

#endif