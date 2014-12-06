//-----------------------------------------------------------------------
// CColor class
/// Describe un color. Internamente implementado como un Vect4f que almacena los valores de rojo verde azul y
/// alfa como valores en coma flotante. Cada uno de ellos con valores de 0.0f a 1.0f. En el caso de las
/// componentes rojo verde y azul, un 0.0f indica ausencia de esa componente y de 1.0f indica la cantidad
/// maxima. Un alfa de 1.0f indica color opaco y uno de .0f indica que es completamente transparente.
//-----------------------------------------------------------------------
#ifndef COLOR_H_
#define COLOR_H_

#include "Vector4.h"

class CColor
{
private:
  Vect4f m_Color; // el color como un vect4f, por conveniencia para el motor.

public:
  // ------------
  // Construcci??n
  // ------------
  explicit CColor ()                                                       :m_Color(1.0f, 1.0f, 1.0f, 1.0f)  {}
  explicit CColor (const Vect4f& vect)                                     :m_Color(vect)                    {}
  explicit CColor (float red, float green, float blue, float alpha = 1.0f) :m_Color(red, green, blue, alpha) {}
	//inline   CColor (uint32 argb);

  // --------------------
  // Acceso a componentes
  // --------------------
  inline void          Set(float red, float green, float blue, float alpha = 1.0f)  { m_Color[0]=red; m_Color[1]=green; m_Color[2]=blue; m_Color[3]=alpha;  }

  inline float         GetRed         ()                   const { return m_Color[0]; }
  inline void          SetRed         (float red)                { m_Color[0] = red; }

  inline float         GetGreen       ()                   const { return m_Color[1]; }
  inline void          SetGreen       (float green)              { m_Color[1] = green; }

  inline float         GetBlue        ()                   const { return m_Color[2]; }
  inline void          SetBlue        (float blue)               { m_Color[2] = blue; }

  inline float         GetAlpha       ()                   const { return m_Color[3]; }
  inline void          SetAlpha       (float alpha)              { m_Color[3] = alpha; }

  inline const Vect4f& GetArgb        ()                   const { return m_Color; }
  inline void          SetArgb        (const Vect4f& argb)       { m_Color = argb; }

	inline uint32        GetUint32Abgr	 ()										const;
	inline uint32        GetUint32Argb	 ()										const;

	inline void          SetUint32Argb	 (int32 argb);
	inline void          SetUint32Abgr	 (int32 argb);

	inline CColor&			 Clamp();

	inline CColor&			 Lerp(const CColor& other, float f);

	// Operadores aritméticos   
	inline        CColor  operator + (const CColor& otro)	const;
	inline        CColor  operator - (const CColor& otro)	const;
	inline        CColor  operator * (const CColor& otro)	const;
	inline        CColor  operator * (float escalar)			const;

	// Operadores aritméticos de actualización   
	inline CColor& operator += (const CColor& otro);
	inline CColor& operator -= (const CColor& otro);
	inline CColor& operator *= (const CColor& otro);
	inline CColor& operator *= (float escalar);

private:
  inline uint8 FloatToByte (float fValue) const;
  inline float ByteToFloat (uint8 uValue) const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// incluimos las funciones inline
#include "Color.inl"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// CONSTANTES DE COLOR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const CColor colBLACK;
extern const CColor colWHITE;
extern const CColor colRED;
extern const CColor colGREEN;
extern const CColor colBLUE;
extern const CColor colMAGENTA;
extern const CColor colYELLOW;
extern const CColor colCYAN;


#endif
