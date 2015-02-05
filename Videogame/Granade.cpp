#include "Granade.h"
#include "Actor\PhysicActor.h"
#include "StaticMeshes\StaticMesh.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Utils\PhysicUserData.h"
#include "PhysicsManager.h"
#include "Core\Core.h"
#include "Utils\Defines.h"
#include "GraphicsManager.h"
#include "ActionToInput.h"

CGranade::CGranade() : 
	m_TimeAux(0),
	m_MinForce(10),
	m_MaxForce(10000),
	m_Force(10),
	m_ExpRadius(30),
	m_ExplosionTime(5),
	exists(false)
{
	mesh = SMESHM->GetResource("Bomb");
}

CGranade::~CGranade()
{
	CHECKED_DELETE(m_PhysicUserData);
	CHECKED_DELETE(actor);
	CHECKED_DELETE(mesh);
}

void CGranade::Start()
{
	m_PhysicUserData = new CPhysicUserData("BOMB");
	m_PhysicUserData->SetPaint(true);
	actor = new CPhysicActor(m_PhysicUserData);
	actor->AddSphereShape(1, CAMCONTM->getActiveCamera()->GetEye(),Vect3f(0, 1, 0));
	actor->CreateBody(0.5f);
}

void CGranade::Update(float dt)
{
	if (ACT2IN->DoAction("KeyMoveForward") && !exists) {
			Start();
		PHYSXM->AddPhysicActor(actor);
		CPhysicActor* aux = PHYSXM->GetActor("BOMB");
		aux->AddVelocityAtPos(CAMCONTM->getActiveCamera()->GetDirection().Normalize(), v3fZERO, m_Force);
		exists = true;
		m_TimeAux = 0;
		m_Force = m_MinForce;
	}
	if (ACT2IN->DoAction("KeyMoveBack")) {
		m_Force += 10 * dt;
		if(m_Force >= m_MaxForce) 
			m_Force = m_MaxForce;
	}
	if (m_TimeAux >= m_ExplosionTime && exists == true) {
		
		std::vector<CPhysicUserData*> impacts;
		PHYSXM->OverlapSphereActorGrenade(m_ExpRadius, actor->GetPosition(), impacts, 10000);
		exists = false;
		PHYSXM->ReleasePhysicActor(actor);
	}
	m_TimeAux += dt;

}

void CGranade::Render()
{
	if(exists) {
		Mat44f mat;
		actor->GetMat44(mat);
		Mat44f scale = m44fIDENTITY;
		scale.SetScale(0.2, 0.2, 0.2);
		GRAPHM->SetTransform(mat * scale);
		mesh->Render(GRAPHM);
		mat = m44fIDENTITY;
		GRAPHM->SetTransform(mat);
	}
}
