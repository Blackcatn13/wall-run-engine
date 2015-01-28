#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Core\Core.h"
#include "Effects\EffectManager.h"
#include "Renderable\PoolRenderableObjectTechnique.h"

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager()
{
	//Constructor, construye la clase CRenderableObjectTechniqueManager.
	//XML relacion RenderableObjectManagers con Techniques

}

CRenderableObjectTechniqueManager::~CRenderableObjectTechniqueManager()
{
	//Destructor, destruye la clase CRenderableObjectTechniqueManager llamando al método Destroy.
	Destroy();
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName)
{
	CEffectTechnique * l_EffectTechnique = EFFECTM->GetEffectTechnique(TechniqueName);
	CRenderableObjectTechnique * l_RenderableObjectTechnique = new CRenderableObjectTechnique(ROTName, l_EffectTechnique);
	AddResource(ROTName, l_RenderableObjectTechnique);
	//InsertRenderableObjectTechnique, introducirá un
	//CRenderableObjectTechnique en el mapa según un nombre de renderable
	//object technique y el nombre de la technique que utilizará.
}

void CRenderableObjectTechniqueManager::Destroy()
{
	//TODO cambiar TMapResource por CRenderableObjectTechnique?
	for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); it++) {
            CHECKED_DELETE(it->second);
        }
        m_Resources.clear();
	//Destroy, destruye todos los CPoolRenderableObjectTechnique y los CRenderableObjectTechnique.
}


void CRenderableObjectTechniqueManager::Load(const std::string &FileName)
{
	m_FileName = FileName;
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(FileName.c_str())) {
        printf("ERROR loading the file.");
    }
    CXMLTreeNode  m = newFile["renderable_object_techniques"];
	int count = m.GetNumChildren();
	for (int i = 0; i < count; ++i) {
		std::string name = m(i).GetName();
        if (name == "pool_renderable_object_technique") {
			std::string l_poolName = m.GetPszISOProperty("name", "");
			//TODO Revisar que se le pasa el hijo correctamente
			CPoolRenderableObjectTechnique * l_Pool = new CPoolRenderableObjectTechnique(m(i));
			m_PoolRenderableObjectTechniques.AddResource(l_poolName, l_Pool);
			int count = m.GetNumChildren();
			for (int i = 0; i < count; ++i) {
				std::string name = m(i).GetName();
				if (name == "default_technique") {
					unsigned int l_VertexNumber = m(i).GetIntProperty("vertex_type",0);
					std::string l_TechniqueName = m(i).GetPszISOProperty("technique", "");
					//añadir CRenderableObjectTechniqueManager a CORE y hacer MACRO
					std::string l_RenderableObjectTechniqueName = GetRenderableObjectTechniqueNameByVertexType(l_VertexNumber);
					//CRenderableObjectTechnique * l_RenderableObjectTechnique = RENDTECHM->GetResource(l_RenderableObjectTechniqueName);
					InsertRenderableObjectTechnique(l_RenderableObjectTechniqueName, l_TechniqueName);
					//CRenderableObjectTechnique * l_RenderableObjectTechnique = new CRenderableObjectTechnique(l_RenderableObjectTechniqueName, 0);
					//AddResource(l_RenderableObjectTechniqueName, l_RenderableObjectTechnique);
				}
			}
		}
	}
}

CMapManager<CPoolRenderableObjectTechnique> & CRenderableObjectTechniqueManager::GetPoolRenderableObjectTechniques()
{
	return m_PoolRenderableObjectTechniques;
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(unsigned int VertexType)
{
	//TODO creo que esto esta mal
	std::string l_VertexName = "DefaultROTTechnique_";
	std::string l_VertexType = "" + VertexType;
	return l_VertexName.append(l_VertexType);
}

