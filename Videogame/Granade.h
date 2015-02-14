#ifndef CGRANADE_H
#define CGRANADE_H

class CPhysicActor;
class CStaticMesh;
class CPhysicUserData;

class CGranade {
private:
	float m_TimeAux;
	float m_MinForce;
	float m_MaxForce;
	float m_Force;
	float m_ExpRadius;
	float m_ExplosionTime;
	bool exists;
	CPhysicActor* actor;
	CStaticMesh *mesh;
	CPhysicUserData	*m_PhysicUserData;
public:
	CGranade();
	~CGranade();
	void Start();
	void Update(float);
	void Render();

};

#endif