//-----------------------------------------------------------------------
// Vector2 class
// This class defines a 4d vector
//-----------------------------------------------------------------------
#ifndef __VECTOR4_H__
#define __VECTOR4_H__


#include "MathTypes.h"
#include "MathUtils.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Utils/Types.h"


template<typename T>
class Vector4
{
public:
  //------------------
  // DATOS PUBLICOS
  //------------------
  T x, y, z, w;
  
public:
  //------------------
  // DATOS PUBLICOS
  //------------------

  // Construcción   
  inline Vector4 ();
  inline Vector4 (const Vector4<T>& otro);
  inline Vector4 (const Vector3<T>& otro);
  inline Vector4 (const Vector3<T>& otro, const T tw);
  inline Vector4 (const T tx, const T ty, const T tz, const T tw);
  inline Vector4 (const T escalar);


  // Operadores aritméticos   
  inline        Vector4<T>  operator + (const Vector4<T>& otro) const;
  inline        Vector4<T>  operator - (const Vector4<T>& otro) const;
  inline        Vector4<T>  operator * (const T escalar) const;
  inline        Vector4<T>  operator / (const T escalar) const;
  inline        Vector4<T>  operator + (const T escalar) const;
  inline        Vector4<T>  operator - (const T escalar) const;
  inline        Vector4<T>  operator - () const;
  inline        Vector4<T>& operator + ();
  inline const  Vector4<T>& operator + () const;


  // Operadores aritméticos de actualización   
  inline Vector4<T>& operator += (const Vector4<T>& otro);
  inline Vector4<T>& operator -= (const Vector4<T>& otro);
  inline Vector4<T>& operator *= (const T escalar);
  inline Vector4<T>& operator /= (const T escalar);
  inline Vector4<T>& operator += (const T escalar);
  inline Vector4<T>& operator -= (const T escalar);


  // Operadores y funciones de asignación
  //inline Vector4<T>& operator =  (const Vector4<T>& otro);
  inline Vector4<T>& operator () (const T tx, const T ty, const T tz, const T tw);
  inline void        Set         (const T tx, const T ty, const T tz, const T tw);
  inline void        SetZero     ();
  

  // Producto escalar (*)
  inline T operator * (const Vector4<T>& otro) const;


  // Operadores y funciones de comparacion
  inline bool operator ==       (const Vector4<T>& otro) const;
  inline bool operator !=       (const Vector4<T>& otro) const;
  inline bool IsEqualEpsilon    (const Vector4<T>& otro) const;
  inline bool IsNotEqualEpsilon (const Vector4<T>& otro) const;


  // Producto por componentes (escalado)
  inline Vector2<T>& Scale     (const Vector2<T>& otro);
  inline Vector2<T>  GetScaled (const Vector2<T>& otro) const;


  // Establecimiento condicional
  inline const Vector2<T>& SetIfMinComponents (const Vector2<T>& otro);
  inline const Vector2<T>& SetIfMaxComponents (const Vector2<T>& otro);


  // Operador de acceso []
  inline T  operator [] (int i) const;
  inline T& operator [] (int i);


  // Funciones de la longitud 
  inline Vector4<T>& Normalize     (const T tk = One<T>());
  inline Vector4<T>  GetNormalized () const;
  inline T           Length        () const;
  inline T           SquaredLength () const;

  // Interpolación lineal
  inline Vector4<T>& Lerp    (const Vector4<T>& otro, const T t);
  inline Vector4<T>  GetLerp (const Vector4<T>& otro, const T t) const;
};

//////////////////////////////////////////////
/// OPERADORES EXTERNOS
//////////////////////////////////////////////
// Operadores aritméticos friend para poder usar orden inverso en las expresiones
template<typename T> inline Vector4<T>  operator * (const T escalar, const Vector4<T>& otro);
template<typename T> inline Vector4<T>  operator / (const T escalar, const Vector4<T>& otro);
template<typename T> inline Vector4<T>  operator + (const T escalar, const Vector4<T>& otro);
template<typename T> inline Vector4<T>  operator - (const T escalar, const Vector4<T>& otro);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fichero con las definiciones de las funciones inline
#include "Vector4.inl"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE VECTORES 4D CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Vector4<float>  Vect4f;
typedef Vector4<double> Vect4d;
typedef Vector4<int32>  Vect4i;
typedef Vector4<uint32> Vect4u;
typedef Vector4<uint8>  Vect4u8;
typedef Vector4<uint16> Vect4w;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES FLOAT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const Vect4f  v4fX;
extern const Vect4f  v4fY;
extern const Vect4f  v4fZ;
extern const Vect4f  v4fW;
extern const Vect4f  v4fONE;
extern const Vect4f  v4fNEGX;
extern const Vect4f  v4fNEGY;
extern const Vect4f  v4fNEGZ;
extern const Vect4f  v4fNEGW;
extern const Vect4f  v4fNEGONE;
extern const Vect4f  v4fZERO;
extern const Vect4f  v4fMAX;
extern const Vect4f  v4fMIN;
extern const Vect4f  v4fTOP;
extern const Vect4f  v4fFRONT;
extern const Vect4f  v4fRIGHT;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES DOUBLE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const Vect4d  v4dX;
extern const Vect4d  v4dY;
extern const Vect4d  v4dZ;
extern const Vect4d  v4dW;
extern const Vect4d  v4dONE;
extern const Vect4d  v4dNEGX;
extern const Vect4d  v4dNEGY;
extern const Vect4d  v4dNEGZ;
extern const Vect4d  v4dNEGW;
extern const Vect4d  v4dNEGONE;
extern const Vect4d  v4dZERO;
extern const Vect4d  v4dMAX;
extern const Vect4d  v4dMIN;
extern const Vect4d  v4dTOP;
extern const Vect4d  v4dFRONT;
extern const Vect4d  v4dRIGHT;

////////////////////////////////////////////////////////////////////////////////////////
// CONSTANTES INT
////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
