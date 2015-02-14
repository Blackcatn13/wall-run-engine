#include "Renderable\RenderableObjectsLayersManager.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include <assert.h>
#include "AnimatedModels\AnimatedModelManager.h"
#include "AnimatedModels\AnimatedInstanceModel.h"

CRenderableObjectsLayersManager ::CRenderableObjectsLayersManager()
{
	m_DefaultRenderableObjectManager = NULL;
	m_CurrentLayer="solid";
}

CRenderableObjectsLayersManager ::~CRenderableObjectsLayersManager()
{
    Destroy();
}

void CRenderableObjectsLayersManager::Update(float ElapsedTime)
{
    for (int i = 0; i < m_ResourcesVector.size(); ++i) {
        m_ResourcesVector[i]->Update(ElapsedTime);
    }
}

void CRenderableObjectsLayersManager::Render(CGraphicsManager *RM)
{
	int test = m_ResourcesVector.size();
    for (int i = 0; i < m_ResourcesVector.size(); ++i) {
		int test2 = i;
        m_ResourcesVector[i]->Render(RM);
    }
}

void CRenderableObjectsLayersManager::Render(CGraphicsManager *RM, const std::string & LayerName)
{
	CRenderableObjectsManager* l_managerInstance = GetResource(LayerName);
    if (l_managerInstance != NULL)
	{
		l_managerInstance->Render(RM);
	}

}
/*
void CRenderableObjectsLayersManager::Destroy()
{
	for (int i = 0; i < m_ResourcesVector.size(); ++i) {
        m_ResourcesVector[i]->Destroy();
    }
    Destroy();
}
*/
void CRenderableObjectsLayersManager::Reload()
{
	Destroy();
	Load(m_FileName);
	Load(m_FileName2);
}

CRenderableObjectsManager * CRenderableObjectsLayersManager::GetRenderableObjectManager(CXMLTreeNode &Node)
{
	std::string l_LayerName=Node.GetPszProperty("layer", m_DefaultLayerName.c_str());
	return GetResource(l_LayerName);
/*
	CXMLTreeNode  m = Node;
    if (Node.Exists()) {
		int count = m.GetNumChildren();
		for (int i = 0; i < count; ++i) {
				std::string name = m(i).GetName();
				if (name == "mesh_instance") 
				{
					std::string layerName = m(i).GetPszISOProperty("layer", m_DefaultLayerName);
					return GetResource(layerName);
			 }
		}
	}*/
}

CRenderableObjectsManager * CRenderableObjectsLayersManager::GetDefaultRenderableObjectManager()
{
	return GetResource( m_DefaultLayerName.c_str());
}

void CRenderableObjectsLayersManager::Load(const std::string &FileName)
{
	CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\animated_models.xml"); //se carga animatedmodels.xml
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(FileName.c_str())) {
        printf("ERROR loading the file.");
    }
    CXMLTreeNode  m = newFile["renderable_objects"];
	CXMLTreeNode  m2= newFile["lua_scripts"];
	if (m.Exists()) {
		m_FileName = FileName;
		int count = m.GetNumChildren();
		for (int i = 0; i < count; ++i) {
				std::string name = m(i).GetName();
				if (name == "layer") {
					std::string layerName = m(i).GetPszISOProperty("name", "");
					bool isDefault = m(i).GetBoolProperty("default", false);
					CRenderableObjectsManager* l_managerInstance = new CRenderableObjectsManager();
					//CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
					AddResource(layerName, l_managerInstance);
					if (isDefault)
					{
						m_DefaultLayerName = layerName;
						m_DefaultRenderableObjectManager = l_managerInstance;
					}
				}
		 }

		 for (int i = 0; i < count; ++i) {
				std::string name = m(i).GetName();
				//if ((name == "mesh_instance") || (name == "renderable_script")){
				if (name == "mesh_instance"){
					(GetRenderableObjectManager(m(i)))->Load(m(i));
					/*std::string layerAssigned = m(i).GetPszISOProperty("layer", "box1");
					if (layerAssigned == "box1")
					{
						m_DefaultRenderableObjectManager->Load(m(i));
					}
					else
					{
						//FIND layerAssigned, load m(i)
						CRenderableObjectsManager* l_managerInstance = GetResource(layerAssigned);
						if (l_managerInstance != NULL)
						{
							l_managerInstance->Load(m(i));
						}
					}*/
				}
		 }
	 }
	 if(m2.Exists())
	 {
		m_FileName2 = FileName;
		int count = m2.GetNumChildren();
        for (int i = 0; i < count; ++i) 
		{
            std::string name = m2(i).GetName();
            if (name == "renderable_script") 
			{
				GetDefaultRenderableObjectManager()->Load(m2(i));
				
			}
		}
	}

}