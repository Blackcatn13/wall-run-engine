#ifndef CLIGHTMANAGER_H
#define CLIGHTMANAGER_H

#include "Utils\MapManager.h"
#include <string>
#include "GraphicsManager.h"
#include "Light.h"


class CLight;

class CLightManager : public CMapManager<CLight>
{
public:
	CLightManager();
	~CLightManager();
	void Load(const std::string &FileName);
	void Render(CGraphicsManager *RenderManager);
};

#endif
