//-----------------------------------------------------------------------
// Matrix44 class
// This class defines a matrix 4x4
//-----------------------------------------------------------------------
#ifndef __MATRIX_44_H__
#define __MATRIX_44_H__

#include "MathTypes.h"
#include "MathUtils.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix33.h"
#include "Utils/Types.h"
#include <d3dx9.h>

#include <memory>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Matriz de 4 filas x 4 columnas
/// @Note1
/// \verbatim
///         Cuando la matriz describe una transformación típica, compuesta de
///         rotación + traslación, se almacena con la estructura:
/// \endverbatim
/// \code
///        |              | P |         | r00 r01 r02 | P_x |        | m00 m01 m02 | m03 |
///        | Rotacion 3x3 | o |         | r10 r11 r12 | P_y |        | m10 m11 m12 | m13 |
///        |              | s |  ---->  | r20 r21 r22 | p_z |  --->  | m20 m21 m22 | m23 |
///        |--------------|---|         |-------------|-----|        |-------------|-----|
///        | 0    0    0  | 1 |         | 0   0   0   |  1  |        | m30 m31 m32 | m33 |
///
///        | x1 x2 x3 | Px |
///        | y1 y2 y3 | Py |
///        | z1 z2 z3 | Pz |
///        |----------|----|
///        | 0  0  0  | 1  |
///          |  |  |    |
///          |  |  |    `------> Posición del origen del sistema de referencia en coordenadas del padre
///          |  |  `-----------> eje Z del sistema de referencia en coordenadas del padre
///          |  `--------------> eje Y del sistema de referencia en coordenadas del padre
///          `-----------------> eje x del sistema de referencia en coordenadas del padre
/// \endcode
/// \verbatim
///         En memoria los datos están contiguos por columnas:
///               / m00 m10 m20 m30 / m01 m11 m21 m31 / m02 m12 m22 m32 / m03 m13 m23 m33 /
///         
///         Esto facilita el acceso a los datos de los ejes y a la posición del sistema de referencia
///         en coordenadas del padre (que suelen ser importantes, ej. vectores de cámara)
///
///
///         Los vectores aislados se consideran 'VECTORES-COLUMNA' y el objeto 'Vector' incluye
///         la funcionalidad de un punto.
///
///
///            | x |                           | x |
///            | y |  como vector              | y |  como punto
///            | z |                           | z |
///                                            | 1 |
///        
///         Esto implica que las multiplicaciones matriz-vector se efectúan siempre en el orden
///         MATRIZ x VECTOR (dada una matriz 4x4 y un vector 4x1 --> obtenemos un vector 4x1, el caso
///         contrario, vector 4x1 por matriz 4x4, produce una matriz 4x4)
/// \endverbatim
/// @Note2 Producto Matriz x vector: 
/// \verbatim   
///         Hay dos tipos de producto de matriz x vector (formalmente productos de matrices), identificados
///         por el operador '*' y el operador '^'
///
///         ( * ) --> Producto Matriz x punto (transformación de ROTACION + TRASLACION)
///                   Los vectores incluyen la funcionalidad del punto, por lo tanto, 
///                   se pueden multiplicar por una matriz de 4x4, considerándolos matrices 
///                   de 4x1
///
///         | m00 m01 m02 m03 |     | x |     | x*m00 + y*m01 + z*m02 + m03 |
///         | m10 m11 m12 m13 |  *  | y |  =  | x*m10 + y*m11 + z*m12 + m13 |
///         | m20 m21 m22 m23 |     | z |     | x*m20 + y*m21 + z*m22 + m23 |
///         | m30 m31 m32 m33 |     | 1 |     | x*m30 + y*m31 + z*m32 + m33 |
///
///         ( ^ ) --> Producto Matriz x vector (transformación de ROTACION)
///                   En este caso solo se multiplica el menor 3x3, que incluye la rotación,
///                   por el vector-columna 3x1, sin añadir el termino 'w = 1'
///
///         | m00 m01 m02 |     | x |     | x*m00 + y*m01 + z*m02 |
///         | m10 m11 m12 |  ^  | y |  =  | x*m10 + y*m11 + z*m12 |
///         | m20 m21 m22 |     | z |     | x*m20 + y*m21 + z*m22 |
/// \endverbatim
/// @Note3  Producto de matrices
/// \verbatim
///         Dos matrices M1 y M2 se multiplican de tal forma que el resultado es una matriz con las
///         filas de la primera y las columnas de la segunda. 
///
///         | m00 m01 m02 m03 |     | n00 n01 n02 n03 |   | m00*n00 + m01*n10 + m02*n20 + m03*n30   ... |
///         | m10 m11 m12 m13 |  *  | n10 n11 n12 n13 | = |  ...                                        |
///         | m20 m21 m22 m23 |     | n20 n21 n22 n23 |   |  ...                                        |
///         | m30 m31 m32 m33 |     | n30 n31 n32 n33 |   |  ...                                        |
///
///         Si consideramos las matrices como transformaciones entre espacios, los productos se
///         concatenan partiendo de la matriz construida a partir del sistema de referencia del
///         mundo hasta llegar a la matriz local de un objeto y a los puntos de los que se compone.
///
///         Si tenemos los sistemas de referencia local -> hijo -> padre -> mundo, para obtener un
///         punto en coordenadas de mundo, a partir de un punto en coordenadas locales, haremos:
///
///           P(en mundo) = M(padre->mundo) * M(hijo->padre) * M(local->hijo) * P(en locales)
/// 
///         Si concatenamos una rotación y una traslación:
///           M(rotacion) * M(traslación) * P  --> el punto se traslada en locales a la posición de traslación y
///                                                luego rota respecto al origen del sistema local (o sea, pasa
///                                                a coordenadas de padre habiendo modificado su posición en locales)
///                                                NOTA: típico efecto de rotar alrededor del mundo, cuando se quiere
///                                                rotar en local y equivocamos el orden de multiplicación de matrices)
///
///           M(traslación) * M(rotación) * P  --> El punto rota en su sistema de referencia y luego es trasladado
///                                                (en el sistema de referencia del padre). Esta concatenación es la que
///                                                sirve para construir una matriz de sistema de referencia:
///
///                          | ux1 ux2 ux3 Px |
///                          | uy1 uy2 uy3 Py |
///                          | uz1 uz2 uz3 Pz |
///                          | 0   0   0   1  |
///
///
///         Si concatenamos una TRASLACION + ROTACION + ESCALADO:
///           M(traslación) * M(rotación) * M(escalado) P  --> El punto (u objeto compuesto de puntos) es escalado, rota en locales,
///                                                            y luego es trasladado en el sistema del padre.
///                                                            Esta concatenación es la que sirve para construir una matriz de
///                                                            sistema de referencia con escalado:
///
///             | ux1*ex x2*ey x3*ez Px |
///             | uy1*ex y2*ey y3*ez Py |
///             | uz1*ex z2*ey z3*ez Pz |
///             |    0     0     0   1  |
///                  |     |     |   |
///                  |     |     |   `-------> Posición del origen del sistema de referencia en coordenadas del padre
///                  |     |     `-----------> eje Z, ESCALADO, del sistema de referencia en coordenadas del padre
///                  |     `-----------------> eje Y, ESCALADO, del sistema de referencia en coordenadas del padre
///                  `-----------------------> eje X, ESCALADO, del sistema de referencia en coordenadas del padre
///
///
///         Si se multiplica el escalado por la izquierda:
///             M(escalado) * M(traslación) * M(rotación)  --> La posición del sistema de referencia se verá afectada, esto es,
///                                                            escalada (en coordenadas del padre) igual que las posiciones de
///                                                            los puntos en local.
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Matrix44
{
public:
  //-----------------
  // DATOS PUBLICOS
  //-----------------

#ifdef ROW_MAJOR_MATRICES
	T m00, m01, m02, m03;       // Primera 'fila'
	T m10, m11, m12, m13;       // Segunda 'fila'
	T m20, m21, m22, m23;       // Tercera 'fila'
	T m30, m31, m32, m33;       // Cuarta  'fila' 
#else
  T m00, m10, m20, m30;       // Primera 'columna'
  T m01, m11, m21, m31;       // Segunda 'columna'
  T m02, m12, m22, m32;       // Tercera 'columna'
  T m03, m13, m23, m33;       // Cuarta  'columna' 
#endif

public:
  //-------------------
  // METODOS PUBLICOS
  //-------------------

  // Construcción
  inline Matrix44 ();
	inline Matrix44 (const D3DXMATRIX &otra);
  inline Matrix44 (const Matrix44<T>& otra);
  inline Matrix44 (const Matrix33<T>& otra);
  inline Matrix44 (const Matrix34<T>& otra);
  inline Matrix44 (const T _m00, const T _m01, const T _m02, const T _m03,
                   const T _m10, const T _m11, const T _m12, const T _m13,
                   const T _m20, const T _m21, const T _m22, const T _m23,
                   const T _m30, const T _m31, const T _m32, const T _m33);
  inline Matrix44 (const Vector3<T>& ejeX,
                   const Vector3<T>& ejeY,
                   const Vector3<T>& ejeZ,
                   const Vector3<T>& pos);
  inline Matrix44 (const T angleY, const T angleX, const T angleZ);


  // Reseteos parciales (traslación/rotación/escalado) y totales (a identidad, a cero)
  inline Matrix44<T>& ResetTranslation   ();
  inline Matrix44<T>& ResetRotation      ();
  inline Matrix44<T>& ResetScale         ();
  inline Matrix44<T>& ResetRotationScale ();
  inline Matrix44<T>& SetIdentity        ();
  inline Matrix44<T>& SetZero            ();


  // Establecimiento de datos de la matriz con reseteo previo a identidad
  inline Matrix44<T>& SetFromBasis     (const Vector3<T>& vBasisX,
                                        const Vector3<T>& vBasisY,
                                        const Vector3<T>& vBasisZ,
                                        const Vector3<T>& Pos);
	inline Matrix44<T>& SetFromBasis     (const Vector3<T>& vBasisX,
																				const Vector3<T>& vBasisY,
																				const Vector3<T>& vBasisZ);
  inline Matrix44<T>& SetFromAngleX    (const T angleX);
  inline Matrix44<T>& SetFromAngleY    (const T angleY);
  inline Matrix44<T>& SetFromAngleZ    (const T angleZ);
  inline Matrix44<T>& SetFromAnglesXZ  (const T angleX, const T angleZ);
  inline Matrix44<T>& SetFromAnglesYXZ (const T angleY, const T angleX, const T angleZ);
  inline Matrix44<T>& SetFromPos       (const T posX, const T posY, const T posZ);
  inline Matrix44<T>& SetFromPos       (const Vector3<T>& pos);
  inline Matrix44<T>& SetFromScale     (const T escala_x, const T escala_y, const T escala_z);
  inline Matrix44<T>& SetFromLookAt    (const Vector3<T>& vPos, const Vector3<T>& vTarget, const Vector3<T>& vUp);
  inline Matrix44<T>& SetFromLookAt    (const Vector3<T>& vPos, const Vector3<T>& vTarget);
  //inline Matrix44<T>& SetFromQuatPos   (const Quatn<T>& quat, const Vector3<T>& vPos);


  // Modificación de una de las partes de la matriz dejando invariable las otras
  inline Matrix44<T>& SetRotByAngleX    (const T angleX);
  inline Matrix44<T>& SetRotByAngleY    (const T angleY);
  inline Matrix44<T>& SetRotByAngleZ    (const T angleZ);
  inline Matrix44<T>& SetRotByAnglesXZ  (const T angleX, const T angleZ);
  inline Matrix44<T>& SetRotByAnglesYXZ (const T angleY, const T angleX, const T angleZ);
  //inline Matrix44<T>& SetRotByQuat      (const Quatn<T>& quat);

  inline Matrix44<T>& SetPos          (const Vector3<T>& pos);
  inline Matrix44<T>& SetPos          (const T posX, const T posY, const T posZ);

  inline Matrix44<T>& SetScale        (const Matrix33<T>& mat_escala);
  inline Matrix44<T>& SetScale        (const Matrix44<T>& mat_escala);
  inline Matrix44<T>& SetScale        (const Vector3<T>& vect_escala);
  inline Matrix44<T>& SetScale        (const T escala_x, const T escala_y, const T escala_z);
  inline Matrix44<T>& SetSubMatrix33  (const Matrix33<T>& mat_sub);
  inline Matrix44<T>& SetSubMatrix34  (const Matrix34<T>& mat_sub);


  // Acceso
  inline Vector4<T>        GetRow                  (int i) const;
  inline Vector4<T>				 GetColum                (int i) const;
  inline Vector3<T>				 GetVectorBasis          (int i) const;
	inline T								 GetVectorBasisLength    (int i) const;
  inline Vector3<T>				 GetPosBasis             () const;
  inline void              GetBasis                (Vector3<T>& vBasisX,
                                                    Vector3<T>& vBasisY,
                                                    Vector3<T>& vBasisZ,
                                                    Vector3<T>& Pos) const;
  inline Matrix33<T>       GetSubMatrix33          () const;
  inline void              GetSubMatrix33          (Matrix33<T>& subMat) const;
  inline Matrix34<T>       GetSubMatrix34          () const;
  inline void              GetSubMatrix34          (Matrix34<T>& subMat) const;

  inline Matrix33<T>       Get33RotationNormalized () const;
  inline Matrix33<T>       Get33RotationScaled     () const;
  inline Matrix33<T>       Get33Scale              () const;
  inline Vector3<T>        GetTranslationVector    () const;

  inline Matrix44<T>       Get44RotationNormalized () const;
  inline Matrix44<T>       Get44RotationScaled     () const;
  inline Matrix44<T>       Get44Scale              () const;
  inline Matrix44<T>       Get44Translation        () const;

  inline Vector3<T>        GetPos                  () const;

  inline Vector3<T>        GetScale                () const;
  inline T                 GetScaleX               () const;
  inline T                 GetScaleY               () const;
  inline T                 GetScaleZ               () const;

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

         Matrix44<T>& SetFromPitchRollYaw (const Vector3<T>& v3PitchRollYaw);
  inline Matrix44<T>& SetPitchRollYaw     (const Vector3<T>& v3PitchRollYaw);

  // Operadores de aritmética de matrices
  Matrix44<T>        operator + (const Matrix44<T>& otra) const;
  Matrix44<T>        operator - (const Matrix44<T>& otra) const; 
  Matrix44<T>        operator * (const Matrix44<T>& otra) const;
  Matrix44<T>        operator * (const T escalar) const;
  friend Matrix44<T> operator * (const T escalar, const Matrix44<T>& mat);
  Matrix44<T>        operator / (const T escalar) const;
  Vector3<T>         operator * (const Vector3<T>& vector) const;
  Vector3<T>         operator ^ (const Vector3<T>& vector) const;
  Vector4<T>         operator * (const Vector4<T>& vector) const;
  
  
  // Operadores aritméticos de actualización
  Matrix44<T>& operator += (const Matrix44<T>& otra);
  Matrix44<T>& operator -= (const Matrix44<T>& otra);
  Matrix44<T>& operator *= (const Matrix44<T>& otra);
  Matrix44<T>& operator *= (const T escalar);
  Matrix44<T>& operator /= (const T escalar);


  // Operadores de comparación
  inline bool operator ==       (const Matrix44<T>& otra) const;
  inline bool operator !=       (const Matrix44<T>& otra) const;
  inline bool IsEqualEpsilon    (const Matrix44<T>& otra, const T Epsilo = Epsilon<T>()) const;//TODO CW quito la n para que no sea igual
  inline bool IsNotEqualEpsilon (const Matrix44<T>& otra, const T Epsilo = Epsilon<T>()) const;//TODO CW quito la n para que no sea igual


  // Funciones de transformación de vectores / puntos
  inline void       TransformVector       (Vector3<T>& vector) const;
  inline void       TransformPoint        (Vector3<T>& vector) const;
  inline Vector3<T> GetTransformVector    (const Vector3<T>& vector) const;
  inline Vector3<T> GetTransformPoint     (const Vector3<T>& vector) const;
  void              TransformArrayVectors (int iElements, Vector3<T>* pVecOUT, Vector3<T>* pVecIN) const;
  void              TransformArrayPoints  (int iElements, Vector3<T>* pVecOUT, Vector3<T>* pVecIN) const;
	D3DXMATRIX        GetD3DXMatrix         ( ) const;


  // Operaciones especiales con matrices
  // [const]
  Matrix44<T>  GetInverted         () const;
  Matrix44<T>  GetInvertedTRS      () const;
  Matrix44<T>  GetInvertedTR       () const;
  Matrix44<T>  GetTransposed       () const; 
  Matrix44<T>  GetRotedByAngleX    (const T angleX) const;
  Matrix44<T>  GetRotedByAngleY    (const T angleY) const;
  Matrix44<T>  GetRotedByAngleZ    (const T angleZ) const;
  Matrix44<T>  GetRotedByAnglesXZ  (const T angleX, const T angleZ) const;
  Matrix44<T>  GetRotedByAnglesYXZ (const T angleY, const T angleX, const T angleZ) const;
  Matrix44<T>  GetTranslated       (const Vector3<T>& pos) const;
  Matrix44<T>  GetScaled           (const T escala_x, const T escala_y, const T escala_z) const;


  // [no const]
  Matrix44<T>& Invert         ();
  Matrix44<T>& InvertTRS      ();
  Matrix44<T>& InvertTR       ();
  Matrix44<T>& Transpose      ();
  Matrix44<T>& RotByAngleX    (const T angleX);
  Matrix44<T>& RotByAngleY    (const T angleY);
  Matrix44<T>& RotByAngleZ    (const T angleZ);
  Matrix44<T>& RotByAnglesXZ  (const T angleX, const T angleZ);
  Matrix44<T>& RotByAnglesYXZ (const T angleY, const T angleX, const T angleZ);
  Matrix44<T>& Translate      (const Vector3<T>& pos);
  Matrix44<T>& Scale          (const T escala_x, const T escala_y, const T escala_z);
  // (*)
  Matrix44<T>& AlignYX        (const Vector3<T>& vY, const Vector3<T>& vX);
  Matrix44<T>& AlignYZ        (const Vector3<T>& vY, const Vector3<T>& vZ);
  Matrix44<T>& AlignXZ        (const Vector3<T>& vX, const Vector3<T>& vZ);


  // Acceso/determinación a/de propiedades matriciales
  inline T Determinant          () const;
  bool     IsOrthogonalEpsilon  () const;
  bool     IsOrthonormalEpsilon () const;


private:
  //inline Vector3<T>& VectorBasis (int i);
  //inline Vector3<T>& PosBasis    ();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fichero con las definiciones de las funciones inline
#include "Matrix44.inl"

////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE MATRICES [4x4] CON TIPOS CONCRETOS DE DATOS
////////////////////////////////////////////////////////////////////////////////////////
typedef Matrix44<float>   Mat44f;
typedef Matrix44<double>  Mat44d;

////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES FLOAT
////////////////////////////////////////////////////////////////////////////////////////
extern const Mat44f  m44fIDENTITY;
extern const Mat44f  m44fZERO;
extern const Mat44f  m44fONES;

////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES DOUBLE
////////////////////////////////////////////////////////////////////////////////////////
extern const Mat44d  m44dIDENTITY;
extern const Mat44d  m44dZERO;
extern const Mat44d  m44dONES;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
