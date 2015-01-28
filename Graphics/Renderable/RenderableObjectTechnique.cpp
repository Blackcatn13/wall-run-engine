#include "Renderable\RenderableObjectTechnique.h"

CRenderableObjectTechnique::CRenderableObjectTechnique()
{
}

CRenderableObjectTechnique::CRenderableObjectTechnique(const std::string &Name, CEffectTechnique*EffectTechnique)
{
	setName(Name);
	m_EffectTechnique = EffectTechnique;
}


void CRenderableObjectTechnique::SetEffectTechnique(CEffectTechnique *EffectTechnique)
{
	m_EffectTechnique = EffectTechnique;
}

CEffectTechnique * CRenderableObjectTechnique::GetEffectTechnique() const{
	return m_EffectTechnique;
}

/*La clase CRenderableObjectTechnique nos permitir� enlazar un
CRenderableObject con una CEffectTechnique, a partir de ahora los
CRenderableObject contendr� una referencia a esta clase y coger�n la technique
que deber� utilizar para renderizarse seg�n esta clase.
*/


/*<renderable_object_techniques>
	<pool_renderable_object_technique name="dummy_manager">
		<default_technique vertex_type="31" technique="NormalMapAmbientLightNormalTextureVertexTechnique"/>
		<default_technique vertex_type="19" technique="AmbientLightNormalTextureVertexTechnique"/>
		<default_technique vertex_type="159" technique="Cal3DTechnique"/>
		<default_technique vertex_type="35" technique="AmbientLightNormalTextureVertexTechnique"/>
	</pool_renderable_object_technique>
</renderable_object_techniques>
*/