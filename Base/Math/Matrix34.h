//-----------------------------------------------------------------------
// Matrix34 class
// This class defines a matrix 3x4 (3 rowns and 4 lines)
//-----------------------------------------------------------------------
#ifndef __MATRIX_34_H__
#define __MATRIX_34_H__

#include "MathTypes.h"
#include "MathUtils.h"
#include "Vector3.h"
#include "Matrix33.h"
#include "Matrix44.h"
#include "Utils/Types.h"

#include <memory>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Matriz de 3 filas x 4 columnas
///
/// @Note1  \verbatim
///         Cuando la matriz describe una transformación típica, compuesta de
///         rotación + traslación, se almacena con la estructura:
///
///         |              | P |        | r00 r01 r02 | P_x |        | m00 m01 m02 | m03 |
///         | Rotacion 3x3 | o |  --->  | r10 r11 r12 | P_y |  --->  | m10 m11 m12 | m13 |
///         |              | s |        | r20 r21 r22 | p_z |        | m20 m21 m22 | m23 |
///
///         | x1 x2 x3 | Px |
///         | y1 y2 y3 | Py |
///         | z1 z2 z3 | Pz |
///           |  |  |    |
///           |  |  |    `------> Posición del origen del sistema de referencia en coordenadas del padre
///           |  |  `-----------> eje Z del sistema de referencia en coordenadas del padre
///           |  `--------------> eje Y del sistema de referencia en coordenadas del padre
///           `-----------------> eje x del sistema de referencia en coordenadas del padre
///
///         En memoria los datos están contiguos por columnas:
///                m00 m10 m20 / m01 m11 m21 / m02 m12 m22 / m03 m13 m23 
/// \endverbatim
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Matrix34
{
public:
  //------------------------------------------
  // DATOS PUBLICOS (contiguos por columnas)
  //------------------------------------------

#ifdef ROW_MAJOR_MATRICES
	T m00, m01, m02, m03;
	T m10, m11, m12, m13;
	T m20, m21, m22, m23;
#else
  T m00, m10, m20;
  T m01, m11, m21;
  T m02, m12, m22;
  T m03, m13, m23;
#endif

public:
  //-------------------
  // METODOS PUBLICOS
  //-------------------

  // Construcción
  inline Matrix34 ();
  inline Matrix34 (const Matrix34<T>& otra);
  inline Matrix34 (const Matrix33<T>& sub);
  inline Matrix34 (const Matrix33<T>& sub, const Vector3<T>& pos);
  inline Matrix34 (const T _m00, const T _m01, const T _m02, const T _m03,
                   const T _m10, const T _m11, const T _m12, const T _m13,
                   const T _m20, const T _m21, const T _m22, const T _m23);
  inline Matrix34 (const Vector3<T>& ejeX,
                   const Vector3<T>& ejeY,
                   const Vector3<T>& ejeZ,
                   const Vector3<T>& pos);
  inline Matrix34 (const T angleY, const T angleX, const T angleZ);

 

  // Reseteos parciales (traslación/rotación/escalado) y totales (a identidad, a cero)
  inline Matrix34<T>& ResetTranslation   ();
  inline Matrix34<T>& ResetRotation      ();
  inline Matrix34<T>& ResetScale         ();
  inline Matrix34<T>& ResetRotationScale ();
  inline Matrix34<T>& SetIdentity        ();
  inline Matrix34<T>& SetZero            ();


  // Establecimiento de datos de la matriz con reseteo previo a identidad
  inline Matrix34<T>& SetFromBasis       (const Vector3<T>& ejeX,
                                          const Vector3<T>& ejeY,
                                          const Vector3<T>& ejeZ,
                                          const Vector3<T>& pos);

	inline Matrix34<T>& SetFromBasis       (const Vector3<T>& ejeX,
																					const Vector3<T>& ejeY,
																					const Vector3<T>& ejeZ);

  inline Matrix34<T>& SetFromAngleX    (const T angleX);
  inline Matrix34<T>& SetFromAngleY    (const T angleY);
  inline Matrix34<T>& SetFromAngleZ    (const T angleZ);
  inline Matrix34<T>& SetFromAnglesXZ  (const T angleX, const T angleZ);
  inline Matrix34<T>& SetFromAnglesYXZ (const T angleY, const T angleX, const T angleZ);
  inline Matrix34<T>& SetFromPos       (const T posX, const T posY, const T posZ);
  inline Matrix34<T>& SetFromPos       (const Vector3<T>& pos);
  inline Matrix34<T>& SetFromScale     (const T escala_x, const T escala_y, const T escala_z);
  inline Matrix34<T>& SetFromLookAt    (const Vector3<T>& vPos, const Vector3<T>& vTarget, const Vector3<T>& vUp);
  inline Matrix34<T>& SetFromLookAt    (const Vector3<T>& vPos, const Vector3<T>& vTarget);
//  inline Matrix34<T>& SetFromQuatPos   (const Quatn<T>& quat, const Vector3<T>& vPos);


  // Modificación de una de las partes de la matriz (rotación/traslación/escalado) dejando invariable las otras
  inline Matrix34<T>& SetRotByAngleX    (const T angleX);
  inline Matrix34<T>& SetRotByAngleY    (const T angleY);
  inline Matrix34<T>& SetRotByAngleZ    (const T angleZ);
  inline Matrix34<T>& SetRotByAnglesXZ  (const T angleX, const T angleZ);
  inline Matrix34<T>& SetRotByAnglesYXZ (const T angleY, const T angleX, const T angleZ);
  //inline Matrix34<T>& SetRotByQuat      (const Quatn<T>& quat);

  inline Matrix34<T>& SetPos (const T posX, const T posY, const T posZ);
  inline Matrix34<T>& SetPos (const Vector3<T>& pos);

  inline Matrix34<T>& SetScale (const Matrix33<T>& mat_escala);
  inline Matrix34<T>& SetScale (const Matrix44<T>& mat_escala);
  inline Matrix34<T>& SetScale (const Vector3<T>& vect_escala);
  inline Matrix34<T>& SetScale (const T escala_x, const T escala_y, const T escala_z);

  inline Matrix34<T>& SetSubMatrix33 (const Matrix33<T>& mat_sub);


  // Acceso  
  inline Vector3<T>				 GetVectorBasis           (int i) const;
	inline T								 GetVectorBasisLength    (int i) const;
  inline Vector3<T>				 GetPosBasis              () const;
  inline void              GetBasis                 (Vector3<T>& vBasisX,
                                                     Vector3<T>& vBasisY,
                                                     Vector3<T>& vBasisZ,
                                                     Vector3<T>& Pos) const;
  inline const Matrix33<T>  GetSubMatrix33          () const;
  inline void               GetSubMatrix33          (Matrix33<T>& subMat) const;
  inline const Matrix44<T>  GetMatrix44             () const;
  inline void               GetMatrix44             (Matrix44<T>& subMat) const;

  inline Matrix33<T>        Get33RotationNormalized () const;
  inline Matrix33<T>        Get33RotationScaled     () const;
  inline Matrix33<T>        Get33Scale              () const;
  inline Vector3<T>         GetTranslationVector    () const;

  inline Matrix44<T>        Get44RotationNormalized () const;
  inline Matrix44<T>        Get44RotationScaled     () const;
  inline Matrix44<T>        Get44Scale              () const;
  inline Matrix44<T>        Get44Translation        () const;

  inline Vector3<T>         GetScale                () const;
  inline T                  GetScaleX               () const;
  inline T                  GetScaleY               () const;
  inline T                  GetScaleZ               () const;

  // Angulos de Euler
  inline void GetAnglesYXZ (T& angleY, T& angleX, T& angleZ) const;
  inline T    GetAngleX    () const;
  inline T    GetAngleY    () const;
  inline T    GetAngleZ    () const;

  // Angulos de Euler con criterio de Pitch-Roll-Yaw
  // (Orden XZY, teniendo los ángulos Y,Z el sentido contrario de rotación)
  inline T          GetRoll         () const;
  inline T          GetPitch        () const;
  inline T          GetYaw          () const;
  inline Vector3<T> GetPitchRollYaw () const;

         Matrix34<T>& SetFromPitchRollYaw (const Vector3<T>& v3PitchRollYaw);
  inline Matrix34<T>& SetPitchRollYaw     (const Vector3<T>& v3PitchRollYaw);


  // Operadores de aritmética de matrices
  Matrix34<T>        operator + (const Matrix34<T>& otra) const;
  Matrix34<T>        operator - (const Matrix34<T>& otra) const; 
  Matrix34<T>        operator * (const Matrix34<T>& otra) const;
  Matrix34<T>        operator * (const T escalar) const;
  friend Matrix34<T> operator * (const T escalar, const Matrix34<T>& mat);
  Matrix34<T>        operator / (const T escalar) const;
  Vector3<T>         operator * (const Vector3<T>& vector) const;
  Vector3<T>         operator ^ (const Vector3<T>& vector) const;
  
  
  // Operadores aritméticos de actualización
  Matrix34<T>& operator += (const Matrix34<T>& otra);
  Matrix34<T>& operator -= (const Matrix34<T>& otra);
  Matrix34<T>& operator *= (const Matrix34<T>& otra);
  Matrix34<T>& operator *= (const T escalar);
  Matrix34<T>& operator /= (const T escalar);


  // Operadores de comparación
  inline bool operator ==       (const Matrix34<T>& otra) const;
  inline bool operator !=       (const Matrix34<T>& otra) const;
  inline bool IsEqualEpsilon    (const Matrix34<T>& otra, const T Epsilo = Epsilon<T>()) const; //TODO CW quito la n para que no sea igual
  inline bool IsNotEqualEpsilon (const Matrix34<T>& otra, const T Epsilo = Epsilon<T>()) const; //TODO CW quito la n para que no sea igual


  // Funciones de transformación de vectores / puntos
  inline void       TransformVector       (Vector3<T>& vector) const;
  inline void       TransformPoint        (Vector3<T>& vector) const;
  inline Vector3<T> GetTransformVector    (const Vector3<T>& vector) const;
  inline Vector3<T> GetTransformPoint     (const Vector3<T>& vector) const;
  void              TransformArrayVectors (int iElements, Vector3<T>* pVecOUT, Vector3<T>* pVecIN) const;
  void              TransformArrayPoints  (int iElements, Vector3<T>* pVecOUT, Vector3<T>* pVecIN) const;


  // Operaciones especiales con matrices
  // [const]
  Matrix34<T>  GetInverted       () const;
  Matrix34<T>  GetInvertedTRS    () const;
  Matrix34<T>  GetInvertedTR     () const;
  Matrix34<T>  GetRotedByAngleX    (const T angleX) const;
  Matrix34<T>  GetRotedByAngleY    (const T angleY) const;
  Matrix34<T>  GetRotedByAngleZ    (const T angleZ) const;
  Matrix34<T>  GetRotedByAnglesXZ  (const T angleX, const T angleZ) const;
  Matrix34<T>  GetRotedByAnglesYXZ (const T angleY, const T angleX, const T angleZ) const;
  Matrix34<T>  GetTranslated     (const Vector3<T>& pos) const;
  Matrix34<T>  GetScaled         (const T escala_x, const T escala_y, const T escala_z) const;

  // [no const]
  Matrix34<T>& Invert         ();
  Matrix34<T>& InvertTRS      ();
  Matrix34<T>& InvertTR       ();
  Matrix34<T>& RotByAngleX      (const T angleX);
  Matrix34<T>& RotByAngleY      (const T angleY);
  Matrix34<T>& RotByAngleZ      (const T angleZ);
  Matrix34<T>& RotByAnglesXZ    (const T angleX, const T angleZ);
  Matrix34<T>& RotByAnglesYXZ   (const T angleY, const T angleX, const T angleZ);
  Matrix34<T>& Translate      (const Vector3<T>& pos);
  Matrix34<T>& Scale          (const T escala_x, const T escala_y, const T escala_z);
  // (*)
  Matrix34<T>& AlignYX        (const Vector3<T>& vY, const Vector3<T>& vX);
  Matrix34<T>& AlignYZ        (const Vector3<T>& vY, const Vector3<T>& vZ);
  Matrix34<T>& AlignXZ        (const Vector3<T>& vX, const Vector3<T>& vZ);

  // [const modificando el parámetro]
  void GetInvertedTR (Matrix34<T>& retMat) const;
  

  // Acceso/determinación a/de propiedades matriciales
  inline T Determinant          () const;
  bool     IsOrthogonalEpsilon  () const;
  bool     IsOrthonormalEpsilon () const;

private:
  
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fichero con las definiciones de las funciones inline
#include "Matrix34.inl"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE MATRICES [3x4] CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Matrix34<float>   Mat34f;
typedef Matrix34<double>  Mat34d;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES FLOAT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const Mat34f  m34fIDENTITY;
extern const Mat34f  m34fZERO;
extern const Mat34f  m34fONES;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES DOUBLE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const Mat34d  m34dIDENTITY;
extern const Mat34d  m34dZERO;
extern const Mat34d  m34dONES;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
