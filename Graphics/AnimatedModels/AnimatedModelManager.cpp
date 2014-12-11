#include "AnimatedModelManager.h"
#include <string>
#include "MapManager.h"
#include "AnimatedCoreModel.h"
#include "AnimatedInstanceModel.h"


class CAnimatedModelManager : public CMapManager<CAnimatedCoreModel>
{
public:
	CAnimatedModelManager(){}
	~CAnimatedModelManager(){}
	CAnimatedCoreModel * GetCore(const std::string &Name, const std::string &Path)
	{
		CMapManager<CAnimatedCoreModel>::iterator it = m_Resources.Find(Name);
		if(it != m_Resources->end()){
			return it->second();
		}else{
			CAnimatedCoreModel* animCoreModel = new CAnimatedCoreModel();
			animCoreModel->Load(Path);
			return animCoreModel;
		}
	}
	CAnimatedInstanceModel * GetInstance(const std::string &Name)
	{
		CAnimatedInstanceModel* animInstanceModel = NULL;
		CMapManager<CAnimatedCoreModel>::iterator it = m_Resources.Find(Name);
		if(it != m_Resources->end()){
			animInstanceModel = new CAnimatedInstanceModel();
			animInstanceModel->Initialize(it->second(), CORE->GetGraphicsManager());
		}
		
		return animInstanceModel;
	}
	void Load(const std::string &Filename)
	{
		m_File = FileName;
		CXMLTreeNode newFile;
		if (!newFile.LoadFile(FileName.c_str()))
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
					CAnimatedCoreModel* animModel = CAnimatedCoreModel();
					animModel->Load(path);
					AddResource(name, animModel);
				}
			} 
		}
	}
};