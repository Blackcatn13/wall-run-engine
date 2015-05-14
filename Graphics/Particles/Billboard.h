#ifndef BILLBOARD_H_
#define BILLBOARD_H_

#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Utils\Defines.h"

class CGraphicsManager;
class CXMLTreeNode;
class CTexture;

class CBillboard {
 public:
  CBillboard() {}
  CBillboard(CXMLTreeNode &node);
  CBillboard(float size);
  CBillboard(float size, Vect3f pos);
  ~CBillboard();
  void Update(float ElapsedTime);
  void Render(CGraphicsManager *GM);
  float getSize() {return m_size;}
  void setSize(float size) {m_size = size;}
  Vect3f getPosition() {return m_position;}
  void setPosition(Vect3f position) {m_position = position;}
  GET_SET(CTexture *, Texture)
 protected:
  float			m_size;
  Vect3f		m_position;
  CTexture		*m_Texture;
  CColor		m_Color1;
};

#endif // BILLBOARD_H_