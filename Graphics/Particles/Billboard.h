#ifndef BILLBOARD_H_
#define BILLBOARD_H_

#include "Math\Vector3.h"

class CGraphicsManager;

class CBillboard {
public:
	CBillboard();
	CBillboard(int size);
	CBillboard(int size, Vect3f pos);
	~CBillboard();
	void Render(CGraphicsManager* GM);
	int getSize() {return m_size;}
	void setSize(int size) {m_size = size;}
	Vect3f getPosition() {return m_position;}
	void setPosition(Vect3f position) {m_position = position;}
private:
	int		m_size;
	Vect3f	m_position;
};

#endif // BILLBOARD_H_