//-----------------------------------------------------------------------
// Vector2 class
// This class defines a 3d vector
//-----------------------------------------------------------------------
#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include "MathTypes.h"
#include "MathUtils.h"
#include "Vector2.h"
#include "Utils/Types.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
///
// @brief  Vector de tres dimensiones
// @Note1 \verbatim
// El espacio 3D en el que se definen estos vectores, que también incluyen la funcionalidad de punto,
// es un espacio que sigue un criterio de sistema de referencia de "mano derecha".
// 
//                     z |
//                       |
//                       |  / y
//                       | /
//                       |/
//                        --------  x
// 
// Este criterio es utilizado y respetado en todas las operaciones entre objetos de la librería
// matematica.
// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Vector3
{
public:
  //-----------------
  // DATOS PUBLICOS
  //-----------------
  T x, y, z;
      
public: 
  //-------------------
  // METODOS PUBLICOS
  //-------------------

  // Construcción   
  inline Vector3 ();
  inline Vector3 (const Vector3<T>& otro);
  inline Vector3 (const T tx, const T ty, const T tz);
  inline Vector3 (const T escalar);
  
  
  // Operadores aritméticos   
  inline        Vector3<T>  operator + (const Vector3<T>& otro) const;
  inline        Vector3<T>  operator - (const Vector3<T>& otro) const;
  inline        Vector3<T>  operator * (const T escalar) const;
  inline        Vector3<T>  operator / (const T escalar) const;
  inline        Vector3<T>  operator + (const T escalar) const;
  inline        Vector3<T>  operator - (const T escalar) const;
  inline        Vector3<T>  operator - () const;
  inline        Vector3<T>& operator + ();
  inline const  Vector3<T>& operator + () const;

      
  // Operadores aritméticos de actualización   
  inline Vector3<T>& operator += (const Vector3<T>& otro);
  inline Vector3<T>& operator -= (const Vector3<T>& otro);
  inline Vector3<T>& operator *= (const T escalar);
  inline Vector3<T>& operator /= (const T escalar);
  inline Vector3<T>& operator += (const T escalar);
  inline Vector3<T>& operator -= (const T escalar);


  // Operadores y funciones de asignación
  inline Vector3<T>& operator ()  (const T tx, const T ty, const T tz);
  inline void        Set          (const T tx, const T ty, const T tz);
  inline void        SetZero      ();


  // Coordenadas polares
  void  SetFromPolar (const T longitude, const T latitude, const T length);
  void  GetPolar     (T& longitude, T& latitude, T& length) const;


  // Producto escalar (*) y producto vectorial (^)
  inline T          operator * (const Vector3<T>& otro) const;
  inline Vector3<T> operator ^ (const Vector3<T>& otro) const;


  // Operadores y funciones de comparacion
  inline bool operator ==       (const Vector3<T>& otro) const;
  inline bool operator !=       (const Vector3<T>& otro) const;
  inline bool IsEqualEpsilon    (const Vector3<T>& otro, const T Epsilo = Epsilon<T>()) const;//TODO CW quito la n para que no sea igual
  inline bool IsNotEqualEpsilon (const Vector3<T>& otro, const T Epsilo = Epsilon<T>()) const;//TODO CW quito la n para que no sea igual


  // Producto por componentes (escalado)
  inline Vector3<T>& Scale     (const Vector3<T>& otro);
  inline Vector3<T>  GetScaled (const Vector3<T>& otro) const;


  // Establecimiento condicional
  inline const Vector3<T>& SetIfMinComponents (const Vector3<T>& otro);
  inline const Vector3<T>& SetIfMaxComponents (const Vector3<T>& otro);

  
  // Operador de acceso []
  inline T  operator [] (int i) const;
  inline T& operator [] (int i);


  // Proyecciones
  inline Vector2<T> GetProjXY () const;
  inline Vector2<T> GetProjYZ () const;
  inline Vector2<T> GetProjZX () const;


  // Funciones de la longitud 
  inline Vector3<T>& Normalize     (const T tk = One<T>());
  inline Vector3<T>  GetNormalized (const T tk = One<T>()) const;
  inline T           Length        () const;
  inline T           SquaredLength () const;
  inline T           Distance      (const Vector3<T>& rhs) const;
  inline T           SqDistance    (const Vector3<T>& rhs) const;

  // Rotaciones en los ejes principales
  inline Vector3<T>& RotateX     (const T angle);
  inline Vector3<T>  GetRotatedX (const T angle) const;
  inline Vector3<T>& RotateY     (const T angle);
  inline Vector3<T>  GetRotatedY (const T angle) const;
  inline Vector3<T>& RotateZ     (const T angle);
  inline Vector3<T>  GetRotatedZ (const T angle) const;
  inline T           GetAngleX   () const;
  inline T           GetAngleY   () const;
  inline T           GetAngleZ   () const;
  inline void        GetAngles   (T& angX, T& angY, T& angZ) const;
         

  // Interpolación lineal
  inline Vector3<T>& Lerp    (const Vector3<T>& otro, const T t);
  inline Vector3<T>  GetLerp (const Vector3<T>& otro, const T t) const;
};

//////////////////////////////////////////////
// OPERADORES EXTERNOS
//////////////////////////////////////////////
// Operadores aritméticos para poder usar orden inverso en las expresiones
template<typename T> inline Vector3<T>  operator * (const T escalar, const Vector3<T>& otro);
template<typename T> inline Vector3<T>  operator / (const T escalar, const Vector3<T>& otro);
template<typename T> inline Vector3<T>  operator + (const T escalar, const Vector3<T>& otro);
template<typename T> inline Vector3<T>  operator - (const T escalar, const Vector3<T>& otro);

//////////////////////////////////////////////
// FUNCIONES EXTERNAS
//////////////////////////////////////////////
template<typename T> inline Vector3<T> VectorMinComponents (const Vector3<T>& uno, const Vector3<T>& otro);
template<typename T> inline Vector3<T> VectorMaxComponents (const Vector3<T>& uno, const Vector3<T>& otro);

// Fichero con las definiciones de las funciones inline
#include "Vector3.inl"


////////////////////////////////////////////////////////////////////////////////////////
// TIPOS DE VECTORES 3D CON TIPOS CONCRETOS DE DATOS
////////////////////////////////////////////////////////////////////////////////////////
typedef Vector3<float>  Vect3f;
typedef Vector3<double> Vect3d;
typedef Vector3<int32>  Vect3i;
typedef Vector3<uint32> Vect3u;
typedef Vector3<uint16> Vect3w;


////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES FLOAT
////////////////////////////////////////////////////////////////////////////////////////
extern const Vect3f  v3fX;
extern const Vect3f  v3fY;
extern const Vect3f  v3fZ;
extern const Vect3f  v3fONE;
extern const Vect3f  v3fNEGX;
extern const Vect3f  v3fNEGY;
extern const Vect3f  v3fNEGZ;
extern const Vect3f  v3fNEGONE;
extern const Vect3f  v3fZERO;
extern const Vect3f  v3fMAX;
extern const Vect3f  v3fMIN;
extern const Vect3f  v3fTOP;
extern const Vect3f  v3fBOTTOM;
extern const Vect3f  v3fFRONT;
extern const Vect3f  v3fRIGHT;
extern const Vect3f  v3fUNIT;

////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES DOUBLE
////////////////////////////////////////////////////////////////////////////////////////
extern const Vect3d  v3dX;
extern const Vect3d  v3dY;
extern const Vect3d  v3dZ;
extern const Vect3d  v3dONE;
extern const Vect3d  v3dNEGX;
extern const Vect3d  v3dNEGY;
extern const Vect3d  v3dNEGZ;
extern const Vect3d  v3dNEGONE;
extern const Vect3d  v3dZERO;
extern const Vect3d  v3dMAX;
extern const Vect3d  v3dMIN;
extern const Vect3d  v3dTOP;
extern const Vect3d  v3dFRONT;
extern const Vect3d  v3dRIGHT;
extern const Vect3d  v3dUNIT;

////////////////////////////////////////////////////////////////////////////////////////
// CONSTANTES INT
////////////////////////////////////////////////////////////////////////////////////////
extern const Vect3i  v3iX;
extern const Vect3i  v3iY;
extern const Vect3i  v3iZ;
extern const Vect3i  v3iONE;
extern const Vect3i  v3iNEGX;
extern const Vect3i  v3iNEGY;
extern const Vect3i  v3iNEGZ;
extern const Vect3i  v3iNEGONE;
extern const Vect3i  v3iZERO;
extern const Vect3i  v3iTOP;
extern const Vect3i  v3iFRONT;
extern const Vect3i  v3iRIGHT;
extern const Vect3i  v3iUNIT;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
