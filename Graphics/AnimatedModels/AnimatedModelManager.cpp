#include "AnimatedModelManager.h"
#include <string>
#include "AnimatedCoreModel.h"
#include "AnimatedInstanceModel.h"
#include "XML\XMLTreeNode.h"
#include <iterator>
#include "Core\Core.h"

CAnimatedModelManager::CAnimatedModelManager(){}
CAnimatedModelManager::~CAnimatedModelManager(){}
CAnimatedCoreModel * CAnimatedModelManager::GetCore(const std::string &Name, const std::string &Path)
{
	//TODO Name o Path + Name
	//TMapResource CMapManager<CAnimatedCoreModel>::iterator it
	TMapResource::iterator it = m_Resources.find(Name);
	if(it != m_Resources.end()){
		return it->second;
	}else{
		CAnimatedCoreModel* animCoreModel = new CAnimatedCoreModel();
		animCoreModel->Load(Path);
		return animCoreModel;
	}
}
CAnimatedInstanceModel * CAnimatedModelManager::GetInstance(const std::string &Name)
{
	CAnimatedInstanceModel* animInstanceModel = NULL;
	TMapResource::iterator it = m_Resources.find(Name);
	if(it != m_Resources.end()){
		animInstanceModel = new CAnimatedInstanceModel();
		animInstanceModel->Initialize(it->second, GRAPHM);
	}
		
	return animInstanceModel;
}
void CAnimatedModelManager::Load(const std::string &Filename)
{
	std::string m_File = Filename;
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(Filename.c_str()))
	{
		printf("ERROR loading the file.");
	}
	CXMLTreeNode  m = newFile["animated_models"];
	if (m.Exists())
	{
		int count = m.GetNumChildren();
		for (int i = 0; i < count; ++i)
		{
			if(m(i).GetName() == "animated_model") {
				std::string name = m(i).GetPszISOProperty("name");
				std::string path = m(i).GetPszISOProperty("path");
				CAnimatedCoreModel* animModel = new CAnimatedCoreModel();
				animModel->Load(path);
				AddResource(name, animModel);
			}
		} 
	}
}
