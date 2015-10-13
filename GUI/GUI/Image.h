//----------------------------------------------------------------------------------
// CSlider class
// Author: Enric Vergara
//
// Description:
// A Image..
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_IMAGE_H
#define INC_IMAGE_H

#include "GraphicsManager.h"
#include "GuiElement.h"
#include "Math/Color.h"
#include <map>
#include <vector>

//---Forward Declarations---
class CTexture;
//--------------------------

class CImage: public CGuiElement {
 private:
  typedef std::map<std::string, CTexture *>						tTexturesMap;
  typedef std::vector<CTexture *>::iterator						tItTexturesVec;
  typedef std::vector<CTexture *>											tTexturesVec;

 public:
  CImage( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
          const Vect2f position_percent, std::string lit = "", uint32 textHeightOffset = 0, uint32 textWidthOffset = 0,
          bool isVisible = true, bool isActive = true);

  virtual ~CImage() {/*NOTHING*/;}

  //---------------CGuiElement Interface----------------------
  virtual void	Render									(CGraphicsManager *renderManager, CFontManager *fm);
  virtual void	Update									(CInputManager *intputManager, float elapsedTime);
  virtual void	OnClickedChild					(const std::string &name) {/*NOTHING*/;}


  //---------------CImage Interface---------------------------
  void					SetTexture							(CTexture *texture, std::string name );
  void					SetActiveTexture				(const std::string &inName) {m_sActiveTexture = inName;}
  std::string	&GetActiveTexture				() {return m_sActiveTexture;}
  void					PlayAnimation						(float timePerImage, bool loop);
  void					SetFlip									(ETypeFlip flip) {m_eFlip = flip;}
  ETypeFlip			GetFlip									() const {return m_eFlip;}
  bool					IsQuadrant							() const {return m_bIsQuadrant;}
  void					SetQuadrant							(bool flag) {m_bIsQuadrant = flag;}
  void					SetBackGround						(bool flag) {m_bIsBackGround = flag;}



 private:
  tTexturesMap		m_Textures;
  tItTexturesVec	m_itVecTextures;
  tTexturesVec		m_VecTextures;
  CColor					m_Color;
  std::string			m_sActiveTexture;
  bool						m_bAnimated;
  bool						m_bLoop;
  float						m_fTimePerImage;
  float						m_fCounter;
  ETypeFlip				m_eFlip;
  bool						m_bIsQuadrant;
  bool						m_bIsBackGround;
};

#endif //INC_IMAGE_H