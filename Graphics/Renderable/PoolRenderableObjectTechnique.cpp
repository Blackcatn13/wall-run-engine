#include "Renderable\PoolRenderableObjectTechnique.h"
#include "Core\Core.h"
#include "Effects\EffectManager.h"
#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Renderable\PoolRenderableObjectTechnique.h"
#include "Utils\Defines.h"
#include "Core_Utils/MemLeaks.h"

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode)
{
    CXMLTreeNode m = TreeNode;
    std::string l_poolName = m.GetPszISOProperty("name", "", false);
    setName(l_poolName);
    int count = m.GetNumChildren();
    for (int i = 0; i < count; ++i) {
        std::string name = m(i).GetName();
        if (name == "default_technique") {
            unsigned int l_VertexNumber = m(i).GetIntProperty("vertex_type", 0);
            std::string l_TechniqueName = m(i).GetPszISOProperty("technique", "");
            std::string l_RenderableObjectTechniqueName = RENDTECHM->GetRenderableObjectTechniqueNameByVertexType(l_VertexNumber);
            CRenderableObjectTechnique * l_RenderableObjectTechnique = RENDTECHM->GetResource(l_RenderableObjectTechniqueName);
            AddElement(l_RenderableObjectTechniqueName, l_TechniqueName, l_RenderableObjectTechnique);
        }
    }
}


CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement(const std::string &Name, CEffectTechnique *EffectTechnique, CRenderableObjectTechnique *OnRenderableObjectTechniqueManager)
    : m_RenderableObjectTechnique(Name, EffectTechnique)
{
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
}

void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager)
{
    CEffectTechnique * l_EffectTechnique = EFFECTM->GetEffectTechnique(TechniqueName);
    CPoolRenderableObjectTechniqueElement * l_nuevoElemento = new CPoolRenderableObjectTechniqueElement(Name, l_EffectTechnique, ROTOnRenderableObjectTechniqueManager);
    m_RenderableObjectTechniqueElements.push_back(l_nuevoElemento);
}

void CPoolRenderableObjectTechnique::Apply()
{
    for (int i = 0; i < m_RenderableObjectTechniqueElements.size(); ++i) {
        CRenderableObjectTechnique * adios = m_RenderableObjectTechniqueElements[i]->m_OnRenderableObjectTechniqueManager;
        CEffectTechnique * hola = m_RenderableObjectTechniqueElements[i]->m_RenderableObjectTechnique.GetEffectTechnique();
        m_RenderableObjectTechniqueElements[i]->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(m_RenderableObjectTechniqueElements[i]->m_RenderableObjectTechnique.GetEffectTechnique());
    }
}

