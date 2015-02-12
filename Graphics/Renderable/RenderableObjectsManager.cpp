#include "Renderable\RenderableObjectsManager.h"
#include "GraphicsManager.h"
#include "Mesh\MeshInstance.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "AnimatedModels\AnimatedModelManager.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "Core\ScriptedController.h"

#include <assert.h>

#include "Core_Utils/MemLeaks.h"

CRenderableObjectsManager ::CRenderableObjectsManager()
{
	m_FileName = "";
	m_FileName2 = "";
}

CRenderableObjectsManager ::~CRenderableObjectsManager()
{
    Destroy();
}

void CRenderableObjectsManager::Update(float ElapsedTime)
{
    for (int i = 0; i < m_ResourcesVector.size(); ++i) {
        m_ResourcesVector[i]->Update(ElapsedTime);
    }
}

void CRenderableObjectsManager::Render(CGraphicsManager *RM)
{
    for (int i = 0; i < m_ResourcesVector.size(); ++i) {
        m_ResourcesVector[i]->Render(RM);
    }
}

CRenderableObject* CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position)
{
    CMeshInstance * newInstance = new CMeshInstance(InstanceName, CoreMeshName);
    newInstance->SetPosition(Position);
    AddResource(InstanceName, newInstance);
    return newInstance;
}

CRenderableObject* CRenderableObjectsManager::AddMeshInstance(CXMLTreeNode &Node)
{
    CMeshInstance * newInstance = new CMeshInstance(Node);
    AddResource(newInstance->getName(), newInstance);
    return newInstance;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position)
{
    assert (!"Not yet implemented");
    return NULL;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(CXMLTreeNode &Node)
{
    assert (!"Not yet implemented");
    return NULL;
}

void CRenderableObjectsManager::Load(const std::string &FileName)
{
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(FileName.c_str())) {
        printf("ERROR loading the file.");
    }
    CXMLTreeNode  m = newFile["renderable_objects"];
    CXMLTreeNode m2 = newFile["lua_scripts"];
	//this->Load(m);
	//TODO comentar todo y usar la llamada a Load(XMLTreeNode)
	if (m.Exists()) {
		 m_FileName = FileName;
        //cargamos aqui el animated models para no hacerlo varias veces dentro del bucle
        //CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\level1\\animated_models.xml"); //se carga animatedmodels.xml
        //CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\animated_models.xml"); //se carga animatedmodels.xml
        int count = m.GetNumChildren();
        for (int i = 0; i < count; ++i) {
            std::string name = m(i).GetName();
            if (name == "mesh_instance") {
                //TODO
                std::string meshName = m(i).GetPszISOProperty("name", "box1");
                std::string core = m(i).GetPszISOProperty("core", "box");
                Vect3f pos = m(i).GetVect3fProperty("pos", v3fZERO);
                float yaw = m(i).GetFloatProperty("yaw");
                float pitch = m(i).GetFloatProperty("pitch");
                float roll = m(i).GetFloatProperty("roll");
                Vect3f scale = m(i).GetVect3fProperty("scale", v3fONE);
                //TODO Static mesh por mesh instance
                //
                CMeshInstance* l_meshInstance = new CMeshInstance(meshName, core);
                l_meshInstance->SetYaw(yaw);
                l_meshInstance->SetPosition(pos);
                l_meshInstance->SetPitch(pitch);
                l_meshInstance->SetRoll(roll);
                l_meshInstance->SetScale(scale);
                //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
                AddResource(meshName, l_meshInstance);
            } else if (name == "animated_model") {
                //FALTA CARGAR EL ANIMATED_MODELS.XML
                std::string meshName = m(i).GetPszISOProperty("name", "box1");
                std::string core = m(i).GetPszISOProperty("core", "box");
                Vect3f pos = m(i).GetVect3fProperty("pos", v3fZERO);
                float yaw = m(i).GetFloatProperty("yaw");
                float pitch = m(i).GetFloatProperty("pitch");
                float roll = m(i).GetFloatProperty("roll");
                float scale = m(i).GetFloatProperty("scale");
                CAnimatedInstanceModel * l_AnimatedInstanceModel  = ANIMAN->GetInstance(core);
                l_AnimatedInstanceModel->SetYaw(yaw);
                l_AnimatedInstanceModel->SetPosition(pos);
                l_AnimatedInstanceModel->SetPitch(pitch);
                l_AnimatedInstanceModel->SetRoll(roll);
                l_AnimatedInstanceModel->SetScale(scale);
                //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
                AddResource(meshName, l_AnimatedInstanceModel);
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
				std::string l_name = m2(i).GetPszISOProperty("name", "");
				std::string l_file = m2(i).GetPszISOProperty("file", "");
				if(l_name == "scriptedController")
				{
					CScriptedController *l_ScriptedController = new CScriptedController();
					AddResource(l_name, l_ScriptedController);
				}
				
			}
		}
	}
}

void CRenderableObjectsManager::Load(CXMLTreeNode &Node)
{
	CXMLTreeNode  m = Node;
    if (Node.Exists()) {
        //cargamos aqui el animated models para no hacerlo varias veces dentro del bucle
        //CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\level1\\animated_models.xml"); //se carga animatedmodels.xml
        //CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\animated_models.xml"); //se carga animatedmodels.xml
		//int count = m.GetNumChildren();
        //for (int i = 0; i < count; ++i) {
            std::string name = m.GetName();
            if (name == "mesh_instance") {
                //TODO poder pasar XMLTreeNode
                std::string meshName = m.GetPszISOProperty("name", "box1");
                std::string core = m.GetPszISOProperty("core", "box");
                Vect3f pos = m.GetVect3fProperty("pos", v3fZERO);
                float yaw = m.GetFloatProperty("yaw");
                float pitch = m.GetFloatProperty("pitch");
                float roll = m.GetFloatProperty("roll");
                Vect3f scale = m.GetVect3fProperty("scale", v3fONE);
                //TODO Static mesh por mesh instance hecho?
                //
                CMeshInstance* l_meshInstance = new CMeshInstance(meshName, core);
                l_meshInstance->SetYaw(yaw);
                l_meshInstance->SetPosition(pos);
                l_meshInstance->SetPitch(pitch);
                l_meshInstance->SetRoll(roll);
                l_meshInstance->SetScale(scale);
                //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
                AddResource(meshName, l_meshInstance);
            } else if (name == "animated_model") {
                //FALTA CARGAR EL ANIMATED_MODELS.XML hecho?
                std::string meshName = m.GetPszISOProperty("name", "box1");
                std::string core = m.GetPszISOProperty("core", "box");
                Vect3f pos = m.GetVect3fProperty("pos", v3fZERO);
                float yaw = m.GetFloatProperty("yaw");
                float pitch = m.GetFloatProperty("pitch");
                float roll = m.GetFloatProperty("roll");
                float scale = m.GetFloatProperty("scale");
                CAnimatedInstanceModel * l_AnimatedInstanceModel  = ANIMAN->GetInstance(core);
                l_AnimatedInstanceModel->SetYaw(yaw);
                l_AnimatedInstanceModel->SetPosition(pos);
                l_AnimatedInstanceModel->SetPitch(pitch);
                l_AnimatedInstanceModel->SetRoll(roll);
                l_AnimatedInstanceModel->SetScale(scale);
                //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
                AddResource(meshName, l_AnimatedInstanceModel);
            }else  if (name == "renderable_script") 
			{
				std::string l_name = m.GetPszISOProperty("name", "");
				std::string l_file = m.GetPszISOProperty("file", "");
				if(l_name == "scriptedController")
				{
					CScriptedController *l_ScriptedController = new CScriptedController();
					AddResource(l_name, l_ScriptedController);
				}
				
			}
        //}
    }
}




void CRenderableObjectsManager::Reload()
{
    Destroy();
    Load(m_FileName);
	Load(m_FileName2);
}