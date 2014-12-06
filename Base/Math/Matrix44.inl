//-----------------------------------------------------------------------
// Matrix44 inline
/// Definiciones de funciones inline de la clase 'Matrix44'
/// Este fichero es realmente parte de la cabecera 'Matrix44.h'
//-----------------------------------------------------------------------

// NO PONER INCLUDES

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 1
/// Construcción sin inicialización de parámetros  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>::Matrix44 ()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 2
/// Constructor de copia
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>::Matrix44 (const Matrix44<T>& otra) :
    m00(otra.m00), m10(otra.m10), m20(otra.m20), m30(otra.m30),
    m01(otra.m01), m11(otra.m11), m21(otra.m21), m31(otra.m31),
    m02(otra.m02), m12(otra.m12), m22(otra.m22), m32(otra.m32),
    m03(otra.m03), m13(otra.m13), m23(otra.m23), m33(otra.m33)
{ 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 3
/// Inserción de todos los datos de la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>::Matrix44 (const T _m00, const T _m01, const T _m02, const T _m03,
                              const T _m10, const T _m11, const T _m12, const T _m13,
                              const T _m20, const T _m21, const T _m22, const T _m23,
                              const T _m30, const T _m31, const T _m32, const T _m33)  :
  m00(_m00), m10(_m10), m20(_m20), m30(_m30),
  m01(_m01), m11(_m11), m21(_m21), m31(_m31),
  m02(_m02), m12(_m12), m22(_m22), m32(_m32),
  m03(_m03), m13(_m13), m23(_m23), m33(_m33)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 4
/// A partir de una matriz 3x3. La columna correspondiénte
/// a la posición de la base será [0 0 0 (1)]
/// La última fila será [0 0 0 1]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>::Matrix44 (const Matrix33<T>& otra) :
  m00(otra.m00),  m10(otra.m10),  m20(otra.m20),  m30(Zero<T>()),
  m01(otra.m01),  m11(otra.m11),  m21(otra.m21),  m31(Zero<T>()),
  m02(otra.m02),  m12(otra.m12),  m22(otra.m22),  m32(Zero<T>()),
  m03(Zero<T>()), m13(Zero<T>()), m23(Zero<T>()), m33(One<T>())
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 5
/// A partir de una matriz 3x3
/// La última fila será [0 0 0 1]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>::Matrix44 (const Matrix34<T>& otra) :
  m00(otra.m00), m10(otra.m10), m20(otra.m20), m30(Zero<T>()),
  m01(otra.m01), m11(otra.m11), m21(otra.m21), m31(Zero<T>()),
  m02(otra.m02), m12(otra.m12), m22(otra.m22), m32(Zero<T>()),
  m03(otra.m03), m13(otra.m13), m23(otra.m23), m33(One<T>())
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 6
/// (Ver comentario en SetFromBasis)
/// \verbatim
///       | x1 x2 x3 Px |
///       | y1 y2 y3 Py |
///       | z1 z2 z3 Pz |
///       | 0  0  0  1  |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>::Matrix44 (const Vector3<T>& ejeX,
                              const Vector3<T>& ejeY,
                              const Vector3<T>& ejeZ,
                              const Vector3<T>& pos)
{
  SetFromBasis(ejeX, ejeY, ejeZ, pos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 7
/// (Ver comentario en SetFromAnglesYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>::Matrix44 (const T angleY, const T angleX, const T angleZ)
{
  SetFromAnglesYXZ(angleY, angleX, angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de igualdad absoluta
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Matrix44<T>::operator == (const Matrix44<T>& otra) const
{   
  return (0 == memcmp(this, &otra, sizeof(Matrix44<T>)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de desigualdad absoluta
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Matrix44<T>::operator != (const Matrix44<T>& otra) const
{   
  return (0 != memcmp(this, &otra, sizeof(Matrix44<T>)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Función de igualdad aproximada con epsilon genérico
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Matrix44<T>::IsEqualEpsilon (const Matrix44<T>& otra, const T Epsilon) const
{
  bool bIgual = true;
  for(int i=0; i<16; i++)
  {
    if(!mathUtils::EqualEpsilon(((T*)this)[i], ((T*)&otra)[i], Epsilon))
    {
      bIgual = false;
      break;
    }
  }

  return bIgual;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Función de desigualdad aproximada con epsilon genérico
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Matrix44<T>::IsNotEqualEpsilon (const Matrix44<T>& otra, const T Epsilon) const
{
  bool bDistinto = false;
  for(int i=0; i<16; i++)
  {
    if(!mathUtils::EqualEpsilon(((T*)this)[i], ((T*)&otra)[i], Epsilon))
    {
      bDistinto = true;
      break;
    }
  }

  return bDistinto;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Transformación del vector pasado en el parámetro. Equivalente al producto de la submatriz 3x3 de la
/// matriz actual (rotacion-escalado) por el vector.
/// [ver notas en el operador '^', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix44<T>::TransformVector (Vector3<T>& vector) const
{
  vector = (*this) ^ vector; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Transformación completa (rotacion-escalado-traslación) del vector pasado en el parámetro. Equivalente al
/// producto de la matriz actual por el vector (considerado como punto).
/// [ver notas en el operador '*', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix44<T>::TransformPoint (Vector3<T>& vector) const
{
  vector = (*this) * vector; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector que es la transformación del vector pasado en el parámetro. Aplicando solo la
/// rotación-escalado de la submatriz 3x3 de la matriz actual.
/// [ver notas en el operador '^', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix44<T>::GetTransformVector (const Vector3<T>& vector) const
{
  return (*this) ^ vector;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector que es la transformación completa (rotación-escalado-traslación) del vector pasado
/// en el parámetro (y considerado como punto)
/// [ver notas en el operador '*', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix44<T>::GetTransformPoint (const Vector3<T>& vector) const
{
  return (*this) * vector;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Aplicación de la transformación que contiene la submatriz 3x3 (rotación + escalado) a un array de vectores entrada,
/// situando el resultado en otro array de vectores salida
/// [ver notas en el operador '^', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Matrix44<T>::TransformArrayVectors (int iElements, Vector3<T>* pVecOUT, Vector3<T>* pVecIN) const
{
  for(int i=0; i<iElements; i++)
  {
    pVecOUT[i] = (*this) ^ (pVecIN[i]);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Aplicación de la transformación completa (rotación-escalado-traslación), que contiene la matriz, a un array
/// de vectores entrada (considerados como puntos) situando el resultado en otro array de vectores salida.
/// [ver notas en el operador '*', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Matrix44<T>::TransformArrayPoints (int iElements, Vector3<T>* pVecOUT, Vector3<T>* pVecIN) const
{
  for(int i=0; i<iElements; i++)
  {
    pVecOUT[i] = (*this) * (pVecIN[i]);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: producto de matriz x vector 3D (considerado como punto)
///                      equivalente a producto de matrices [4 4]x[4 1]
///                      equivalente a una transformación de TRASLACION + ROTACION + ESCALADO
///                      se supone la ultima fila igual a [0 0 0 1]
/// \verbatim
///         | m00 m01 m02 m03 |     | x |     | x*m00 + y*m01 + z*m02 + m03 |
///         | m10 m11 m21 m13 |  *  | y |  =  | x*m10 + y*m11 + z*m12 + m13 |
///         | m20 m21 m22 m23 |     | z |     | x*m20 + y*m21 + z*m22 + m23 |
///         | m30 m31 m32 m33 |     | 1 |     | x*m30 + y*m31 + z*m32 + m33 |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix44<T>::operator * (const Vector3<T>& vector) const
{
  return Vector3<T>(vector.x * m00 + vector.y * m01 + vector.z * m02 + m03,
                    vector.x * m10 + vector.y * m11 + vector.z * m12 + m13,
                    vector.x * m20 + vector.y * m21 + vector.z * m22 + m23);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: producto de matriz x vector 3D (usando solo la submatriz[0 0]-[2 2])
///                      equivalente a produto de matrices [3 3]x[3 1]
///                      equivalente a una transformación de ROTACION
/// \verbatim
///         | m00 m01 m02 |     | x |     | x*m00 + y*m01 + z*m02 |
///         | m10 m11 m12 |  ^  | y |  =  | x*m10 + y*m11 + z*m12 |
///         | m20 m21 m22 |     | z |     | x*m20 + y*m21 + z*m22 |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix44<T>::operator ^ (const Vector3<T>& vector) const
{
  return Vector3<T>(vector.x * m00 + vector.y * m01 + vector.z * m02,
                    vector.x * m10 + vector.y * m11 + vector.z * m12,
                    vector.x * m20 + vector.y * m21 + vector.z * m22);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: producto de matriz x vector 4D (completo)
///                      equivalente a producto de matrices [4 4]x[4 1]
/// \verbatim
///         | m00 m01 m02 m03 |     | x |     | x*m00 + y*m01 + z*m02 + w*m03 |
///         | m10 m11 m21 m13 |  *  | y |  =  | x*m10 + y*m11 + z*m12 + w*m13 |
///         | m20 m21 m22 m23 |     | z |     | x*m20 + y*m21 + z*m22 + w*m23 |
///         | m30 m31 m32 m33 |     | 1 |     | x*m30 + y*m31 + z*m32 + w*m33 |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Vector4<T> Matrix44<T>::operator * (const Vector4<T>& vector) const
{
  return Vector4<T>(vector.x * m00 + vector.y * m01 + vector.z * m02 + vector.w * m03,
                    vector.x * m10 + vector.y * m11 + vector.z * m12 + vector.w * m13,
                    vector.x * m20 + vector.y * m21 + vector.z * m22 + vector.w * m23,
                    vector.x * m30 + vector.y * m31 + vector.z * m32 + vector.w * m33);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Elimina la traslación de la matriz, considerándola como una
/// transformación afín de TRASLACION + ROTACION + ESCALADO
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::ResetTranslation()
{
  m03 = Zero<T>();
  m13 = Zero<T>();
  m23 = Zero<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Elimina la rotación de la matriz, considerándola como una transformación
/// afín de TRASLACION + ROTACION + ESCALADO. El escalado y la traslación
/// se mantienen invariables.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::ResetRotation ()
{
  T escale_x = GetVectorBasisLength(0);
  T escale_y = GetVectorBasisLength(1);
  T escale_z = GetVectorBasisLength(2);

  m00 = escale_x;   m01 = Zero<T>();  m02 = Zero<T>();
  m10 = Zero<T>();  m11 = escale_y;   m12 = Zero<T>();
  m20 = Zero<T>();  m21 = Zero<T>();  m22 = escale_z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Se elimina el escalado de la matriz, considerada como transformación
/// de TRASLACION + ROTACION + ESCALADO
/// Equivale a normalizar la matriz 3x3 que representa a la rotación
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::ResetScale ()
{
	T escale_x = 1.0f / GetVectorBasisLength(0);
	T escale_y = 1.0f / GetVectorBasisLength(1);
	T escale_z = 1.0f / GetVectorBasisLength(2);

	m00 *= escale_x;  m01 *= escale_y;  m02 *= escale_z;
	m10 *= escale_x;  m11 *= escale_y;  m12 *= escale_z;
	m20 *= escale_x;  m21 *= escale_y;  m22 *= escale_z;

  return (*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// Se elimina la rotación y el escalado. La submatriz de rotación 3x3 se
/// transforma en la matriz identidad
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::ResetRotationScale ()
{
  m00 = One<T>();   m01 = Zero<T>();  m02 = Zero<T>();
  m10 = Zero<T>();  m11 = One<T>();   m12 = Zero<T>();
  m20 = Zero<T>();  m21 = Zero<T>();  m22 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Cambia los datos de la matriz por los de una matriz identidad
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetIdentity ()
{
  m00 = One<T>();   m01 = Zero<T>();  m02 = Zero<T>();  m03 = Zero<T>();
  m10 = Zero<T>();  m11 = One<T>();   m12 = Zero<T>();  m13 = Zero<T>();
  m20 = Zero<T>();  m21 = Zero<T>();  m22 = One<T>();   m23 = Zero<T>();
  m30 = Zero<T>();  m31 = Zero<T>();  m32 = Zero<T>();  m33 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pone a cero todos los datos de la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetZero ()
{
  m00 = Zero<T>();  m01 = Zero<T>();  m02 = Zero<T>();  m03 = Zero<T>();
  m10 = Zero<T>();  m11 = Zero<T>();  m12 = Zero<T>();  m13 = Zero<T>();
  m20 = Zero<T>();  m21 = Zero<T>();  m22 = Zero<T>();  m23 = Zero<T>();
  m30 = Zero<T>();  m31 = Zero<T>();  m32 = Zero<T>();  m33 = Zero<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece los datos de la matriz a partir de 3 vectores 3D que forman una base y una posición 3D.
/// Se construye una transformación de TRASLACION + ROTACION + ESCALADO o una transformación típica de sistema de
/// coordenadas actual a sistema de coordenadas del padre (en cuyas coordenadas deben darse los vectores
/// parámetro y la posición).
/// \verbatim
///                          | x1 x2 x3 Px |
///                          | y1 y2 y3 Py |
///                          | z1 z2 z3 Pz |
///                          | 0  0  0  1  |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromBasis (const Vector3<T>& ejeX,
                                               const Vector3<T>& ejeY,
                                               const Vector3<T>& ejeZ,
                                               const Vector3<T>& pos)
{
  m00 = ejeX.x;     m01 = ejeY.x;     m02 = ejeZ.x;     m03 = pos.x;
  m10 = ejeX.y;     m11 = ejeY.y;     m12 = ejeZ.y;     m13 = pos.y;
  m20 = ejeX.z;     m21 = ejeY.z;     m22 = ejeZ.z;     m23 = pos.z;
  m30 = Zero<T>();  m31 = Zero<T>();  m32 = Zero<T>();  m33 = One<T>();

  return (*this);
}

template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromBasis (const Vector3<T>& ejeX,
																							 const Vector3<T>& ejeY,
																							 const Vector3<T>& ejeZ)
{
	m00 = ejeX.x;     m01 = ejeY.x;     m02 = ejeZ.x;
	m10 = ejeX.y;     m11 = ejeY.y;     m12 = ejeZ.y;
	m20 = ejeX.z;     m21 = ejeY.z;     m22 = ejeZ.z;

	return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleX' radianes alrededor del ejeX (normalmente inclinación)
/// que también es una matriz de sistema de referencia, con todos sus ejes rotados sobre el ejeX
/// del sistema de referencia padre
/// (Ver comentario en Matrix33::SetFromAngleX)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromAngleX (const T angleX)
{
  T sina;
  T cosa;
  mathUtils::SinCos(angleX, sina, cosa);

  m00 = One<T>();   m01 = Zero<T>();  m02 = Zero<T>();  m03 = Zero<T>();
  m10 = Zero<T>();  m11 = cosa;       m12 = -sina;      m13 = Zero<T>();
  m20 = Zero<T>();  m21 = sina;       m22 = cosa;       m23 = Zero<T>();
  m30 = Zero<T>();  m31 = Zero<T>();  m32 = Zero<T>();  m33 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleY' radianes alrededor del ejeY (normalmente alabeo)
/// que también es una matriz de sistema de referencia, con todos sus ejes rotados sobre el ejeY
/// del sistema de referencia padre
/// (Ver comentario en Matrix33::SetFromAngleY)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromAngleY (const T angleY)
{
  T sina;
  T cosa;
  mathUtils::SinCos(angleY, sina, cosa);

  m00 = cosa;       m01 = Zero<T>();  m02 = sina;       m03 = Zero<T>();
  m10 = Zero<T>();  m11 = One<T>();   m12 = Zero<T>();  m13 = Zero<T>();
  m20 = -sina;      m21 = Zero<T>();  m22 = cosa;       m23 = Zero<T>();
  m30 = Zero<T>();  m31 = Zero<T>();  m32 = Zero<T>();  m33 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleZ' radianes alrededor del ejeZ (normalmente giro)
/// que también es una matriz de sistema de referencia, con todos sus ejes rotados sobre el ejeZ
/// del sistema de referencia padre
/// (Ver comentario en Matrix33::SetFromAngleZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromAngleZ (const T angleZ)
{
  T sina;
  T cosa;
  mathUtils::SinCos(angleZ, sina, cosa);

  m00 = cosa;       m01 = -sina;      m02 = Zero<T>();  m03 = Zero<T>();
  m10 = sina;       m11 = cosa;       m12 = Zero<T>();  m13 = Zero<T>();
  m20 = Zero<T>();  m21 = Zero<T>();  m22 = One<T>();   m23 = Zero<T>();
  m30 = Zero<T>();  m31 = Zero<T>();  m32 = Zero<T>();  m33 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleX' radianes alrededor del ejeX +
/// 'angleZ' alrededor del eje z (primero inclinación y luego giro).
/// También es una matriz de sistema de referencia, con todos sus ejes rotados de la misma
/// forma en el sistema de referencia padre.
/// (Ver comentario en Matrix33::SetFromAnglesXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromAnglesXZ (const T angleX, const T angleZ)
{
  T sinx, cosx, sinz, cosz;
  mathUtils::SinCos(angleX, sinx, cosx);
  mathUtils::SinCos(angleZ, sinz, cosz);

  m00 = cosz;       m01 = -sinz * cosx;  m02 = sinz * sinx;   m03 = Zero<T>();
  m10 = sinz;       m11 = cosz * cosx;   m12 = -cosz * sinx;  m13 = Zero<T>();
  m20 = Zero<T>();  m21 = sinx;          m22 = cosx;          m23 = Zero<T>();
  m30 = Zero<T>();  m31 = Zero<T>();     m32 = Zero<T>();     m33 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleY' radianes alrededor del ejeY +
/// 'angleX' radianes alrededor del ejeX + 'angleZ' alrededor del eje z
/// (primero alabeo, luego inclinación y luego giro).
/// También es una matriz de sistema de referencia, con todos sus ejes rotados de la misma
/// forma en el sistema de referencia padre.
/// (Ver comentario en Matrix33::SetFromAnglesYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromAnglesYXZ (const T angleY, const T angleX, const T angleZ)
{
  T sx, cx, sy, cy, sz, cz;
  mathUtils::SinCos(angleX, sx, cx);
  mathUtils::SinCos(angleY, sy, cy);
  mathUtils::SinCos(angleZ, sz, cz);

  m00 = cz*cy - sz*sx*sy;  m01 = -sz*cx;     m02 = cz*sy + sz*sx*cy;  m03 = Zero<T>();
  m10 = sz*cy + cz*sx*sy;  m11 = cz*cx;      m12 = sz*sy - cz*sx*cy;  m13 = Zero<T>();
  m20 = -cx*sy;            m21 = sx;         m22 = cx*cy;             m23 = Zero<T>();
  m30 = Zero<T>();         m31 = Zero<T>();  m32 = Zero<T>();         m33 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Construye una matriz de traslación, actualizando los datos de la última columna
/// a partir de 3 magnitudes.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromPos (const T posX, const T posY, const T posZ)
{
  m00 = One<T>();    m01 = Zero<T>();   m02 = Zero<T>();   m03 = posX;
  m10 = Zero<T>();   m11 = One<T>();    m12 = Zero<T>();   m13 = posY;
  m20 = Zero<T>();   m21 = Zero<T>();   m22 = One<T>();    m23 = posZ;
  m30 = Zero<T>();   m31 = Zero<T>();   m32 = Zero<T>();   m33 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Construye una matriz de traslación, actualizando los datos de la última columna
/// a partir de un vector 3D.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromPos (const Vector3<T>& pos)
{
  m00 = One<T>();    m01 = Zero<T>();   m02 = Zero<T>();   m03 = pos.x;
  m10 = Zero<T>();   m11 = One<T>();    m12 = Zero<T>();   m13 = pos.y;
  m20 = Zero<T>();   m21 = Zero<T>();   m22 = One<T>();    m23 = pos.z;
  m30 = Zero<T>();   m31 = Zero<T>();   m32 = Zero<T>();   m33 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Construye una matriz de escala, actualizando la diagonal de la submatriz 3x3
/// en las 3 primeras columnas, a partir de una magnitud para cada eje de la base
/// que forma la transformación de la matriz.
/// \verbatim
///   | escala_X     0         0     |
///   |    0      escala_Y     0     |
///   |    0         0      escala_Z |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromScale (const T escala_x, const T escala_y, const T escala_z)
{
  m00 = escala_x;    m01 = Zero<T>();   m02 = Zero<T>();   m03 = Zero<T>();
  m10 = Zero<T>();   m11 = escala_y;    m12 = Zero<T>();   m13 = Zero<T>();
  m20 = Zero<T>();   m21 = Zero<T>();   m22 = escala_z;    m23 = Zero<T>();
  m30 = Zero<T>();   m31 = Zero<T>();   m32 = Zero<T>();   m33 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la matriz de forma que el eje Y tendrá la dirección y el sentido del vector que va
/// desde 'vPos' hasta 'vTarget', utilizando el vector 'vUp' como referencia del sentido del 'arriba' del
/// mundo, para poder construir un sistema de referencia con 3 vectores típico de una matriz de cámara, siendo
/// el eje Y el vector de vista.
/// @Note1 El vector vUp debe estar normalizado.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromLookAt (const Vector3<T>& vPos, const Vector3<T>& vTarget, const Vector3<T>& vUp)
{
  Vector3<T> vLookAt_eje_Z = (vTarget - vPos).Normalize();

#ifdef CHECK_MATH_SINGULARITY_LOOK_AT
  //--------------------------------------------------------------------<<<
  // Se comprueba cuando el vector 'look_at' tiene la misma dirección que el vector 'up'.
  // En este caso el producto vectorial falla al intentar calcular el vector 'right' ya que da (0,0,0).
  // La solución es desviar un poco el vector 'up' al detectar la condición.
  if(vUP.IsNotEqualEpsilon(v3fY))
  {
    if(vLookAt_eje_Z.IsEqualEpsilon(vUp, (T)0.0001))
    {
      vUP.y -= (T)0.1;
    }
    else
    if(vLookAt_eje_Z.IsEqualEpsilon(-vUp, (T)0.0001))
    {
      vUP.y += (T)0.1;
    }
  }
  else
  {
    if(vLookAt_eje_Z.IsEqualEpsilon(vUp, (T)0.0001))
    {
      vUP.z += (T)0.1;
    }
    else
    if(vLookAt_eje_Z.IsEqualEpsilon(-vUp, (T)0.0001))
    {
      vUP.z -= (T)0.1;
    }
  }
  //-------------------------------------------------------------------->>>
#endif

  Vector3<T> vRight_eje_X = vUp ^ vLookAt_eje_Z;
  vRight_eje_X.Normalize();
  Vector3<T> vUP_eje_Y = vLookAt_eje_Z ^ vRight_eje_X;

  SetFromBasis(vRight_eje_X, vUP_eje_Y, vLookAt_eje_Z, vPos);

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la matriz de forma que el eje Y tendrá la dirección y el sentido del vector que va
/// desde 'vPos' hasta 'vTarget', utilizando el vector 'vUp' como referencia del sentido del 'arriba' del
/// mundo, para poder construir un sistema de referencia con 3 vectores típico de una matriz de cámara, siendo
/// el eje Y el vector de vista.
/// @Note1 El vector 'up' se considera (0,0,1) en este caso. Apuntando hacía arriba en las condiciones normales
///        del sistema de referencia adoptado.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromLookAt (const Vector3<T>& vPos, const Vector3<T>& vTarget)
{
  return SetFromLookAt(vPos, vTarget, v3fY);
}
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la matriz a partir de un cuaternión para la parte de rotación y un vector para la parte de posición
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetFromQuatPos (const Quatn<T>& quat, const Vector3<T>& vPos)
{
  T x = quat.x;
  T y = quat.y;
  T z = quat.z;
  T w = quat.w;

  T xx  =  x + x;
  T yy  =  y + y;
  T zz  =  z + z;
  T wx  =  w * xx;
  T wy  =  w * yy;
  T wz  =  w * zz;
  T xxx = xx * x;
  T yyx = yy * x;
  T zzx = zz * x;
  T yyy = yy * y;
  T zzy = zz * y;
  T zzz = zz * z;

  m00 = One<T>() - (yyy + zzz);  m01 = yyx - wz;                m02 = zzx + wy;               m03 = vPos.x;
  m10 = yyx + wz;                m11 = One<T>() - (xxx + zzz);  m12 = zzy - wx;               m13 = vPos.y;
  m20 = zzx - wy;                m21 = zzy + wx;                m22 = One<T>() - (xxx + yyy); m23 = vPos.z;
  m30 = Zero<T>();               m31 = Zero<T>();               m32 = Zero<T>();              m33 = One<T>();

  return (*this);
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Igual que 'SetFromAngleX' pero solo modifica la submatriz 3x3 para cambiar la rotación
/// dejando la traslación y el escalado invariables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetRotByAngleX (const T angleX)
{
  ResetRotation();

	Vector2<T> v;

	// eje X
	v.x = m10;
	v.y = m20;
	v.Rotate(angleX);
	m10 = v.x;
	m20 = v.y;

	// eje Y
	v.x = m11;
	v.y = m21;
	v.Rotate(angleX);
	m11 = v.x;
	m21 = v.y;

	// eje Z
	v.x = m12;
	v.y = m22;
	v.Rotate(angleX);
	m12 = v.x;
	m22 = v.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Igual que 'SetFromAngleY' pero solo modifica la submatriz 3x3 para cambiar la rotación
/// dejando la traslación y el escalado invariables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetRotByAngleY (const T angleY)
{
  ResetRotation();
  
	Vector2<T> v;

	// eje X
	v.x = m20;
	v.y = m00;
	v.Rotate(angleY);
	m20 = v.x;
	m00 = v.y;

	// eje Y
	v.x = m21;
	v.y = m01;
	v.Rotate(angleY);
	m21 = v.x;
	m01 = v.y;

	// eje Z
	v.x = m22;
	v.y = m02;
	v.Rotate(angleY);
	m22 = v.x;
	m02 = v.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Igual que 'SetFromAngleZ' pero solo modifica la submatriz 3x3 para cambiar la rotación
/// dejando la traslación y el escalado invariables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetRotByAngleZ (const T angleZ)
{
  ResetRotation();
  
	Vector2<T> v;

	// eje X
	v.x = m00;
	v.y = m10;
	v.Rotate(angleZ);
	m00 = v.x;
	m10 = v.y;

	// eje Y
	v.x = m01;
	v.y = m11;
	v.Rotate(angleZ);
	m01 = v.x;
	m11 = v.y;

	// eje Z
	v.x = m02;
	v.y = m12;
	v.Rotate(angleZ);
	m02 = v.x;
	m12 = v.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Igual que 'SetFromAngleXZ' pero solo modifica la submatriz 3x3 para cambiar la rotación
/// dejando la traslación y el escalado invariables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetRotByAnglesXZ (const T angleX, const T angleZ)
{
  ResetRotation();
  
	Vector2<T> v;

	////////////
	// RotX
	////////////

	// eje X
	v.x = m10;
	v.y = m20;
	v.Rotate(angleX);
	m10 = v.x;
	m20 = v.y;

	// eje Y
	v.x = m11;
	v.y = m21;
	v.Rotate(angleX);
	m11 = v.x;
	m21 = v.y;

	// eje Z
	v.x = m12;
	v.y = m22;
	v.Rotate(angleX);
	m12 = v.x;
	m22 = v.y;

	////////////
	// RotZ
	////////////

	// eje X
	v.x = m00;
	v.y = m10;
	v.Rotate(angleZ);
	m00 = v.x;
	m10 = v.y;

	// eje Y
	v.x = m01;
	v.y = m11;
	v.Rotate(angleZ);
	m01 = v.x;
	m11 = v.y;

	// eje Z
	v.x = m02;
	v.y = m12;
	v.Rotate(angleZ);
	m02 = v.x;
	m12 = v.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Igual que 'SetFromAngleXZ' pero solo modifica la submatriz 3x3 para cambiar la rotación
/// dejando la traslación y el escalado invariables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetRotByAnglesYXZ (const T angleY, const T angleX, const T angleZ)
{
  ResetRotation();
  
	Vector2<T> v;

	////////////
	// RotY
	////////////

	// eje X
	v.x = m20;
	v.y = m00;
	v.Rotate(angleY);
	m20 = v.x;
	m00 = v.y;

	// eje Y
	v.x = m21;
	v.y = m01;
	v.Rotate(angleY);
	m21 = v.x;
	m01 = v.y;

	// eje Z
	v.x = m22;
	v.y = m02;
	v.Rotate(angleY);
	m22 = v.x;
	m02 = v.y;

	////////////
	// RotX
	////////////

	// eje X
	v.x = m10;
	v.y = m20;
	v.Rotate(angleX);
	m10 = v.x;
	m20 = v.y;

	// eje Y
	v.x = m11;
	v.y = m21;
	v.Rotate(angleX);
	m11 = v.x;
	m21 = v.y;

	// eje Z
	v.x = m12;
	v.y = m22;
	v.Rotate(angleX);
	m12 = v.x;
	m22 = v.y;

	////////////
	// RotZ
	////////////

	// eje X
	v.x = m00;
	v.y = m10;
	v.Rotate(angleZ);
	m00 = v.x;
	m10 = v.y;

	// eje Y
	v.x = m01;
	v.y = m11;
	v.Rotate(angleZ);
	m01 = v.x;
	m11 = v.y;

	// eje Z
	v.x = m02;
	v.y = m12;
	v.Rotate(angleZ);
	m02 = v.x;
	m12 = v.y;

  return (*this);
}
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la rotación de la matriz a partir de un cuaternión dejando la posición y el escalado invariables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetRotByQuat (const Quatn<T>& quat)
{
  Vector3<T> vSavePos   = GetPos();
  Vector3<T> vSaveScale = GetScale();

  SetFromQuatPos(quat, vSavePos);
  SetScale(vSaveScale);

  return (*this);
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la parte de la cuarta columna que representa la traslación,
/// considerando a la matriz como una transformación afín
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetPos (const T posX, const T posY, const T posZ)
{
  m03 = posX;
  m13 = posY;
  m23 = posZ;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión con un vector 3D como parámetro
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetPos (const Vector3<T>& pos)
{
  m03 = pos.x;
  m13 = pos.y;
  m23 = pos.z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece el escalado de la matriz considerada como una transformación.
/// Las partes de traslación y rotación permenecen invariables.
/// El proceso es:
///   - Eliminar el escalado actual (normalizando los vectores base que definen los ejes)
///   - Establecer el nuevo escalado. Multiplicando cada vector base por su escalado correspondiente.
/// \verbatim
/// NOTA: La matriz parámetro es considerada una matriz de escalado 3x3:
/// 
///       | escala_x     0         0     |
///       |    0      escala_y     0     |
///       |    0         0      escala_z |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetScale (const Matrix33<T>& mat_escala)
{
	T scale_x = mat_escala.m00 / GetVectorBasisLength(0);
	T scale_y = mat_escala.m11 / GetVectorBasisLength(1);
	T scale_z = mat_escala.m22 / GetVectorBasisLength(2);

	m00 *= scale_x;  m01 *= scale_y;  m02 = *= scale_z;
	m10 *= scale_x;  m11 *= scale_y;  m12 = *= scale_z;
	m20 *= scale_x;  m21 *= scale_y;  m22 = *= scale_z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece el escalado de la matriz considerada como una transformación.
/// Las partes de traslación y rotación permenecen invariables.
/// El proceso es:
///   - Eliminar el escalado actual (normalizando los vectores base que definen los ejes)
///   - Establecer el nuevo escalado. Multiplicando cada vector base por su escalado correspondiente.
/// \verbatim
/// NOTA: La matriz parámetro es considerada una matriz de escalado 4x4:
///
///       | escala_x     0         0        0  |
///       |    0      escala_y     0        0  |
///       |    0         0      escala_z    0  |
///       |    0         0         0        1  |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetScale (const Matrix44<T>& mat_escala)
{
	T scale_x = mat_escala.m00 / GetVectorBasisLength(0);
	T scale_y = mat_escala.m11 / GetVectorBasisLength(1);
	T scale_z = mat_escala.m22 / GetVectorBasisLength(2);

	m00 *= scale_x;  m01 *= scale_y;  m02 = *= scale_z;
	m10 *= scale_x;  m11 *= scale_y;  m12 = *= scale_z;
	m20 *= scale_x;  m21 *= scale_y;  m22 = *= scale_z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece el escalado de la matriz considerada como una transformación.
/// Las partes de traslación y rotación permenecen invariables.
/// El proceso es:
///   - Eliminar el escalado actual (normalizando los vectores base que definen los ejes)
///   - Establecer el nuevo escalado. Multiplicando cada vector base por su escalado correspondiente.
///
/// El parámetro es un vector 3D cuyas componentes son las escalas
/// en los tres ejes X, Y, Z.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetScale (const Vector3<T>& vect_escala)
{
	T scale_x = vect_escala.x / GetVectorBasisLength(0);
	T scale_y = vect_escala.y / GetVectorBasisLength(1);
	T scale_z = vect_escala.z / GetVectorBasisLength(2);

	m00 *= scale_x;  m01 *= scale_y;  m02 *= scale_z;
	m10 *= scale_x;  m11 *= scale_y;  m12 *= scale_z;
	m20 *= scale_x;  m21 *= scale_y;  m22 *= scale_z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la matriz como una matriz de escalado a partir de tres magnitudes
/// consideradas como escalas en los tres ejes (escala_x, escala_y, escala_z)
///
/// La parte de rotación permenece invariable.
/// El proceso es:
///   - Eliminar el escalado actual (normalizando los vectores base que definen los ejes)
///   - Establecer el nuevo escalado. Multiplicando cada vector base por su escalado correspondiente.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetScale (const T escala_x, const T escala_y, const T escala_z)
{
	T scale_x = escala_x / GetVectorBasisLength(0);
	T scale_y = escala_y / GetVectorBasisLength(1);
	T scale_z = escala_z / GetVectorBasisLength(2);

	m00 *= scale_x;  m01 *= scale_y;  m02 *= scale_z;
	m10 *= scale_x;  m11 *= scale_y;  m12 *= scale_z;
	m20 *= scale_x;  m21 *= scale_y;  m22 *= scale_z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la submatriz 3x3 de la matriz actual
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetSubMatrix33 (const Matrix33<T>& mat_sub)
{
  m00 = mat_sub.m00;   m01 = mat_sub.m01;   m02 = mat_sub.m02;
  m10 = mat_sub.m10;   m11 = mat_sub.m11;   m12 = mat_sub.m12;
  m20 = mat_sub.m20;   m21 = mat_sub.m21;   m22 = mat_sub.m22;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la submatriz 3x4 de la matriz actual
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetSubMatrix34 (const Matrix34<T>& mat_sub)
{
  m00 = mat_sub.m00;   m01 = mat_sub.m01;   m02 = mat_sub.m02;   m03 = mat_sub.m03;
  m10 = mat_sub.m10;   m11 = mat_sub.m11;   m12 = mat_sub.m12;   m13 = mat_sub.m13;
  m20 = mat_sub.m20;   m21 = mat_sub.m21;   m22 = mat_sub.m22;   m23 = mat_sub.m23;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve los 3 primeros elementos de una columna, interpretados como un
/// vector 3D que forma la base que define la matriz en coordenadas del
/// sistema de referencia padre
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix44<T>::GetVectorBasis (int i) const
{
#ifdef ROW_MAJOR_MATRICES 
	return Vector3<T>( ((T*)this)[i],
										 ((T*)this)[4  + i],
										 ((T*)this)[8  + i]);
#else
	return *(Vector3<T>*)((Vector4<T>*)this + i);
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve el módulo del vector formado por los 3  elementos de una columna
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix44<T>::GetVectorBasisLength (int i) const
{
#ifdef ROW_MAJOR_MATRICES
	return Vector3<T>(((T*)this)[i],
										((T*)this)[4 + i],
										((T*)this)[8 + i] ).Length();
#else
	return (*(Vector3<T>*)((Vector4<T>*)this + i)).Length();
#endif

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve los 3 primeros elementos de la última columna, interpretados
/// como la posición de la base que define la matriz en coordenadas del
/// sistema de referencia padre
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix44<T>::GetPosBasis () const
{
  return GetVectorBasis(3);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve los 3 primeros elementos de cada columna, interpretados
/// como vectores 3D y como una base con su posición en coordenadas del
/// sistema de referencia padre
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix44<T>::GetBasis (Vector3<T>& vBasisX,
                                   Vector3<T>& vBasisY,
                                   Vector3<T>& vBasisZ,
                                   Vector3<T>& Pos) const
{
  vBasisX = GetVectorBasis(0);
  vBasisY = GetVectorBasis(1);
  vBasisZ = GetVectorBasis(2);
  Pos     = GetVectorBasis(3);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la submatriz matriz 3x3, que suele contener la parte de
/// rotación + escalado, cuando la matriz es una transformación
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T> Matrix44<T>::GetSubMatrix33 () const
{
   return Matrix33<T>(m00, m01, m02,
                      m10, m11, m12,
                      m20, m21, m22);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Rellena la matriz 3x3 del parámetro con la submatriz 3x3
/// de la matriz actual
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix44<T>::GetSubMatrix33 (Matrix33<T>& subMat) const
{
  subMat.m00 = m00;  subMat.m01 = m01;  subMat.m02 = m02;
  subMat.m10 = m10;  subMat.m11 = m11;  subMat.m12 = m12;
  subMat.m20 = m20;  subMat.m21 = m21;  subMat.m22 = m22;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la submatriz 3x4 de la matriz actual, que suele contener
/// la transformación afín completa: traslación + rotación + escalado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T> Matrix44<T>::GetSubMatrix34 () const
{
  return Matrix34<T>(m00, m01, m02, m03,
                     m10, m11, m12, m13,
                     m20, m21, m22, m23);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Rellena la matriz 3x4 del parámetro con la submatriz 3x4
/// de la matriz actual, que suele contener la transformación afín
/// completa: traslación + rotación + escalado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix44<T>::GetSubMatrix34 (Matrix34<T>& subMat) const
{
  subMat.m00 = m00;  subMat.m01 = m01;  subMat.m02 = m02;  subMat.m03 = m03;
  subMat.m10 = m10;  subMat.m11 = m11;  subMat.m12 = m12;  subMat.m13 = m13;
  subMat.m20 = m20;  subMat.m21 = m21;  subMat.m22 = m22;  subMat.m23 = m23;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve una matriz 3x3 con la parte de rotación, considerando
/// a la matriz compuesta por una traslacion + rotación + escalado
/// (Versión que anula el escalado)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T> Matrix44<T>::Get33RotationNormalized () const
{
  return Matrix33<T>(GetVectorBasis(0).GetNormalized(),
                     GetVectorBasis(1).GetNormalized(),
                     GetVectorBasis(2).GetNormalized());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve una matriz 3x3 con la parte de rotación + escalado, considerando
/// a la matriz compuesta por una traslacion + rotación + escalado
/// (Versión que contiene el escalado)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T> Matrix44<T>::Get33RotationScaled () const
{
  return Matrix33<T>(m00, m01, m02,
                     m10, m11, m12,
                     m20, m21, m22);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la matriz de escalado. Considerando a la matriz compuesta por
/// una traslación + rotación + escalado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T> Matrix44<T>::Get33Scale() const
{
  return Matrix33<T>(GetVectorBasis(0).Length(),            Zero<T>()      ,          Zero<T>(),
                            Zero<T>()          , GetVectorBasis(1).Length(),          Zero<T>(),
                            Zero<T>()          ,            Zero<T>()      , GetVectorBasis(2).Length());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la parte de traslación. Considerando a la matriz compuesta por
/// una traslación + rotación + escalado
/// (Versión que devuelve un vector de traslación)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix44<T>::GetTranslationVector () const
{
  return Vector3<T>(m03, m13, m23);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la parte de rotación de la matriz, considerándola como
/// una transformación de traslación + rotación + escalado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T> Matrix44<T>::Get44RotationScaled () const
{
  return Matrix44<T>(  m00,       m01,       m02,     Zero<T>(),
                       m10,       m11,       m12,     Zero<T>(),
                       m20,       m21,       m22,     Zero<T>(),
                     Zero<T>(), Zero<T>(), Zero<T>(), One<T>());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la parte de rotación de la matriz, considerándola como
/// una transformación de traslación + rotación + escalado
/// (Versión que elimina el escalado)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T> Matrix44<T>::Get44RotationNormalized () const
{
  return Matrix44<T>(GetVectorBasis(0).GetNormalized(),
                     GetVectorBasis(1).GetNormalized(),
                     GetVectorBasis(2).GetNormalized(),
                     Vector3<T>(Zero<T>(), Zero<T>(), Zero<T>()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la parte de traslación de la matriz, considerándola 
/// como una transformación de traslación + rotación + escalado,
/// como una matriz 4x4
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T> Matrix44<T>::Get44Translation () const
{
  return Matrix44<T>(One<T>(),  Zero<T>(), Zero<T>(), m03,
                     Zero<T>(), One<T>(),  Zero<T>(), m13,
                     Zero<T>(), Zero<T>(), One<T>(),  m23,
                     Zero<T>(), Zero<T>(), Zero<T>(), One<T>());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la parte de escalado de la matriz, considerándola
/// como una transformación de traslación + rotación + escalado,
/// como una matriz 4x4
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T> Matrix44<T>::Get44Scale () const
{
  return Matrix44<T>(GetVectorBasis(0).Length(),            Zero<T>()      ,          Zero<T>()        ,  Zero<T>(),
                            Zero<T>()          , GetVectorBasis(1).Length(),          Zero<T>()        ,  Zero<T>(),
                            Zero<T>()          ,            Zero<T>()      , GetVectorBasis(2).Length(),  Zero<T>(),
                            Zero<T>()          ,            Zero<T>()      ,          Zero<T>()        ,  One<T>());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la posicion de la matriz, considerándola como una transformación 
/// de traslación + rotación + escalado, como un vector3
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix44<T>::GetPos () const
{
  return Vector3<T>(m03, m13, m23);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la escala de la matriz en los tres ejes en forma de vector 3D, considerando la base del sistema
/// de referencia que define.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix44<T>::GetScale () const
{
  return Vector3<T>(GetVectorBasis(0).Length(),
                    GetVectorBasis(1).Length(),
                    GetVectorBasis(2).Length());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la escala de la matriz en el ejeX, considerando la base
/// del sistema de referencia que define.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix44<T>::GetScaleX () const
{
  return GetVectorBasis(0).Length();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la escala de la matriz en el ejeY, considerando la base
/// del sistema de referencia que define.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix44<T>::GetScaleY () const
{
  return GetVectorBasis(1).Length();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la escala de la matriz en el ejeZ, considerando la base
/// del sistema de referencia que define.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix44<T>::GetScaleZ () const
{
  return GetVectorBasis(2).Length();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención de ángulos de euler (Ver comentario en Matrix33::GetAnglesYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Matrix44<T>::GetAnglesYXZ (T& angleY, T& angleX, T& angleZ) const
{
  angleY = mathUtils::ATan2(-m20, m22);
  angleX = mathUtils::ASin(m21);
  angleZ = mathUtils::ATan2(-m01, m11);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención del angulo de Euler que representa la rotación sobre el eje X
/// (Ver comentario en Matrix33::GetAnglesYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
T Matrix44<T>::GetAngleX () const
{
  return mathUtils::ASin(m21);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención del angulo de Euler que representa la rotación sobre el eje Y
/// (Ver comentario en Matrix33::GetAnglesYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
T Matrix44<T>::GetAngleY () const
{
  return mathUtils::ATan2(-m20, m22);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención del angulo de Euler que representa la rotación sobre el eje Z
/// (Ver comentario en Matrix33::GetAnglesYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
T Matrix44<T>::GetAngleZ () const
{
  return mathUtils::ATan2(-m01, m11);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el ángulo de rotación sobre el eje Y, siguiendo el criterio de ángulos
/// y orden de concatenación explicado en 'Matrix33::SetPitchRollYaw'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix44<T>::GetRoll () const
{
  return mathUtils::ATan2(m20, m00);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el ángulo de rotación sobre el eje X, siguiendo el criterio de ángulos
/// y orden de concatenación expresado en 'Matrix33::SetPitchRollYaw'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix44<T>::GetPitch () const
{
  return mathUtils::ATan2(-m12, m11);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el ángulo de rotación sobre el eje Z, siguiendo el criterio de ángulos
/// y orden de concatenación expresado en 'SetPitchRollYaw'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix44<T>::GetYaw () const
{
  return mathUtils::ASin(-m10);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención de los ángulos de rotación sobre los ejes, aplicados con el criterio 'PitchRollYaw' explicado
/// en 'Matrix33::SetPitchRollYaw'.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix44<T>::GetPitchRollYaw () const
{
  return Vector3<T>(mathUtils::ATan2(-m12, m11),
                    mathUtils::ATan2(m20, m00),
                    mathUtils::ASin(-m10));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Construye una matriz de rotación a partir de los ángulos de rotación sobre los ejes, aplicados con el criterio
/// 'PitchRollYaw' explicado en 'Matrix33::SetPitchRollYaw'. Esta función pone a cero la parte de posición.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::SetFromPitchRollYaw (const Vector3<T>& v3PitchRollYaw)
{
  T sx, cx, sy, cy, sz, cz;
  mathUtils::SinCos(v3PitchRollYaw.x, sx, cx);
  mathUtils::SinCos(v3PitchRollYaw.y, sy, cy);
  mathUtils::SinCos(v3PitchRollYaw.z, sz, cz);

  m00 = cy*cz;     m01 = cy*sz*cx - sy*sx;  m02 = -cy*sz*sx - sy*cx;  m03 = Zero<T>(); 
  m10 = -sz;       m11 = cz*cx;             m12 = -cz*sx;             m13 = Zero<T>();
  m20 = sy*cz;     m21 = sy*sz*cx + cy*sx;  m22 = -sy*sz*sx + cy*cx;  m23 = Zero<T>();
  m30 = Zero<T>(); m31 = Zero<T>();         m32 = Zero<T>();          m33 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establecimiento de los ángulos de rotación sobre los ejes, aplicados con el criterio 'PitchRollYaw' explicado
/// en 'Matrix33::SetPitchRollYaw'. Esta función modifica solo la parte rotación.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T>& Matrix44<T>::SetPitchRollYaw (const Vector3<T>& v3PitchRollYaw)
{
  T sx, cx, sy, cy, sz, cz;
  mathUtils::SinCos(v3PitchRollYaw.x, sx, cx);
  mathUtils::SinCos(v3PitchRollYaw.y, sy, cy);
  mathUtils::SinCos(v3PitchRollYaw.z, sz, cz);

  m00 = cy*cz;     m01 = cy*sz*cx - sy*sx;  m02 = -cy*sz*sx - sy*cx;
  m10 = -sz;       m11 = cz*cx;             m12 = -cz*sx;
  m20 = sy*cz;     m21 = sy*sz*cx + cy*sx;  m22 = -sy*sz*sx + cy*cx;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Acceso a filas como vectores 4D
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector4<T> Matrix44<T>::GetRow (int i) const
{
#ifdef ROW_MAJOR_MATRICES
	return *((Vector4<T>*)this + i);
#else
	return Vector4<T>( ((T*)this)[i],
		((T*)this)[4  + i],
		((T*)this)[8  + i],
		((T*)this)[12 + i] );
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Acceso a columnas como vectores 4D
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector4<T> Matrix44<T>::GetColum (int i) const
{
#ifdef ROW_MAJOR_MATRICES
	return Vector4<T>( ((T*)this)[i],
		((T*)this)[4  + i],
		((T*)this)[8  + i],
		((T*)this)[12 + i] );
#else
	return *((Vector4<T>*)this + i);
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: suma de matrices
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> Matrix44<T>::operator + (const Matrix44<T>& otra) const
{
  return (Matrix44<T>(m00 + otra.m00, m01 + otra.m01, m02 + otra.m02, m03 + otra.m03,  
                      m10 + otra.m10, m11 + otra.m11, m12 + otra.m12, m13 + otra.m13,
                      m20 + otra.m20, m21 + otra.m21, m22 + otra.m22, m23 + otra.m23,
                      m30 + otra.m30, m31 + otra.m31, m32 + otra.m32, m33 + otra.m33));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: resta de matrices
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> Matrix44<T>::operator - (const Matrix44<T> &otra) const
{
  return (Matrix44<T>(m00 - otra.m00, m01 - otra.m01, m02 - otra.m02, m03 - otra.m03,  
                      m10 - otra.m10, m11 - otra.m11, m12 - otra.m12, m13 - otra.m13,
                      m20 - otra.m20, m21 - otra.m21, m22 - otra.m22, m23 - otra.m23,
                      m30 - otra.m30, m31 - otra.m31, m32 - otra.m32, m33 - otra.m33));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: producto de matrices [4 4] x [4 4]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> Matrix44<T>::operator * (const Matrix44<T>& otra) const
{
  return (Matrix44<T>(m00 * otra.m00 + m01 * otra.m10 + m02 * otra.m20 + m03 * otra.m30,
                      m00 * otra.m01 + m01 * otra.m11 + m02 * otra.m21 + m03 * otra.m31,
                      m00 * otra.m02 + m01 * otra.m12 + m02 * otra.m22 + m03 * otra.m32,
                      m00 * otra.m03 + m01 * otra.m13 + m02 * otra.m23 + m03 * otra.m33,

                      m10 * otra.m00 + m11 * otra.m10 + m12 * otra.m20 + m13 * otra.m30,
                      m10 * otra.m01 + m11 * otra.m11 + m12 * otra.m21 + m13 * otra.m31,
                      m10 * otra.m02 + m11 * otra.m12 + m12 * otra.m22 + m13 * otra.m32,
                      m10 * otra.m03 + m11 * otra.m13 + m12 * otra.m23 + m13 * otra.m33,

                      m20 * otra.m00 + m21 * otra.m10 + m22 * otra.m20 + m23 * otra.m30,
                      m20 * otra.m01 + m21 * otra.m11 + m22 * otra.m21 + m23 * otra.m31,
                      m20 * otra.m02 + m21 * otra.m12 + m22 * otra.m22 + m23 * otra.m32,
                      m20 * otra.m03 + m21 * otra.m13 + m22 * otra.m23 + m23 * otra.m33,

                      m30 * otra.m00 + m31 * otra.m10 + m32 * otra.m20 + m33 * otra.m30,
                      m30 * otra.m01 + m31 * otra.m11 + m32 * otra.m21 + m33 * otra.m31,          
                      m30 * otra.m02 + m31 * otra.m12 + m32 * otra.m22 + m33 * otra.m32, 
                      m30 * otra.m03 + m31 * otra.m13 + m32 * otra.m23 + m33 * otra.m33));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: producto matriz x escalar 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> Matrix44<T>::operator * (const T escalar) const
{
  return (Matrix44<T>(m00 * escalar, m01 * escalar, m02 * escalar, m03 * escalar,
                      m10 * escalar, m11 * escalar, m12 * escalar, m13 * escalar,
                      m20 * escalar, m21 * escalar, m22 * escalar, m23 * escalar,
                      m30 * escalar, m31 * escalar, m32 * escalar, m33 * escalar));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador externo: producto escalar x matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> operator * (const T escalar, const Matrix44<T>& mat)
{
  return (Matrix44<T>(mat.m00 * escalar, mat.m01 * escalar, mat.m02 * escalar, mat.m03 * escalar,  
                      mat.m10 * escalar, mat.m11 * escalar, mat.m12 * escalar, mat.m13 * escalar,  
                      mat.m20 * escalar, mat.m21 * escalar, mat.m22 * escalar, mat.m23 * escalar,  
                      mat.m30 * escalar, mat.m31 * escalar, mat.m32 * escalar, mat.m33 * escalar));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: cociente matriz / escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> Matrix44<T>::operator / (const T escalar) const
{
  T inv_escalar = One<T>();

#ifdef CHECK_MATH_SINGULARITY
  //--------------------------------------------------------------<<<
  // Con chequeo de división por cero
  ASSERTMSG(escalar != Zero<T>(), "División por cero en cociente matriz4x4-escalar");
  if(escalar != Zero<T>())
  {
    inv_escalar /= escalar;
  }
  //-------------------------------------------------------------->>>
#else
  //--------------------------------------------------------------<<<
  // Sin chequeo
  inv_escalar /= escalar;
  //-------------------------------------------------------------->>>
#endif

  return (Matrix44<T>(m00 * inv_escalar, m01 * inv_escalar, m02 * inv_escalar, m03 * inv_escalar,
                      m10 * inv_escalar, m11 * inv_escalar, m12 * inv_escalar, m13 * inv_escalar,  
                      m20 * inv_escalar, m21 * inv_escalar, m22 * inv_escalar, m23 * inv_escalar, 
                      m30 * inv_escalar, m31 * inv_escalar, m32 * inv_escalar, m33 * inv_escalar));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: suma
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::operator += (const Matrix44<T> &otra)
{
  m00 += otra.m00; m01 += otra.m01; m02 += otra.m02;  m03 += otra.m03;  
  m10 += otra.m10; m11 += otra.m11; m12 += otra.m12;  m13 += otra.m13;
  m20 += otra.m20; m21 += otra.m21; m22 += otra.m22;  m23 += otra.m23;
  m30 += otra.m30; m31 += otra.m31; m32 += otra.m32;  m33 += otra.m33;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: resta
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::operator -= (const Matrix44<T>& otra)
{
  m00 -= otra.m00; m01 -= otra.m01; m02 -= otra.m02;  m03 -= otra.m03;  
  m10 -= otra.m10; m11 -= otra.m11; m12 -= otra.m12;  m13 -= otra.m13;
  m20 -= otra.m20; m21 -= otra.m21; m22 -= otra.m22;  m23 -= otra.m23;
  m30 -= otra.m30; m31 -= otra.m31; m32 -= otra.m32;  m33 -= otra.m33;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: producto
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::operator *= (const Matrix44<T>& otra)
{
  return (*this) = (*this) * otra;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: producto matriz x escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::operator *= (const T escalar)
{
  m00 *= escalar; m01 *= escalar; m02 *= escalar; m03 *= escalar; 
  m10 *= escalar; m11 *= escalar; m12 *= escalar; m13 *= escalar; 
  m20 *= escalar; m21 *= escalar; m22 *= escalar; m23 *= escalar;
  m30 *= escalar; m31 *= escalar; m32 *= escalar; m33 *= escalar;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: cociente matriz / escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::operator /= (const T escalar)
{
  T inv_escalar = One<T>();

#ifdef CHECK_MATH_SINGULARITY
  //--------------------------------------------------------------<<<
  // Con chequeo de división por cero
  ASSERTMSG(escalar != Zero<T>(), "División por cero en cociente matriz-escalar");
  if(escalar != Zero<T>())
  {
    inv_escalar /= escalar;
  }
  //-------------------------------------------------------------->>>
#else
  //--------------------------------------------------------------<<<
  // Sin chequeo
  inv_escalar /= escalar;
  //-------------------------------------------------------------->>>
#endif

  m00 *= inv_escalar;  m01 *= inv_escalar;  m02 *= inv_escalar;  m03 *= inv_escalar;
  m10 *= inv_escalar;  m11 *= inv_escalar;  m12 *= inv_escalar;  m13 *= inv_escalar;
  m20 *= inv_escalar;  m21 *= inv_escalar;  m22 *= inv_escalar;  m23 *= inv_escalar;
  m30 *= inv_escalar;  m31 *= inv_escalar;  m32 *= inv_escalar;  m33 *= inv_escalar;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Calcula el determinante de la matriz. Para una matriz 4x4 se define como un escalar igual a:
/// \verbatim
///     | m00 m01 m02 m03 |
///     | m10 m11 m12 m13 |          | m11 m12 m13 |         | m10 m12 m13 |         | m10 m11 m13 |         | m10 m11 m12 |
///     | m20 m21 m22 m23 |  = m00 * | m21 m22 m23 | - m01 * | m20 m22 m23 | + m02 * | m20 m21 m23 | - m03 * | m20 m21 m22 |
///     | m30 m31 m32 m33 |          | m31 m32 m33 |         | m30 m32 m33 |         | m30 m31 m33 |         | m30 m31 m32 |
///
/// Coste: [40 productos y 23 sumas]
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix44<T>::Determinant () const
{
  return m00 * Matrix33<T>(m11, m12, m13, m21, m22, m23, m31, m32, m33).Determinant() -
         m01 * Matrix33<T>(m10, m12, m13, m20, m22, m23, m30, m32, m33).Determinant() +
         m02 * Matrix33<T>(m10, m11, m13, m20, m21, m23, m30, m31, m33).Determinant() -
         m03 * Matrix33<T>(m10, m11, m12, m20, m21, m22, m30, m31, m32).Determinant();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Invierte la matriz actual
/// Calcula la inversa formal de la matriz actual, que cumple que M*M(inversa) = M(inversa)*M = I
/// \verbatim
///                  traspuesta matriz de cofactores de M
///   M(inversa) = ----------------------------------------
///                        Determinante de M
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::Invert ()
{
  return ((*this) = GetInverted());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión que devuelve una copia
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> Matrix44<T>::GetInverted () const
{
  T determinante = Determinant();
  T inv_det = One<T>();

#ifdef CHECK_MATH_SINGULARITY
  //-----------------------------------------------------------------------<<<
  // Con chequeo de división por cero
  ASSERTMSG(determinante != Zero<T>(), "Determinante nulo en cálculo de matriz inversa");
  if(determinante != Zero<T>())
  {
    inv_det /= determinante;
  }
  //----------------------------------------------------------------------->>>
#else
  //-----------------------------------------------------------------------<<<  
  // Sin chequeo
  inv_det /= determinante;
  //----------------------------------------------------------------------->>>
#endif

  Matrix44<T> auxMat;

  auxMat.m00 = Matrix33<T>(m11, m12, m13, m21, m22, m23, m31, m32, m33).Determinant() * inv_det;
  auxMat.m01 =-Matrix33<T>(m01, m02, m03, m21, m22, m23, m31, m32, m33).Determinant() * inv_det;
  auxMat.m02 = Matrix33<T>(m01, m02, m03, m11, m12, m13, m31, m32, m33).Determinant() * inv_det;
  auxMat.m03 =-Matrix33<T>(m01, m02, m03, m11, m12, m13, m21, m22, m23).Determinant() * inv_det;

  auxMat.m10 =-Matrix33<T>(m10, m12, m13, m20, m22, m23, m30, m32, m33).Determinant() * inv_det;
  auxMat.m11 = Matrix33<T>(m00, m02, m03, m20, m22, m23, m30, m32, m33).Determinant() * inv_det;
  auxMat.m12 =-Matrix33<T>(m00, m02, m03, m10, m12, m13, m30, m32, m33).Determinant() * inv_det;
  auxMat.m13 = Matrix33<T>(m00, m02, m03, m10, m12, m13, m20, m22, m23).Determinant() * inv_det;

  auxMat.m20 = Matrix33<T>(m10, m11, m13, m20, m21, m23, m30, m31, m33).Determinant() * inv_det;
  auxMat.m21 =-Matrix33<T>(m00, m01, m03, m20, m21, m23, m30, m31, m33).Determinant() * inv_det;
  auxMat.m22 = Matrix33<T>(m00, m01, m03, m10, m11, m13, m30, m31, m33).Determinant() * inv_det;
  auxMat.m23 =-Matrix33<T>(m00, m01, m03, m10, m11, m13, m20, m21, m23).Determinant() * inv_det;

  auxMat.m30 =-Matrix33<T>(m10, m11, m12, m20, m21, m22, m30, m31, m32).Determinant() * inv_det;
  auxMat.m31 = Matrix33<T>(m00, m01, m02, m20, m21, m22, m30, m31, m32).Determinant() * inv_det;
  auxMat.m32 =-Matrix33<T>(m00, m01, m02, m10, m11, m12, m30, m31, m32).Determinant() * inv_det;
  auxMat.m33 = Matrix33<T>(m00, m01, m02, m10, m11, m12, m20, m21, m22).Determinant() * inv_det;

  return auxMat;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Calcula la inversa de la matriz si esta cumple los siguientes requisitos:
///   - Es una transformación afín compuesta de TRASLACION + ROTACION + ESCALADO
///   - Si la matriz es resultado de multiplicar M(traslacion) * M(Rotacion) * M(escalado), cada
///     matriz de estas se define:
/// \verbatim
///          | 1 0 0 Tx |   | x1  x2  x3  0 |   | ex 0  0  0 |
///          | 0 1 0 Ty | * | y1  y2  y3  0 | * | 0  ey 0  0 |
///          | 0 0 1 Tz |   | z1  z2  z3  0 |   | 0  0  ez 0 |
///          | 0 0 0  1 |   | 0   0   0   1 |   | 0  0  0  1 |
/// \endverbatim
/// Donde la matriz de rotación respresenta a una base 'ortonormal' en el espacio 3D
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::InvertTRS ()
{
  return ((*this) = GetInvertedTRS());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión que devuelve una copia
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> Matrix44<T>::GetInvertedTRS ()const
{
  T div_x = One<T>();
  T div_y = One<T>();
  T div_z = One<T>();

  T esc_x = GetScaleX();
  T esc_y = GetScaleY();
  T esc_z = GetScaleZ();

#ifdef CHECK_MATH_SINGULARITY
  //-------------------------------------------------------------<<<
  // Con chequeo de división por cero
  ASSERTMSG(esc_x != Zero<T>() &&
            esc_y != Zero<T>() &&
            esc_z != Zero<T>(),
            "División por cero en cálculo de matriz inversa TRS");
  if(esc_x != Zero<T>())
  {
    div_x /= (esc_x * esc_x);
  }
  if(esc_y != Zero<T>())
  {
    div_y /= (esc_y * esc_y);
  }
  if(esc_z != Zero<T>())
  {
    div_z /= (esc_z * esc_z);
  }
  //------------------------------------------------------------->>>
#else
  //-------------------------------------------------------------<<<
  // Sin chequeo
  div_x /= (esc_x * esc_x);
  div_y /= (esc_y * esc_y);
  div_z /= (esc_z * esc_z);
  //------------------------------------------------------------->>>
#endif 

  Matrix44<T> auxMat;

  auxMat.m00 = m00 / div_x;
  auxMat.m01 = m10 / div_x;
  auxMat.m02 = m20 / div_x;

  auxMat.m10 = m01 / div_y;
  auxMat.m11 = m11 / div_y;
  auxMat.m12 = m21 / div_y;

  auxMat.m20 = m02 / div_z;
  auxMat.m21 = m12 / div_z;
  auxMat.m22 = m22 / div_z;

  auxMat.m03 = -m03*auxMat.m00 - m13*auxMat.m01 - m23*auxMat.m02;
  auxMat.m13 = -m03*auxMat.m10 - m13*auxMat.m11 - m23*auxMat.m12;
  auxMat.m23 = -m03*auxMat.m20 - m13*auxMat.m21 - m23*auxMat.m22;

  auxMat.m30 = Zero<T>();
  auxMat.m31 = Zero<T>();
  auxMat.m32 = Zero<T>();
  auxMat.m33 = One<T>();
  
  return auxMat;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Calcula la inversa de la matriz si esta cumple los siguientes requisitos:
///   - Es una transformación afín compuesta de TRASLACION + ROTACION
///   - Si la matriz es resultado de multiplicar M(traslacion) * M(Rotacion), cada
///     matriz de estas se define:
/// \verbatim
///          | 1 0 0 Tx |   | x1  x2  x3  0 |
///          | 0 1 0 Ty | * | y1  y2  y3  0 |
///          | 0 0 1 Tz |   | z1  z2  z3  0 |
///          | 0 0 0  1 |   | 0   0   0   1 |
/// \endverbatim
/// Donde la matriz de rotación respresenta a una base 'ortonormal' en el espacio 3D
/// (Versión que modifica la matriz)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::InvertTR ()
{
  return ((*this) = GetInvertedTR());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión const que devuelve una copia
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> Matrix44<T>::GetInvertedTR () const
{
  Matrix44<T> auxMat;

  auxMat.m00 = m00;
  auxMat.m01 = m10;
  auxMat.m02 = m20;

  auxMat.m10 = m01;
  auxMat.m11 = m11;
  auxMat.m12 = m21;

  auxMat.m20 = m02;
  auxMat.m21 = m12;
  auxMat.m22 = m22;

  auxMat.m03 = -m03*auxMat.m00 - m13*auxMat.m01 - m23*auxMat.m02;
  auxMat.m13 = -m03*auxMat.m10 - m13*auxMat.m11 - m23*auxMat.m12;
  auxMat.m23 = -m03*auxMat.m20 - m13*auxMat.m21 - m23*auxMat.m22;

  auxMat.m30 = Zero<T>();
  auxMat.m31 = Zero<T>();
  auxMat.m32 = Zero<T>();
  auxMat.m33 = One<T>();
  
  return auxMat;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Trasposición de la matriz (cambio de filas por columnas)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::Transpose ()
{
  T valor;
  valor = m01;  m01 = m10;  m10 = valor;
  valor = m02;  m02 = m20;  m20 = valor;
  valor = m03;  m03 = m30;  m30 = valor;
  valor = m12;  m12 = m21;  m21 = valor;
  valor = m13;  m13 = m31;  m31 = valor;
  valor = m23;  m23 = m32;  m32 = valor;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve una matriz traspuesta de la actual
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> Matrix44<T>::GetTransposed () const
{
  return Matrix44<T>(m00, m10, m20, m30,
                     m01, m11, m21, m31,
                     m02, m12, m22, m32,
                     m03, m13, m23, m33);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Se aplica una rotación a la matriz de 'angleX' radianes, alrededor del ejeX
/// del sistema de referencia del padre
/// (versión const que devuelve una matriz copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
Matrix44<T> Matrix44<T>::GetRotedByAngleX (const T angleX) const
{
  return Matrix44<T>(*this).RotByAngleX(angleX);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
Matrix44<T>& Matrix44<T>::RotByAngleX (const T angleX)
{
	Vector2<T> v;

	// eje X
	v.x = m10;
	v.y = m20;
	v.Rotate(angleX);
	m10 = v.x;
	m20 = v.y;

	// eje Y
	v.x = m11;
	v.y = m21;
	v.Rotate(angleX);
	m11 = v.x;
	m21 = v.y;

	// eje Z
	v.x = m12;
	v.y = m22;
	v.Rotate(angleX);
	m12 = v.x;
	m22 = v.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Se aplica una rotación a la matriz de 'angleY' radianes, alrededor del ejeY
/// del sistema de referencia del padre
/// (versión const que devuelve una matriz copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
Matrix44<T> Matrix44<T>::GetRotedByAngleY (const T angleY) const
{
  return Matrix44<T>(*this).RotByAngleY(angleY);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::RotByAngleY (const T angleY)
{
	Vector2<T> v;

	// eje X
	v.x = m20;
	v.y = m00;
	v.Rotate(angleY);
	m20 = v.x;
	m00 = v.y;

	// eje Y
	v.x = m21;
	v.y = m01;
	v.Rotate(angleY);
	m21 = v.x;
	m01 = v.y;

	// eje Z
	v.x = m22;
	v.y = m02;
	v.Rotate(angleY);
	m22 = v.x;
	m02 = v.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Se aplica una rotación a la matriz de 'angleZ' radianes, alrededor del ejeZ
/// del sistema de referencia del padre
/// (versión const que devuelve una matriz copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
Matrix44<T> Matrix44<T>::GetRotedByAngleZ (const T angleZ) const
{
  return Matrix44<T>(*this).RotByAngleY(angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::RotByAngleZ (const T angleZ)
{
	Vector2<T> v;

	// eje X
	v.x = m00;
	v.y = m10;
	v.Rotate(angleZ);
	m00 = v.x;
	m10 = v.y;

	// eje Y
	v.x = m01;
	v.y = m11;
	v.Rotate(angleZ);
	m01 = v.x;
	m11 = v.y;

	// eje Z
	v.x = m02;
	v.y = m12;
	v.Rotate(angleZ);
	m02 = v.x;
	m12 = v.y;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Se aplica una rotación a la matriz de 'angleX' radianes, alrededor del ejeX y
/// luego de 'AngleZ' radianes alrededor del eje Z, del sistema de referencia del padre
/// (versión const que devuelve una matriz copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
Matrix44<T> Matrix44<T>::GetRotedByAnglesXZ  (const T angleX, const T angleZ) const
{
  return Matrix44<T>(*this).RotByAnglesXZ(angleX, angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::RotByAnglesXZ (const T angleX, const T angleZ)
{
	Vector2<T> v;

	////////////
	// RotX
	////////////

	// eje X
	v.x = m10;
	v.y = m20;
	v.Rotate(angleX);
	m10 = v.x;
	m20 = v.y;

	// eje Y
	v.x = m11;
	v.y = m21;
	v.Rotate(angleX);
	m11 = v.x;
	m21 = v.y;

	// eje Z
	v.x = m12;
	v.y = m22;
	v.Rotate(angleX);
	m12 = v.x;
	m22 = v.y;

	////////////
	// RotZ
	////////////

	// eje X
	v.x = m00;
	v.y = m10;
	v.Rotate(angleZ);
	m00 = v.x;
	m10 = v.y;

	// eje Y
	v.x = m01;
	v.y = m11;
	v.Rotate(angleZ);
	m01 = v.x;
	m11 = v.y;

	// eje Z
	v.x = m02;
	v.y = m12;
	v.Rotate(angleZ);
	m02 = v.x;
	m12 = v.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Se aplica una rotación a la matriz de 'angleY' radianes, alrededor del ejeY,
/// luego de 'angleX' radianes alrededor del ejeX y luego de 'AngleZ'
/// alrededor del eje Z, del sistema de referencia del padre
/// (versión const que devuelve una matriz copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
Matrix44<T> Matrix44<T>::GetRotedByAnglesYXZ (const T angleY, const T angleX, const T angleZ) const
{
  return Matrix44<T>(*this).RotByAnglesYXZ(angleY, angleX, angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::RotByAnglesYXZ (const T angleY, const T angleX, const T angleZ)
{
	Vector2<T> v;

	////////////
	// RotY
	////////////

	// eje X
	v.x = m20;
	v.y = m00;
	v.Rotate(angleY);
	m20 = v.x;
	m00 = v.y;

	// eje Y
	v.x = m21;
	v.y = m01;
	v.Rotate(angleY);
	m21 = v.x;
	m01 = v.y;

	// eje Z
	v.x = m22;
	v.y = m02;
	v.Rotate(angleY);
	m22 = v.x;
	m02 = v.y;

	////////////
	// RotX
	////////////

	// eje X
	v.x = m10;
	v.y = m20;
	v.Rotate(angleX);
	m10 = v.x;
	m20 = v.y;

	// eje Y
	v.x = m11;
	v.y = m21;
	v.Rotate(angleX);
	m11 = v.x;
	m21 = v.y;

	// eje Z
	v.x = m12;
	v.y = m22;
	v.Rotate(angleX);
	m12 = v.x;
	m22 = v.y;

	////////////
	// RotZ
	////////////

	// eje X
	v.x = m00;
	v.y = m10;
	v.Rotate(angleZ);
	m00 = v.x;
	m10 = v.y;

	// eje Y
	v.x = m01;
	v.y = m11;
	v.Rotate(angleZ);
	m01 = v.x;
	m11 = v.y;

	// eje Z
	v.x = m02;
	v.y = m12;
	v.Rotate(angleZ);
	m02 = v.x;
	m12 = v.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Cambia la posición de la base (traslacion), considerada la matriz como
/// transformación afin de traslacion + rotación + escalado
/// (versión const que devuelve una copia)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
template<typename T>
Matrix44<T> Matrix44<T>::GetTranslated (const Vector3<T>& pos) const
{
  return Matrix44<T>(*this).Translate(pos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::Translate (const Vector3<T>& pos)
{
	SetPos(pos);

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Escala la matriz (solo escala los vectores de la base y no la posición)
/// (versión const que devuelve una copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T> Matrix44<T>::GetScaled (const T escala_x, const T escala_y, const T escala_z) const
{
  return Matrix44<T>(*this).Scale(escala_x, escala_y, escala_z);  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::Scale (const T escala_x, const T escala_y, const T escala_z)
{
  m00 *= escala_x; m10 *= escala_x; m20 *= escala_x;
	m01 *= escala_y; m11 *= escala_y; m21 *= escala_y;
	m02 *= escala_z; m12 *= escala_z; m22 *= escala_z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Alinea la matriz con dos vectores.
///  - El 'Y' se considera el eje principal.
///  - A partir de este 'Y' y el 'X' se obtiene, por producto vectorial el 'Z'.
///  - A partir de este 'Z' y el 'Y' (eje principal) se reajusta el 'X' para
///    producir una base de vectores ortogonales.
///  - Se normaliza para obtener una base ortonormal.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::AlignYX (const Vector3<T>& vY, const Vector3<T>& vX)
{
	Vector3<T> y = vY;
	Vector3<T> z = vX ^ vY;
	Vector3<T> x = vY ^ z;

	SetFromBasis(x, y , z);

	ResetScale();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Alinea la matriz con dos vectores.
///  - El 'Y' se considera el eje principal.
///  - A partir de este 'Y' y el 'Z' se obtiene, por producto vectorial el 'X'.
///  - A partir de este 'X' y el 'Y' (eje principal) se reajusta el 'Z' para
///    producir una base de vectores ortogonales.
///  - Se normaliza para obtener una base ortonormal.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::AlignYZ (const Vector3<T>& vY, const Vector3<T>& vZ)
{
	Vector3<T> y = vY;
	Vector3<T> x = vY ^ vZ;
	Vector3<T> z = x ^ vY;

	SetFromBasis(x, y , z);

	ResetScale();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Alinea la matriz con dos vectores.
///  - El 'X' se considera el eje principal.
///  - A partir de este 'X' y el 'Z' se obtiene, por producto vectorial el 'Y'.
///  - A partir de este 'Y' y el 'X' (eje principal) se reajusta el 'Z' para
///    producir una base de vectores ortogonales.
///  - Se normaliza para obtener una base ortonormal.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix44<T>& Matrix44<T>::AlignXZ (const Vector3<T>& vX, const Vector3<T>& vZ)
{
	Vector3<T> x = vX;
	Vector3<T> y = vZ ^ vX;
	Vector3<T> z = vX ^ y;

	SetFromBasis(x, y , z);

	ResetScale();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve true si la parte de la matriz 3x3 que representa a los vectores 
/// de la base, considerando a la matriz como un sistema de referencia,
/// representa una base en el espacio 3D con sus ejes ortogonales entre sí.
/// (Utiliza un epsilon como margen de tolerancia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
bool Matrix44<T>::IsOrthogonalEpsilon  () const
{
  return(mathUtils::ZeroEpsilon<T>(GetVectorBasis(0) * GetVectorBasis(1)) &&
         mathUtils::ZeroEpsilon<T>(GetVectorBasis(0) * GetVectorBasis(2)) &&
         mathUtils::ZeroEpsilon<T>(GetVectorBasis(1) * GetVectorBasis(2)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve true si la parte de la matriz 3x3 que representa a los vectores 
/// de la base, considerando a la matriz como un sistema de referencia,
/// representa una base en el espacio 3D con sus ejes ortogonales entre sí y
/// con módulo 1
/// (Utiliza un epsilon como margen de tolerancia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
bool Matrix44<T>::IsOrthonormalEpsilon () const
{
  return(IsOrthogonalEpsilon() &&
         mathUtils::EqualEpsilon<T>(GetVectorBasis(0).SquaredLength(), One<T>()) && 
         mathUtils::EqualEpsilon<T>(GetVectorBasis(1).SquaredLength(), One<T>()) && 
         mathUtils::EqualEpsilon<T>(GetVectorBasis(2).SquaredLength(), One<T>()));
}

template<typename T>
inline Matrix44<T>::Matrix44(const D3DXMATRIX &otra)
: m00(otra._11), m10(otra._12), m20(otra._13), m30(otra._14)
, m01(otra._21), m11(otra._22), m21(otra._23), m31(otra._24)
, m02(otra._31), m12(otra._32), m22(otra._33), m32(otra._34)
, m03(otra._41), m13(otra._42), m23(otra._43), m33(otra._44)
{
}

template<typename T>
D3DXMATRIX Matrix44<T>::GetD3DXMatrix() const
{
  return D3DXMATRIX(	m00, m10, m20, m30
								, m01, m11, m21, m31
								, m02, m12, m22, m32
								, m03, m13, m23, m33);
}