#include "Billboard.h"
#include "GraphicsManager.h"
#include "Core\Core.h"
#include "Camera\CameraController.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "XML\XMLTreeNode.h"
#include "Texture\Texture.h"
#include "Math\Color.h"
#include "Utils\Defines.h"

CBillboard::CBillboard(CXMLTreeNode &node)
  : m_size(node.GetFloatProperty("size", .0f	, false))
  , m_position(node.GetVect3fProperty("position", v3fZERO	, false))
  , m_Color1(CColor((node.GetVect4fProperty("color", v4fZERO	, false))))

{
  std::string tempText = node.GetPszISOProperty("texture", ""	, false);
  if (tempText != "") {
    m_Texture = new CTexture();
    m_Texture->Load(node.GetPszISOProperty("texture", ""	, false));
  } else {
    m_Texture = NULL;
  }
}

CBillboard::CBillboard(float size)
  : m_size(size)
  , m_position(v3fZERO) {
}

CBillboard::CBillboard(float size, Vect3f pos)
  : m_size(size)
  , m_position(pos) {
}

CBillboard::~CBillboard() {
  CHECKED_DELETE(m_Texture);
}

void CBillboard::Render(CGraphicsManager *GM) {
  CCamera *actCam = CAMCONTM->getActiveCamera();
  Vect3f up_cam = actCam->GetVecUp().Normalize();
  Vect3f up =  up_cam * m_size / 2;
  Vect3f direction = actCam->GetDirection().Normalize();
  Vect3f right = (up_cam ^ direction) * m_size / 2;
  Vect3f ul = m_position + up - right;
  Vect3f ur = m_position + up + right;
  Vect3f dl = m_position - up - right;
  Vect3f dr = m_position - up + right;
  //GM->DrawQuad3D(ul, ur, dl, dr, m_Color1);

  if (m_Texture != NULL)
    GM->DrawQuad3D(ul, ur, dl, dr,  m_Texture, m_Color1);
  else
    GM->DrawQuad3D(ul, ur, dl, dr, m_Color1);
}


void CBillboard::Update(float ElapsedTime) {
}