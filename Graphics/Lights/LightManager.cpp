#include "Lights\LightManager.h"
#include "XML\XMLTreeNode.h"
#include <map>
#include "OmniLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "Utils\Logger.h"
#include "Core\Core.h"
#include "Core_Utils/MemLeaks.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObjectsLayersManager.h"


CLightManager::CLightManager()
{}
CLightManager::~CLightManager()
{
    Destroy();
}

void CLightManager::Load(const std::string &FileName)
{
    m_File = FileName;
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(FileName.c_str())) {
        std::string l_ErrorMsg = "ERROR loading file" + FileName;
        LOGGER->AddNewLog(ELL_ERROR,  l_ErrorMsg.c_str());
        //	printf("ERROR loading the file.");
    }
    CXMLTreeNode  m = newFile["lights"];
    if (m.Exists()) {
        int count = m.GetNumChildren();
        for (int i = 0; i < count; ++i) {
            if (std::string("light") == m(i).GetName()) {
                //	XML type ejemplo
                std::string l_Type = m(i).GetPszISOProperty("type", "");
                std::string l_Name = m(i).GetPszISOProperty("name", "");
                if (l_Type == "omni") {
                    COmniLight * l_OmniLight = new COmniLight(m(i));
                    l_OmniLight->Init();
                    AddResource(l_Name, l_OmniLight);
                } else if (l_Type == "directional") {
                    CDirectionalLight* l_DirectionalLight = new CDirectionalLight(m(i));
                    l_DirectionalLight->Init();
                    AddResource(l_Name, l_DirectionalLight);
                } else if (l_Type == "spot") {
                    CSpotLight* l_SpotLight = new CSpotLight(m(i));
                    l_SpotLight->Init();
                    AddResource(l_Name, l_SpotLight);
                }
            }
        }
    }
}
void CLightManager::Render(CGraphicsManager *RenderManager)
{
    //El render de las luces solo es para debug
#ifdef _DEBUG
    {
        TMapResource::iterator it = m_Resources.begin();
        TMapResource::iterator it_End = m_Resources.end();
        while (it != it_End) {
            it->second->Render(RenderManager);
            ++it;
        }
    }
#endif
}

void CLightManager::Reload()
{
    Destroy();
    Load(m_File);
}

void CLightManager::ReloadRO()
{
    for (auto it = m_Resources.begin(); it != m_Resources.end(); ++it) {
        it->second->ReloadRO();
    }
}