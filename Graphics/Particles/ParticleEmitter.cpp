#include "Particles\ParticleEmitter.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include "Math\Color.h"
#include "Math\Vector4.h"
#include "GraphicsManager.h"
#include "Particle.h"

CParticleEmitter::CParticleEmitter(CXMLTreeNode  &node):
	m_CurrentTime(.0f)
	{
	 std::string name = node.GetName();
	 if (name == "particle"){
		 m_MinSpeed = node.GetFloatProperty("min_speed", -1.0f, false);
		 m_MaxSpeed = node.GetFloatProperty("max_speed", -1.0f, false);
		 m_MinParticles = node.GetIntProperty("min_particles", -1, false);
		 m_MaxParticles = node.GetIntProperty("max_particles", -1, false);
		 m_MinEmissionTime = node.GetFloatProperty("min_emission_time", -1.0f, false);
		 m_MaxEmissionTime = node.GetFloatProperty("max_emission_time", -1.0f, false);
		 m_MinAge = node.GetFloatProperty("min_age", -1.0f, false);
		 m_MaxAge = node.GetFloatProperty("max_age", -1.0f, false);
		 m_MinSize = node.GetFloatProperty("min_size", -1.0f, false);
		 m_MaxSize = node.GetFloatProperty("max_size", -1.0f, false);
		 Vect4f l_Color1 = node.GetVect4fProperty("color_1", v4fZERO, false);
		 m_Color1 = CColor(l_Color1);
		 Vect4f l_Color2 = node.GetVect4fProperty("color_2", v4fZERO, false);
		 m_Color2 = CColor(l_Color2);
		 m_vPos = node.GetVect3fProperty("v_Pos", v3fZERO, false);
		 m_vSpawnDir1 = node.GetVect3fProperty("spawn_dir1", v3fZERO, false);
		 m_vSpawnDir2 = node.GetVect3fProperty("spawn_dir2", v3fZERO, false);
		 m_sTexture = node.GetPszISOProperty("texture", "", false);
	 }
	 m_Particles = new CRecyclingArray<CParticle>(m_MaxParticles);

	// m_TimeNextParticle =  (rand()) / (static_cast <float> (m_MaxEmissionTime/m_MinEmissionTime));
	 
}

CParticleEmitter::~CParticleEmitter(){}

void CParticleEmitter::Render(CGraphicsManager *RM)
{
	/*for(int i = 0; i<m_Particles.size(); ++i){
		m_Particles[i]->Render(RM);
	}	*/	
}


void CParticleEmitter::Update(float dt)
{



	/*

	
	
	“Reservar” una nueva posición:
	CParticle* part = m_Particles.New();
	“Eliminar” una posición:
	m_Particles.Free(cont);
	Recorrer el array:
	for (uint32 cont=0; cont<m_Particles.GetTotalElements(); cont++)
	{
			if (!m_Particles.IsFree(cont))
			{       
				CParticle* par = m_Particles.GetAt(cont);
			 }
	 }
	Saber si hay espacio libre
	 if (m_Particles.GetNumFreeElements() > 0)....

	*/



//	m_CurrentTime = m_CurrentTime +1 *dt;



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