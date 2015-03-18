#include "Particles\ParticleEmitter.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include "Math\Color.h"
#include "Math\Vector4.h"
#include "GraphicsManager.h"

CParcitleEmitter::CParcitleEmitter(CXMLTreeNode  &node):
	m_CurrentTime(.0f)
	{
	 std::string name = node.GetName();
	 if (name == "particle"){
		 m_MinSpeed = node.GetFloatProperty("min_speed", .0f, false);
		 m_MaxSpeed = node.GetFloatProperty("max_speed", .0f, false);
		 m_MinParticles = node.GetIntProperty("min_particles", 0, false);
		 m_MaxParticles = node.GetIntProperty("max_particles", 0, false);
		 m_MinEmissionTime = node.GetFloatProperty("min_emission_time", .0f, false);
		 m_MaxEmissionTime = node.GetFloatProperty("max_emission_time", .0f, false);
		 m_MinAge = node.GetFloatProperty("min_age", .0f, false);
		 m_MaxAge = node.GetFloatProperty("max_age", .0f, false);
		 m_MinSize = node.GetFloatProperty("min_size", .0f, false);
		 m_MaxSize = node.GetFloatProperty("max_size", .0f, false);
		 Vect4f l_Color1 = node.GetVect4fProperty("color_1", v4fZERO, false);
		 m_Color1 = CColor(l_Color1);
		 Vect4f l_Color2 = node.GetVect4fProperty("color_2", v4fZERO, false);
		 m_Color2 = CColor(l_Color2);
		 m_vPos = node.GetVect3fProperty("v_Pos", v3fZERO, false);
		 m_vSpawnDir1 = node.GetVect3fProperty("spawn_dir1", v3fZERO, false);
		 m_vSpawnDir2 = node.GetVect3fProperty("spawn_dir2", v3fZERO, false);
		 m_sTexture = node.GetPszISOProperty("texture", "", false);
	 }
	 m_TimeNextParticle =  (rand()) / (static_cast <float> (m_MaxEmissionTime/m_MinEmissionTime));
}
void CParcitleEmitter::Render(CGraphicsManager *RM)
{

	/*for(int i = 0; i<m_Particles.size(); ++i){
		m_Particles[i]->Render(RM);
	}	*/	
}


void CParcitleEmitter::Update(float dt)
{
	m_CurrentTime = m_CurrentTime +1 *dt;
	//for(int i = 0; i<m_Particles.size(); ++i){
	//	if m_CurrentTime > m_Particles[i]->GetAge() delete m_Particles[i]
	//}	

	//if m_CurrentTime > m_TimeNextParticle && m_Particles.size() < m_MaxParticles
	//{
	//	//Creo particula nueva
	//	m_TimeLastParticleEmited = m_CurrentTime + random(min_emission_time, max_emission_time)
	//	//new Particule()
	//	////calculas los randoms de las propiedades de la particula
	//	//Particule->

	//	//add to vector m_Particles

	//}
	//m_Particles[i]->SetAge(m_CurrentTime + random(min_age, max_age))


	//for(int i = 0; i<m_Particles.size(); ++i){
	//	m_Particles[i]->Update(dt)
	//}	
}