//-----------------------------------------------------------------------
// Vector2 class
// This class defines a 2d vector
//-----------------------------------------------------------------------
#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include "MathTypes.h"
#include "MathUtils.h"
#include "Utils/Types.h"


template<typename T>
class Vector2
{
public:
    T x, y;

    // Construcción
    inline Vector2 ();
    inline Vector2 (const Vector2<T>& otro);
    inline Vector2 (const T fx, const T fy);
    inline Vector2 (const T escalar);


    // Operadores aritméticos
    inline        Vector2<T>  operator + (const Vector2<T>& otro) const;
    inline        Vector2<T>  operator - (const Vector2<T>& otro) const;
    inline        Vector2<T>  operator * (const T escalar) const;
    inline        Vector2<T>  operator / (const T escalar) const;
    inline        Vector2<T>  operator + (const T escalar) const;
    inline        Vector2<T>  operator - (const T escalar) const;
    inline        Vector2<T>  operator - () const;
    inline        Vector2<T>& operator + ();
    inline const  Vector2<T>& operator + () const;


    // Operadores aritméticos de actualización
    inline Vector2<T>& operator += (const Vector2<T>& otro);
    inline Vector2<T>& operator -= (const Vector2<T>& otro);
    inline Vector2<T>& operator *= (const T escalar);
    inline Vector2<T>& operator /= (const T escalar);
    inline Vector2<T>& operator += (const T escalar);
    inline Vector2<T>& operator -= (const T escalar);


    // Operadores y funciones de asignación
    inline Vector2<T>& operator ()  (const T tx, const T ty);
    inline void        Set          (const T tx, const T ty);
    inline void        SetZero      ();


    // Coordenadas polares
    void  SetFromPolar (const T ang, const T length);
    void  GetPolar     (T& ang, T& length) const;


    // Producto escalar (*)
    inline T operator * (const Vector2<T>& otro) const;


    // Operadores y funciones de comparacion
    inline bool operator ==       (const Vector2<T>& otro) const;
    inline bool operator !=       (const Vector2<T>& otro) const;
    inline bool IsEqualEpsilon    (const Vector2<T>& otro) const;
    inline bool IsNotEqualEpsilon (const Vector2<T>& otro) const;


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
    inline Vector2<T>& Normalize     (const T tk = One<T>());
    inline Vector2<T>  GetNormalized () const;
    inline T           Length        () const;
    inline T           SquaredLength () const;


    // Rotaciones
    inline Vector2<T>& Rotate     (const T angle);
    inline Vector2<T>  GetRotated (const T angle) const;
    inline T           GetAngle   () const;


    // Interpolación lineal
    inline Vector2<T>& Lerp    (const Vector2<T>& otro, const T t);
    inline Vector2<T>  GetLerp (const Vector2<T>& otro, const T t) const;

    // Funciones auxiliares
    inline Vector2<T>& Add_Max (const Vector2<T>& otro);
};

//--------------------------------------------------------------------------
// OPERADORES EXTERNOS
//--------------------------------------------------------------------------
// Operadores para poder usar orden inverso en las expresiones
template<typename T> inline Vector2<T>  operator * (const T escalar, const Vector2<T>& otro);
template<typename T> inline Vector2<T>  operator / (const T escalar, const Vector2<T>& otro);
template<typename T> inline Vector2<T>  operator + (const T escalar, const Vector2<T>& otro);
template<typename T> inline Vector2<T>  operator - (const T escalar, const Vector2<T>& otro);

//--------------------------------------------------------------------------
// FUNCIONES EXTERNAS
//--------------------------------------------------------------------------
template<typename T> inline Vector2<T> VectorMinComponents (const Vector2<T>& uno, const Vector2<T>& otro);
template<typename T> inline Vector2<T> VectorMaxComponents (const Vector2<T>& uno, const Vector2<T>& otro);

// Fichero con las definiciones de las funciones inline
#include "Vector2.inl"

//--------------------------------------------------------------------------
// TIPOS DE VECTORES 2D CON TIPOS CONCRETOS DE DATOS
//--------------------------------------------------------------------------
typedef Vector2<float>  Vect2f;
typedef Vector2<double> Vect2d;
typedef Vector2<int32>  Vect2i;
typedef Vector2<uint32> Vect2u;
typedef Vector2<uint16> Vect2w;

//--------------------------------------------------------------------------
//  CONSTANTES FLOAT
//--------------------------------------------------------------------------
extern const Vect2f  v2fX;
extern const Vect2f  v2fY;
extern const Vect2f  v2fONE;
extern const Vect2f  v2fNEGX;
extern const Vect2f  v2fNEGY;
extern const Vect2f  v2fNEGONE;
extern const Vect2f  v2fZERO;
extern const Vect2f  v2fMAX;
extern const Vect2f  v2fMIN;
extern const Vect2f  v2fFRONT;
extern const Vect2f  v2fRIGHT;
extern const Vect2f  v2fUNIT;

//--------------------------------------------------------------------------
//  CONSTANTES DOUBLE
//--------------------------------------------------------------------------
extern const Vect2d  v2dX;
extern const Vect2d  v2dY;
extern const Vect2d  v2dONE;
extern const Vect2d  v2dNEGX;
extern const Vect2d  v2dNEGY;
extern const Vect2d  v2dNEGONE;
extern const Vect2d  v2dZERO;
extern const Vect2d  v2dMAX;
extern const Vect2d  v2dMIN;
extern const Vect2d  v2dTOP;
extern const Vect2d  v2dFRONT;
extern const Vect2d  v2dRIGHT;
extern const Vect2d  v2dUNIT;

//--------------------------------------------------------------------------
// CONSTANTES INT
//--------------------------------------------------------------------------
extern const Vect2i  v2iX;
extern const Vect2i  v2iY;
extern const Vect2i  v2iONE;
extern const Vect2i  v2iNEGX;
extern const Vect2i  v2iNEGY;
extern const Vect2i  v2iNEGONE;
extern const Vect2i  v2iZERO;
extern const Vect2i  v2iTOP;
extern const Vect2i  v2iFRONT;
extern const Vect2i  v2iRIGHT;
extern const Vect2i  v2iUNIT;


#endif
