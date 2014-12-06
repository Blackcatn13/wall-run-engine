//-----------------------------------------------------------------------
// Matrix33 class
// Matrices de 3 filas x 3 columnas
//-----------------------------------------------------------------------
#ifndef __MATRIX_33_H__
#define __MATRIX_33_H__

#include "MathTypes.h"
#include "MathUtils.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Utils/Types.h"

#include <memory>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Matriz de 3 filas x 3 columnas
///
/// @Note1  \verbatim
///         Esta matriz puede describir una transformación de rotación. Una transformación entre
///         sistemas de referencia tan solo rotados entre si.
///
///         |              |        | r00 r01 r02 |        | m00 m01 m02 |
///         | Rotacion 3x3 |  --->  | r10 r11 r12 |  --->  | m10 m11 m12 |
///         |              |        | r20 r21 r22 |        | m20 m21 m22 |
///
///         | x1 x2 x3 |
///         | y1 y2 y3 |
///         | z1 z2 z3 |
///           |  |  |
///           |  |  |
///           |  |  `-----------> eje Z del sistema de referencia en coordenadas del padre
///           |  `--------------> eje Y del sistema de referencia en coordenadas del padre
///           `-----------------> eje x del sistema de referencia en coordenadas del padre
///
///         En memoria los datos están contiguos por columnas:
///                m00 m10 m20 / m01 m11 m21 / m02 m12 m22 / m03 m13 m23 
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Matrix33
{
public:
  //-----------------
  // DATOS PUBLICOS
  //-----------------

#ifdef ROW_MAJOR_MATRICES
	T m00, m01, m02;
	T m10, m11, m12;
	T m20, m21, m22;
#else
  T m00, m10, m20;
  T m01, m11, m21;
  T m02, m12, m22;
#endif

public:
  //-------------------
  // METODOS PUBLICOS
  //-------------------

  // Construcción
  inline Matrix33 ();
  inline Matrix33 (const Matrix33<T>& otra);
  inline Matrix33 (const T _m00, const T _m01, const T _m02,
                   const T _m10, const T _m11, const T _m12,
                   const T _m20, const T _m21, const T _m22);
  inline Matrix33 (const Vector3<T>& ejeX,
                   const Vector3<T>& ejeY,
                   const Vector3<T>& ejeZ);
  inline Matrix33 (const T angleY, const T angleX, const T angleZ);


  // Reseteos parciales (traslación/rotación/escalado) y totales (a identidad, a cero)
  inline Matrix33<T>& ResetRotation    ();
  inline Matrix33<T>& ResetScale       ();
  inline Matrix33<T>& SetIdentity      ();
  inline Matrix33<T>& SetZero          ();


  // Establecimiento de datos de la matriz con reseteo previo a identidad
  inline Matrix33<T>& SetFromBasis     (const Vector3<T>& ejeX,
                                        const Vector3<T>& ejeY,
                                        const Vector3<T>& ejeZ);

  inline Matrix33<T>& SetFromAngleX    (const T angleX);
  inline Matrix33<T>& SetFromAngleY    (const T angleY);
  inline Matrix33<T>& SetFromAngleZ    (const T angleZ);
  inline Matrix33<T>& SetFromAnglesXZ  (const T angleX, const T angleZ);
  inline Matrix33<T>& SetFromAnglesYXZ (const T angleY, const T angleX, const T angleZ);
  inline Matrix33<T>& SetFromScale     (const T escala_x, const T escala_y, const T escala_z);
  inline Matrix33<T>& SetFromLookAt    (const Vector3<T>& vPos, const Vector3<T>& vTarget);
  inline Matrix33<T>& SetFromLookAt    (const Vector3<T>& vPos, const Vector3<T>& vTarget, const Vector3<T>& vUp);
//  inline Matrix33<T>& SetFromQuat      (const Quatn<T>& quat);


  // Modificación de una de las partes de la matriz (rotación/escalado) dejando invariable las otras
  inline Matrix33<T>& SetRotByAngleX    (const T angleX);
  inline Matrix33<T>& SetRotByAngleY    (const T angleY);
  inline Matrix33<T>& SetRotByAngleZ    (const T angleZ);
  inline Matrix33<T>& SetRotByAnglesXZ  (const T angleX, const T angleZ);
  inline Matrix33<T>& SetRotByAnglesYXZ (const T angleY, const T angleX, const T angleZ);
//  inline Matrix34<T>& SetRotByQuat      (const Quatn<T>& quat);

  inline Matrix33<T>& SetScale          (const Matrix33<T>& mat_escala);
  inline Matrix33<T>& SetScale          (const Vector3<T>& vect_escala);
  inline Matrix33<T>& SetScale          (const T escala_x, const T escala_y, const T escala_z);
  

  // Acceso
  inline Vector3<T>        GetRow                  (int i) const;
  inline Vector3<T>				 GetColum                (int i) const;
  inline Vector3<T>				 GetVectorBasis          (int i) const;
	inline T								 GetVectorBasisLength    (int i) const;
  inline void              GetBasis                (Vector3<T>& vBasisX,
                                                    Vector3<T>& vBasisY,
                                                    Vector3<T>& vBasisZ) const;
  inline Matrix33<T>       Get33RotationNormalized () const;
  inline Matrix33<T>       Get33RotationScaled     () const;
  inline Matrix33<T>       Get33Scale              () const;

  inline Vector3<T>        GetScale                () const;
  inline T                 GetScaleX               () const;
  inline T                 GetScaleY               () const;
  inline T                 GetScaleZ               () const;

  // Angulos de Euler
  void GetAnglesYXZ (T& angleY, T& angleX, T& angleZ) const;
  T    GetAngleX    () const;
  T    GetAngleY    () const;
  T    GetAngleZ    () const;

  // Angulos de Euler con criterio de Pitch-Roll-Yaw
  // (Orden XZY, teniendo los ángulos Y,Z el sentido contrario de rotación)
  inline T     GetRoll         () const;
  inline T     GetPitch        () const;
  inline T     GetYaw          () const;
  Vector3<T>   GetPitchRollYaw () const;
  Matrix33<T>& SetPitchRollYaw (const Vector3<T>& v3PitchRollYaw);


  // Operadores de aritmética de matrices
  Matrix33<T>        operator + (const Matrix33<T>& otra) const;
  Matrix33<T>        operator - (const Matrix33<T>& otra) const;
  Matrix33<T>        operator * (const Matrix33<T>& otra) const;
  Matrix33<T>        operator * (const T escalar) const;
  friend Matrix33<T> operator * (const T escalar, const Matrix33<T>& mat);
  Matrix33<T>        operator / (const T escalar) const;
  Vector3<T>         operator * (const Vector3<T>& vector) const;
  Vector3<T>         operator ^ (const Vector3<T>& vector) const;


  // Operadores aritméticos de actualización
  Matrix33<T>& operator += (const Matrix33<T>& otra);
  Matrix33<T>& operator -= (const Matrix33<T>& otra);
  Matrix33<T>& operator *= (const Matrix33<T>& otra);
  Matrix33<T>& operator *= (const T escalar);
  Matrix33<T>& operator /= (const T escalar);


  // Operadores de comparación
  inline bool operator ==       (const Matrix33<T>& otra) const;
  inline bool operator !=       (const Matrix33<T>& otra) const;
  inline bool IsEqualEpsilon    (const Matrix33<T>& otra, const T Epsilo = Epsilon<T>()) const;//TODO CW quito la n para que no sea igual
  inline bool IsNotEqualEpsilon (const Matrix33<T>& otra, const T Epsilo = Epsilon<T>()) const;//TODO CW quito la n para que no sea igual

  // Funciones de transformación de vectores
  inline void       TransformVector       (Vector3<T>& vector) const;
  inline Vector3<T> GetTransformVector    (const Vector3<T>& vector) const;
  void              TransformArrayVectors (int iElements, Vector3<T>* pVecOUT, Vector3<T>* pVecIN) const;


  // Funciones de matriz: Inversión, transposición, escalado, rotación [const]
  Matrix33<T>  GetInverted         () const;
  Matrix33<T>  GetInvertedRS       () const;
  Matrix33<T>  GetTransposed       () const;
  Matrix33<T>  GetRotedByAngleX    (const T angleX) const;
  Matrix33<T>  GetRotedByAngleY    (const T angleY) const;
  Matrix33<T>  GetRotedByAngleZ    (const T angleZ) const;
  Matrix33<T>  GetRotedByAnglesXZ  (const T angleX, const T angleZ) const;
  Matrix33<T>  GetRotedByAnglesYXZ (const T angleY, const T angleX, const T angleZ) const;
  Matrix33<T>  GetScaled           (const T escala_x, const T escala_y, const T escala_z) const;
  

  // Funciones de matriz: Inversión, transposición, escalado, rotación [no const]
  Matrix33<T>& Invert         ();
  Matrix33<T>& InvertRS       ();
  Matrix33<T>& Transpose      ();
  Matrix33<T>& RotByAngleX    (const T angleX);
  Matrix33<T>& RotByAngleY    (const T angleY);
  Matrix33<T>& RotByAngleZ    (const T angleZ);
  Matrix33<T>& RotByAnglesXZ  (const T angleX, const T angleZ);
  Matrix33<T>& RotByAnglesYXZ (const T angleY, const T angleX, const T angleZ);
  Matrix33<T>& Scale          (const T escala_x, const T escala_y, const T escala_z);

  // (*)
  Matrix33<T>& AlignYX        (const Vector3<T>& vY, const Vector3<T>& vX); 
  Matrix33<T>& AlignYZ        (const Vector3<T>& vY, const Vector3<T>& vZ); 
  Matrix33<T>& AlignXZ        (const Vector3<T>& vX, const Vector3<T>& vZ);


  // Acceso/determinación a/de propiedades matriciales
  inline T     Determinant          () const;
  bool         IsOrthogonalEpsilon  () const;
  bool         IsOrthonormalEpsilon () const;
};

// Fichero con las definiciones de las funciones inline
#include "Matrix33.inl"


////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE MATRICES [3x3] CON TIPOS CONCRETOS DE DATOS
////////////////////////////////////////////////////////////////////////////////////////
typedef Matrix33<float>   Mat33f;
typedef Matrix33<double>  Mat33d;

////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES FLOAT
////////////////////////////////////////////////////////////////////////////////////////
extern const Mat33f  m33fIDENTITY;
extern const Mat33f  m33fZERO;
extern const Mat33f  m33fONES; 
                              
////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES DOUBLE
////////////////////////////////////////////////////////////////////////////////////////
extern const Mat33d  m33dIDENTITY;
extern const Mat33d  m33dZERO;
extern const Mat33d  m33dONES;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //_NGT_MATRIX_33_H_
