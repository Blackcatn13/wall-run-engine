#include "Renderable\PoolRenderableObjectTechnique.h"
#include "Core\Core.h"
#include "Effects\EffectManager.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Renderable\PoolRenderableObjectTechnique.h"
#include "Utils\Defines.h"
#include "Core_Utils/MemLeaks.h"

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode)
{
    /*<renderable_object_techniques>
    	<pool_renderable_object_technique name="dummy_manager">
    		<default_technique vertex_type="31"
    	technique="NormalMapAmbientLightNormalTextureVertexTechnique"/>
    	<default_technique vertex_type="19"
    	technique="AmbientLightNormalTextureVertexTechnique"/>
    	<default_technique vertex_type="159" technique="Cal3DTechnique"/>
    	<default_technique vertex_type="35"
    	technique="AmbientLightNormalTextureVertexTechnique"/>
    	</pool_renderable_object_technique>
    */
    //nombres para parsear
    //nombre DefaultROTTechnique_31 renderableObjectTechnique
    //nombre: technique="NormalMapAmbientLightNormalTextureVertexTechnique"/> EffectTechnique
    CXMLTreeNode m = TreeNode;
    //padre debe ser <pool_renderable_object_technique name="dummy_manager"
    std::string l_poolName = m.GetPszISOProperty("name", "");
    setName(l_poolName);
    int count = m.GetNumChildren();
    //hijos deben ser <default_technique vertex_type="
    for (int i = 0; i < count; ++i) {
        std::string name = m(i).GetName();
        if (name == "default_technique") {
            unsigned int l_VertexNumber = m(i).GetIntProperty("vertex_type", 0);
            std::string l_TechniqueName = m(i).GetPszISOProperty("technique", "");
            //añadir CRenderableObjectTechniqueManager a CORE y hacer MACRO
            std::string l_RenderableObjectTechniqueName = RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(l_VertexNumber);
            CRenderableObjectTechnique * l_RenderableObjectTechnique = RENDTECHM->GetResource(l_RenderableObjectTechniqueName);
            //añadir EffectManager a CORE si no existe y hacer macro
            //lo hacemos dentro del AddElement
            //CEffectTechnique * l_EffectTechnique = EFFECTM->GetEffectTechnique(l_TechniqueName);
            //crear:
            //CPoolRenderableObjectTechniqueElement(const std::string &Name, CEffectTechnique *EffectTechnique, CRenderableObjectTechnique *OnRenderableObjectTechniqueManager);
            //vars: l_poolName, l_EffectTechnique, l_RenderableObjectTechnique
            AddElement(l_RenderableObjectTechniqueName, l_TechniqueName, l_RenderableObjectTechnique);
            //añadir techniqueElement al pool
            //bool isDefault = m(i).GetBoolProperty("default", false);
            //CRenderableObjectsManager* l_managerInstance = new CRenderableObjectsManager();
            //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
            //AddResource(layerName, l_managerInstance);
            //if (isDefault)
            //{
            //	m_DefaultLayerName = layerName;
            //	m_DefaultRenderableObjectManager = l_managerInstance;
            //}
        }
    }
    //XML para crear todos los CPoolRenderableObjectTechniqueElement del vector, podemos usar AddElement.
    //m_OnRenderableObjectTechniqueManager = NULL;
}


CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement(const std::string &Name, CEffectTechnique *EffectTechnique, CRenderableObjectTechnique *OnRenderableObjectTechniqueManager)
    : m_RenderableObjectTechnique(Name, EffectTechnique)
{
    //m_RenderableObjectTechnique = CRenderableObjectTechnique::CRenderableObjectTechnique(Name, EffectTechnique);
    m_OnRenderableObjectTechniqueManager = OnRenderableObjectTechniqueManager;
    //TODO revisar que le entra el nombre correcto, ahora se le está pasando el nombre de la pool y debería ser el de vertex?
}


CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
    Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
    /*TODO no coge bien el it->second

    for (std::vector<CPoolRenderableObjectTechniqueElement *>::iterator it = m_RenderableObjectTechniqueElements.begin(); it != m_RenderableObjectTechniqueElements.end(); it++) {
            CHECKED_DELETE(it->second);
     }*/
    for (int i = 0; i < m_RenderableObjectTechniqueElements.size(); ++i) {
        CHECKED_DELETE(m_RenderableObjectTechniqueElements[i]);
    }
    m_RenderableObjectTechniqueElements.clear();
    //CHECKED_DELETE(m_RenderableObjectTechniqueElements);
    //no tiene punteros, solo a renderableOBjectManager y esos ya se destruyen desde RenderableObjectLayerManager
}

void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager)
{
    CEffectTechnique * l_EffectTechnique = EFFECTM->GetEffectTechnique(TechniqueName);
    //usar TechniqueName para recuperar CRenderableObjectTechnique
    //CRenderableObjectTechnique  * ejemplo01 = new CRenderableObjectTechnique(Name, ejemplo00);
    CPoolRenderableObjectTechniqueElement * l_nuevoElemento = new CPoolRenderableObjectTechniqueElement(Name, l_EffectTechnique, ROTOnRenderableObjectTechniqueManager);
    m_RenderableObjectTechniqueElements.push_back(l_nuevoElemento);
    //crear un CPoolRenderableObjectTechniqueElement con los datos de entrada
    //añadir a m_RenderableObjectTechniqueElements
}

void CPoolRenderableObjectTechnique::Apply()
{
    for (int i = 0; i < m_RenderableObjectTechniqueElements.size(); ++i) {
        CRenderableObjectTechnique * adios = m_RenderableObjectTechniqueElements[i]->m_OnRenderableObjectTechniqueManager;
        CEffectTechnique * hola = m_RenderableObjectTechniqueElements[i]->m_RenderableObjectTechnique.GetEffectTechnique();
        m_RenderableObjectTechniqueElements[i]->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(m_RenderableObjectTechniqueElements[i]->m_RenderableObjectTechnique.GetEffectTechnique());
    }
}

