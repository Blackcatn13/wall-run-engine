//-----------------------------------------------------------------------
// Matrix34 inline
/// Definiciones de funciones inline de la clase 'Matrix34'
/// Este fichero es realmente parte de la cabecera 'Matrix34.h' --> NO PONER INCLUDES
//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 1
/// Construcción sin inicialización de parámetros  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>::Matrix34 ()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 2
/// Constructor de copia
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>::Matrix34 (const Matrix34<T>& otra) :
  m00(otra.m00), m10(otra.m10), m20(otra.m20),
  m01(otra.m01), m11(otra.m11), m21(otra.m21),
  m02(otra.m02), m12(otra.m12), m22(otra.m22),
  m03(otra.m03), m13(otra.m13), m23(otra.m23)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 3
/// A partir de una matriz 3x3. Se rellena la sub-matriz 3x3 que
/// normalmente contiene la rotación y/o el escalado. La columna
/// restante se inicia a cero
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>::Matrix34 (const Matrix33<T>& sub) :
  m00(sub.m00),   m10(sub.m10),   m20(sub.m20),
  m01(sub.m01),   m11(sub.m11),   m21(sub.m21),
  m02(sub.m02),   m12(sub.m12),   m22(sub.m22),
  m03(Zero<T>()), m13(Zero<T>()), m23(Zero<T>())
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 4
/// A partir de una matriz 3x3 que rellena la submatriz 3x3 de rotacion/escalado
/// y un vector 3D para la columna de posición
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>::Matrix34 (const Matrix33<T>& sub, const Vector3<T>& pos) :
  m00(sub.m00),   m10(sub.m10),   m20(sub.m20),
  m01(sub.m01),   m11(sub.m11),   m21(sub.m21),
  m02(sub.m02),   m12(sub.m12),   m22(sub.m22),
  m03(pos.x),     m13(pos.y),     m23(pos.z)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 5
/// Inserción de todos los datos de la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>::Matrix34 (const T _m00, const T _m01, const T _m02, const T _m03,
                              const T _m10, const T _m11, const T _m12, const T _m13,
                              const T _m20, const T _m21, const T _m22, const T _m23) :
  m00(_m00), m10(_m10), m20(_m20),
  m01(_m01), m11(_m11), m21(_m21),
  m02(_m02), m12(_m12), m22(_m22),
  m03(_m03), m13(_m13), m23(_m23)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 6
/// (Ver comentario en SetFromBasis)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>::Matrix34 (const Vector3<T>& ejeX,
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
inline Matrix34<T>::Matrix34 (const T angleY, const T angleX, const T angleZ)
{
  SetFromAnglesYXZ(angleY, angleX, angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Elimina la traslación de la matriz, considerándola como una
/// transformación afín de TRASLACION + ROTACION + ESCALADO definida
/// con una matriz 4x4 cuya última fila fuera [0 0 0 1]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::ResetTranslation ()
{
  m03 = Zero<T>();
  m13 = Zero<T>();
  m23 = Zero<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Elimina la rotación de la matriz, considerándola como una transformación
/// afín de TRASLACION + ROTACION + ESCALADO definida con una matriz 4x4 cuya
/// última fila fuera [0 0 0 1].
/// El escalado se mantiene invariable.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::ResetRotation ()
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
/// de TRASLACION + ROTACION + ESCALADO definida con una matriz 4x4 cuya
/// última fila fuera [0 0 0 1].
/// Equivale a normalizar la matriz 3x3 que representa a la rotación
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::ResetScale ()
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
/// Establece la submatriz 3x3
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetSubMatrix33 (const Matrix33<T>& mat_sub)
{
  m00 = mat_sub.m00;   m01 = mat_sub.m01;   m02 = mat_sub.m02;
  m10 = mat_sub.m10;   m11 = mat_sub.m11;   m12 = mat_sub.m12;
  m20 = mat_sub.m20;   m21 = mat_sub.m21;   m22 = mat_sub.m22;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Se elimina la rotación y el escalado. La submatriz de rotación 3x3 se
/// transforma en la matriz identidad
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::ResetRotationScale ()
{
  m00 = One<T>();   m01 = Zero<T>();  m02 = Zero<T>();
  m10 = Zero<T>();  m11 = One<T>();   m12 = Zero<T>();
  m20 = Zero<T>();  m21 = Zero<T>();  m22 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Cambia los datos de la matriz por los de una 'matriz identidad'
/// NOTA: No es realmente una matriz identidad ya que solo pueden serlo
///       matrices cuadradas. En este caso, lo que se transforma en una
///       matriz identidad es la parte 3x3 que actúa normalmente como
///       rotación.
/// \verbatim
///       | 1 0 0 0 |
///       | 0 1 0 0 |
///       | 0 0 1 0 |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetIdentity ()
{
  m00 = One<T>();   m10 = Zero<T>();  m20 = Zero<T>();
  m01 = Zero<T>();  m11 = One<T>();   m21 = Zero<T>();
  m02 = Zero<T>();  m12 = Zero<T>();  m22 = One<T>();
  m03 = Zero<T>();  m13 = Zero<T>();  m23 = Zero<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pone a cero todas las componentes de la matriz
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetZero ()
{
  for(int i=0; i<12; i++)
  {
    ((T*)this)[i] = Zero<T>();
  }

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece los datos de la matriz a partir de 3 vectores 3D que forman
/// una base y una pocición 3D.
/// Se construye una tranformación de ROTACION + TRASLACION o una transformación
/// típica de sistema de coordenadas actual a sistema de coordenadas del padre (en cuyas
/// coordenadas deben darse los vectores parámetro y la posición)
/// \verbatim
///                          | x1 x2 x3 Px |
///                          | y1 y2 y3 Py |
///                          | z1 z2 z3 Pz |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromBasis (const Vector3<T>& ejeX,
                                               const Vector3<T>& ejeY,
                                               const Vector3<T>& ejeZ,
                                               const Vector3<T>& pos)
{
	m00 = ejeX.x;     m01 = ejeY.x;     m02 = ejeZ.x;     m03 = pos.x;
	m10 = ejeX.y;     m11 = ejeY.y;     m12 = ejeZ.y;     m13 = pos.y;
	m20 = ejeX.z;     m21 = ejeY.z;     m22 = ejeZ.z;     m23 = pos.z;

  return (*this);
}

template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromBasis (const Vector3<T>& ejeX,
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
/// del sistema de referencia padre, en la submatriz 3x3 que forman las 3 primeras columnas
/// (Ver comentario en Matrix33::SetFromAngleX)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromAngleX (const T angleX)
{
  T sina;
  T cosa;
  mathUtils::SinCos(angleX, sina, cosa);

  m00 = One<T>();   m01 = Zero<T>();  m02 = Zero<T>();  m03 = Zero<T>();
  m10 = Zero<T>();  m11 = cosa;       m12 = -sina;      m13 = Zero<T>();
  m20 = Zero<T>();  m21 = sina;       m22 = cosa;       m23 = Zero<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleY' radianes alrededor del ejeY (normalmente inclinación)
/// que también es una matriz de sistema de referencia, con todos sus ejes rotados sobre el ejeY
/// del sistema de referencia padre, en la submatriz 3x3 que forman las 3 primeras columnas
/// (Ver comentario en Matrix33::SetFromAngleY)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromAngleY (const T angleY)
{
  T sina;
  T cosa;
  mathUtils::SinCos(angleY, sina, cosa);

  m00 = cosa;       m01 = Zero<T>();  m02 = sina;       m03 = Zero<T>();
  m10 = Zero<T>();  m11 = One<T>();   m12 = Zero<T>();  m13 = Zero<T>();
  m20 = -sina;      m21 = Zero<T>();  m22 = cosa;       m23 = Zero<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleZ' radianes alrededor del ejeZ (normalmente inclinación)
/// que también es una matriz de sistema de referencia, con todos sus ejes rotados sobre el ejeZ
/// del sistema de referencia padre, en la submatriz 3x3 que forman las 3 primeras columnas
/// (Ver comentario en Matrix33::SetFromAngleZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromAngleZ (const T angleZ)
{
  T sina;
  T cosa;
  mathUtils::SinCos(angleZ, sina, cosa);

  m00 = cosa;       m01 = -sina;      m02 = Zero<T>();  m03 = Zero<T>();
  m10 = sina;       m11 = cosa;       m12 = Zero<T>();  m13 = Zero<T>();
  m20 = Zero<T>();  m21 = Zero<T>();  m22 = One<T>();   m23 = Zero<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleX' radianes alrededor del ejeX +
/// 'angleZ' alrededor del eje z (primero inclinación y luego giro) en la submatriz
/// 3x3 que forman las 3 primeras columnas.
/// (Ver comentario en Matrix33::SetFromAngleXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromAnglesXZ  (const T angleX, const T angleZ)
{
  T sinx, cosx, sinz, cosz;
  mathUtils::SinCos(angleX, sinx, cosx);
  mathUtils::SinCos(angleZ, sinz, cosz);

  m00 = cosz;       m01 = -sinz * cosx;  m02 = sinz * sinx;   m03 = Zero<T>();
  m10 = sinz;       m11 = cosz * cosx;   m12 = -cosz * sinx;  m13 = Zero<T>();
  m20 = Zero<T>();  m21 = sinx;          m22 = cosx;          m23 = Zero<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleY' radianes alrededor del ejeY +
/// 'angleX' radianes alrededor del ejeX + 'angleZ' alrededor del eje z
/// (primero alabeo, luego inclinación y luego giro) en la submatriz 3x3
/// que forman las 3 primeras columnas.
/// También es una matriz de sistema de referencia, con todos sus ejes
/// rotados de la misma forma en el sistema de referencia padre.
/// (Ver comentario en Matrix33::SetFromAngleYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromAnglesYXZ (const T angleY, const T angleX, const T angleZ)
{
  T sx, cx, sy, cy, sz, cz;
  mathUtils::SinCos(angleX, sx, cx);
  mathUtils::SinCos(angleY, sy, cy);
  mathUtils::SinCos(angleZ, sz, cz);

  m00 = cz*cy - sz*sx*sy;  m01 = -sz*cx;  m02 = cz*sy + sz*sx*cy;  m03 = Zero<T>();
  m10 = sz*cy + cz*sx*sy;  m11 = cz*cx;   m12 = sz*sy - cz*sx*cy;  m13 = Zero<T>();
  m20 = -cx*sy;            m21 = sx;      m22 = cx*cy;             m23 = Zero<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Construye una matriz de traslación, actualizando los datos de la última columna
/// a partir de 3 magnitudes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromPos (const T posX, const T posY, const T posZ)
{
  m00 = One<T>();    m01 = Zero<T>();   m02 = Zero<T>();   m03 = posX;
  m10 = Zero<T>();   m11 = One<T>();    m12 = Zero<T>();   m13 = posY;
  m20 = Zero<T>();   m21 = Zero<T>();   m22 = One<T>();    m23 = posZ;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Construye una matriz de traslación, actualizando los datos de la última columna
/// a partir de un vector 3D
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromPos (const Vector3<T>& pos)
{
  m00 = One<T>();    m01 = Zero<T>();   m02 = Zero<T>();   m03 = pos.x;
  m10 = Zero<T>();   m11 = One<T>();    m12 = Zero<T>();   m13 = pos.y;
  m20 = Zero<T>();   m21 = Zero<T>();   m22 = One<T>();    m23 = pos.z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Construye una matriz de escala, actualizando la diagonal de la submatriz 3x3
/// en las 3 primeras columnas, a partir de una magnitud para cada eje de la base
/// que forma la transformación de la matriz
/// \verbatim
///   | escala_X     0         0     |
///   |    0      escala_Y     0     |
///   |    0         0      escala_Z |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromScale (const T escala_x, const T escala_y, const T escala_z)
{
  m00 = escala_x;    m01 = Zero<T>();   m02 = Zero<T>();   m03 = Zero<T>();
  m10 = Zero<T>();   m11 = escala_y;    m12 = Zero<T>();   m13 = Zero<T>();
  m20 = Zero<T>();   m21 = Zero<T>();   m22 = escala_z;    m23 = Zero<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la matriz de forma que el eje Y tendrá la dirección y el sentido del vector que va
/// desde 'vPos' hasta 'vTarget', utilizando el vector 'vUp' como referencia del sentido del 'arriba' del
/// mundo, para poder construir un sistema de referencia con 3 vectores típico de una matriz de cámara, siendo
/// el eje Y el vector de vista.
/// @Note1 El vector vUp debe estar normalizado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromLookAt (const Vector3<T>& vPos, const Vector3<T>& vTarget, const Vector3<T>& vUp)
{
	Matrix33<T> mat;
	mat.SetFromLookAt(vPos, vTarget, vUp);
	SetSubMatrix33(mat);
  return SetPos(vPos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la matriz de forma que el eje Y tendrá la dirección y el sentido del vector que va
/// desde 'vPos' hasta 'vTarget', utilizando el vector 'vUp' como referencia del sentido del 'arriba' del
/// mundo, para poder construir un sistema de referencia con 3 vectores típico de una matriz de cámara, siendo
/// el eje Y el vector de vista.
/// @Note1 El vector 'up' se considera (0,0,1) en este caso. Apuntando hacía arriba en las condiciones normales
///        del sistema de referencia adoptado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromLookAt (const Vector3<T>& vPos, const Vector3<T>& vTarget)
{

	Matrix33<T> mat;
	mat.SetFromLookAt(vPos, vTarget);
	SetSubMatrix33(mat);
  return SetPos(vPos);  
}
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la matriz a partir de un cuaternión, conteniéndo una rotación, y una posición
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromQuatPos (const Quatn<T>& quat, const Vector3<T>& vPos)
{
	Matrix33<T> mat;
	mat.SetRotByQuat(quat);
	SetSubMatrix33(mat);
  return SetPos(vPos);
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Igual que 'SetFromAngleX' pero solo modifica la submatriz 3x3 para cambiar la rotación
/// dejando la traslación y el escalado invariables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetRotByAngleX (const T angleX)
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
inline Matrix34<T>& Matrix34<T>::SetRotByAngleY (const T angleY)
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
inline Matrix34<T>& Matrix34<T>::SetRotByAngleZ (const T angleZ)
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
inline Matrix34<T>& Matrix34<T>::SetRotByAnglesXZ  (const T angleX, const T angleZ)
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
/// Igual que 'SetFromAngleYXZ' pero solo modifica la submatriz 3x3 para cambiar la rotación
/// dejando la traslación y el escalado invariables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetRotByAnglesYXZ (const T angleY, const T angleX, const T angleZ)
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
/// Modifica la submatriz 3x3 que contiene la rotación por medio de un cuaternión, dejando la traslación y el
/// escalado invariables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetRotByQuat(const Quatn<T>& quat)
{
	Matrix33<T> mat;
	mat.SetRotByQuat(quat);
  SetSubMatrix33(mat);

  return (*this);
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Cambia la cuarta columna que representa la traslación,
/// considerando a la matriz como una transformación afín
/// definida por una matriz 4x4 con la última fila a [0 0 0 1]
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetPos (const T posX, const T posY, const T posZ)
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
inline Matrix34<T>& Matrix34<T>::SetPos (const Vector3<T>& pos)
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
inline Matrix34<T>& Matrix34<T>::SetScale (const Matrix33<T>& mat_escala)
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
inline Matrix34<T>& Matrix34<T>::SetScale (const Matrix44<T>& mat_escala)
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
inline Matrix34<T>& Matrix34<T>::SetScale (const Vector3<T>& vect_escala)
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
inline Matrix34<T>& Matrix34<T>::SetScale (const T escala_x, const T escala_y, const T escala_z)
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
/// Devuelve el módulo del vector formado por los 3  elementos de una columna
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix34<T>::GetVectorBasisLength (int i) const
{
#ifdef ROW_MAJOR_MATRICES
	return Vector3<T>(((T*)this)[i],
		((T*)this)[4 + i],
		((T*)this)[8 + i] ).Length();
#else
	return (*((Vector3<T>*)this + i)).Length();
#endif

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve los 3 elementos de una columna, interpretados como un
/// vector 3D que forma la base que define la matriz en coordenadas del
/// sistema de referencia padre (versión const)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix34<T>::GetVectorBasis (int i) const
{
#ifdef ROW_MAJOR_MATRICES 
	return Vector3<T>(((T*)this)[i],
										((T*)this)[4  + i],
										((T*)this)[8  + i]);
#else
	return *((Vector3<T>*)this + i);
#endif
}

template<typename T>
inline Vector3<T> Matrix34<T>::GetPosBasis () const
{
	return GetVectorBasis(3);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve los 3 elementos de cada columna, interpretados
/// como vectores 3D y como una base con su posición en coordenadas del
/// sistema de referencia padre
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix34<T>::GetBasis (Vector3<T>& vBasisX,
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
/// Devuelve una referencia a la sub-matriz 3x3, que contiene la rotación
/// y el escalado de una transformación, sin efectuar copia
/// (versión const)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Matrix33<T> Matrix34<T>::GetSubMatrix33 () const
{
	return Matrix33<T>(	m00,       m01,       m02,
											m10,       m11,       m12,
											m20,       m21,       m22);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Rellena la matriz 3x3 del parámetro con la submatriz 3x3, que normalmente
/// tiene la información de escalado y/o rotación, de la matriz actual
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix34<T>::GetSubMatrix33 (Matrix33<T>& subMat) const
{
  subMat = GetSubMatrix33();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve una matriz 4x4 a partir de la matriz 3x4 actual, completando
/// la última fila con [0 0 0 1]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Matrix44<T> Matrix34<T>::GetMatrix44 () const
{
  return Matrix44<T>( m00,       m01,       m02,       m03,
                      m10,       m11,       m12,       m13,
                      m20,       m21,       m22,       m23,
                     Zero<T>(), Zero<T>(), Zero<T>(), One<T>());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Rellena la matriz del parámetro con una matriz 4x4 a partir de la
/// matriz 3x4 actual, completando la última fila con [0 0 0 1]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix34<T>::GetMatrix44 (Matrix44<T>& subMat) const
{
  subMat.m00 = m00;        subMat.m01 = m01;        subMat.m02 = m02;        subMat.m03 = m03; 
  subMat.m10 = m10;        subMat.m11 = m11;        subMat.m12 = m12;        subMat.m13 = m13; 
  subMat.m20 = m20;        subMat.m21 = m21;        subMat.m22 = m22;        subMat.m23 = m23; 
  subMat.m30 = Zero<T>();  subMat.m31 = Zero<T>();  subMat.m32 = Zero<T>();  subMat.m33 = One<T>(); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve una matriz 3x3 con la parte de rotación, considerando
/// a la matriz compuesta por una traslacion + rotación + escalado
/// (Versión que anula el escalado)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T> Matrix34<T>::Get33RotationNormalized () const
{
  return Matrix33<T>(GetVectorBasis(0).GetNormalized(),
                     GetVectorBasis(1).GetNormalized(),
                     GetVectorBasis(2).GetNormalized());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve una matriz 3x3 con la parte de rotación, considerando
/// a la matriz compuesta por una traslacion + rotación
/// (Versión que contiene el escalado)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T> Matrix34<T>::Get33RotationScaled () const
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
inline Matrix33<T> Matrix34<T>::Get33Scale () const
{
  return Matrix33<T>(GetVectorBasis(0).Length(),            Zero<T>()      ,          Zero<T>(),
                            Zero<T>()          , GetVectorBasis(1).Length(),          Zero<T>(),
                            Zero<T>()          ,            Zero<T>()      , GetVectorBasis(2).Length());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la parte de traslación (4 columna) Considerando a la matriz compuesta por
/// una traslación + rotación + escalado
/// (Versión que devuelve un vector de traslación)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix34<T>::GetTranslationVector () const
{
  return Vector3<T>(m03, m13, m23);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la parte de rotación de la matriz, considerándola como
/// una transformación de traslación + rotación + escalado
/// (Versión que elimina el escalado)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T> Matrix34<T>::Get44RotationNormalized () const
{
  return Matrix44<T>(GetVectorBasis(0).GetNormalized(),
                     GetVectorBasis(1).GetNormalized(),
                     GetVectorBasis(2).GetNormalized(),
                     Vector3<T>(Zero<T>(), Zero<T>(), Zero<T>()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la parte de rotación de la matriz, considerándola como
/// una transformación de traslación + rotación + escalado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T> Matrix34<T>::Get44RotationScaled () const
{
  return Matrix44<T>(  m00,       m01,       m02,     Zero<T>(),
                       m10,       m11,       m12,     Zero<T>(),
                       m20,       m21,       m22,     Zero<T>(),
                     Zero<T>(), Zero<T>(), Zero<T>(), One<T>());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la parte de escalado de la matriz, considerándola
/// como una transformación de traslación + rotación + escalado,
/// como una matriz 4x4
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T> Matrix34<T>::Get44Scale () const
{
  return Matrix44<T>(GetVectorBasis(0).Length(),            Zero<T>()      ,          Zero<T>()        ,  Zero<T>(),
                            Zero<T>()          , GetVectorBasis(1).Length(),          Zero<T>()        ,  Zero<T>(),
                            Zero<T>()          ,            Zero<T>()      , GetVectorBasis(2).Length(),  Zero<T>(),
                            Zero<T>()          ,            Zero<T>()      ,          Zero<T>()        ,  One<T>());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la parte de traslación de la matriz, considerándola 
/// como una transformación de traslación + rotación + escalado,
/// como una matriz 4x4
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix44<T> Matrix34<T>::Get44Translation () const
{
  return Matrix44<T>(One<T>(),  Zero<T>(), Zero<T>(), m03,
                     Zero<T>(), One<T>(),  Zero<T>(), m13,
                     Zero<T>(), Zero<T>(), One<T>(),  m23,
                     Zero<T>(), Zero<T>(), Zero<T>(), One<T>());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve las escalas de la matriz en los 3 ejes en forma de vector 3D
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix34<T>::GetScale () const
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
inline T Matrix34<T>::GetScaleX () const
{
  return GetVectorBasis(0).Length();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la escala de la matriz en el ejeY, considerando la base
/// del sistema de referencia que define.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix34<T>::GetScaleY () const
{
  return GetVectorBasis(1).Length();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la escala de la matriz en el ejeZ, considerando la base
/// del sistema de referencia que define.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix34<T>::GetScaleZ () const
{
  return GetVectorBasis(2).Length();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención de ángulos de euler (Ver comentario en Matrix33::GetAnglesYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Matrix34<T>::GetAnglesYXZ (T& angleY, T& angleX, T& angleZ) const
{
  GetSubMatrix33().GetAnglesYXZ(angleY, angleX, angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención del angulo de Euler que representa la rotación sobre el eje X
/// (Ver comentario en Matrix33::GetAnglesYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix34<T>::GetAngleX () const
{
  return GetSubMatrix33().GetAngleX();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención del angulo de Euler que representa la rotación sobre el eje Y
/// (Ver comentario en Matrix33::GetAnglesYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix34<T>::GetAngleY () const
{
  return GetSubMatrix33().GetAngleY();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención del angulo de Euler que representa la rotación sobre el eje Z
/// (Ver comentario en Matrix33::GetAnglesYXZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix34<T>::GetAngleZ () const
{
  return GetSubMatrix33().GetAngleZ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el ángulo de rotación sobre el eje Y, siguiendo el criterio de ángulos
/// y orden de concatenación explicado en 'Matrix33::SetPitchRollYaw'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix34<T>::GetRoll () const
{
  return GetSubMatrix33().GetRoll();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el ángulo de rotación sobre el eje X, siguiendo el criterio de ángulos
/// y orden de concatenación explicado en 'Matrix33::SetPitchRollYaw'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix34<T>::GetPitch () const
{
  return GetSubMatrix33().GetPitch();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el ángulo de rotación sobre el eje Z, siguiendo el criterio de ángulos
/// y orden de concatenación explicado en 'Matrix33::SetPitchRollYaw'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix34<T>::GetYaw () const
{
  return GetSubMatrix33().GetYaw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención de los ángulos de rotación sobre los ejes, aplicados con el criterio 'PitchRollYaw' explicado
/// en 'Matrix33::SetPitchRollYaw'.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix34<T>::GetPitchRollYaw () const
{
  return GetSubMatrix33().GetPitchRollYaw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Construye una matriz de rotación a partir de los ángulos de rotación sobre los ejes, aplicados con el criterio
/// 'PitchRollYaw' explicado en 'Matrix33::SetPitchRollYaw'. Esta función pone a cero la parte de posición.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetFromPitchRollYaw (const Vector3<T>& v3PitchRollYaw)
{
  T sx, cx, sy, cy, sz, cz;
  mathUtils::SinCos(v3PitchRollYaw.x, sx, cx);
  mathUtils::SinCos(v3PitchRollYaw.y, sy, cy);
  mathUtils::SinCos(v3PitchRollYaw.z, sz, cz);

  m00 = cy*cz;  m01 = cy*sz*cx - sy*sx;  m02 = -cy*sz*sx - sy*cx;  m03 = Zero<T>();
  m10 = -sz;    m11 = cz*cx;             m12 = -cz*sx;             m13 = Zero<T>();
  m20 = sy*cz;  m21 = sy*sz*cx + cy*sx;  m22 = -sy*sz*sx + cy*cx;  m23 = Zero<T>();

  return (*this);  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establecimiento de los ángulos de rotación sobre los ejes, aplicados con el criterio 'PitchRollYaw' explicado
/// en 'Matrix33::SetPitchRollYaw'. Esta función modifica solo la parte rotación.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix34<T>::SetPitchRollYaw (const Vector3<T>& v3PitchRollYaw)
{
	Matrix33<T> mat;
	mat.SetPitchRollYaw(v3PitchRollYaw);
	SetSubMatrix33(mat);
 
  return (*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: suma de matrices
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> Matrix34<T>::operator + (const Matrix34<T>& otra) const
{
  return (Matrix34<T>(m00 + otra.m00, m01 + otra.m01, m02 + otra.m02, m03 + otra.m03,
                      m10 + otra.m10, m11 + otra.m11, m12 + otra.m12, m13 + otra.m13,
                      m20 + otra.m20, m21 + otra.m21, m22 + otra.m22, m23 + otra.m23));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: resta de matrices
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> Matrix34<T>::operator - (const Matrix34<T>& otra) const
{
  return (Matrix34<T>(m00 - otra.m00, m01 - otra.m01, m02 - otra.m02, m03 - otra.m03,  
                      m10 - otra.m10, m11 - otra.m11, m12 - otra.m12, m13 - otra.m13,
                      m20 - otra.m20, m21 - otra.m21, m22 - otra.m22, m23 - otra.m23));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: producto de matrices [3 4] x [3 4]
/// Matemáticamente no está definido pero se puede considerar, dado el habitual uso de la
/// matriz de 4x4 como transformación afín de traslación + rotación + escalado, con
/// la última fila a [0 0 0 1], como el producto de dos matrices 4x4, dado:
/// \verbatim
///   | m00 m01 m02 m03 |      | n00 n01 n02 n03 |
///   | m10 m11 m12 m13 |  *   | n10 n11 n12 n13 |    m30 = m31 = m32 = 0      m33 = 1
///   | m20 m21 m22 m23 |      | n20 n21 n22 n23 |    n30 = n31 = n32 = 0      n33 = 1
///   | m30 m31 m32 m33 |      | n30 n31 n32 n33 |    
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> Matrix34<T>::operator * (const Matrix34<T>& otra) const
{
  return (Matrix34<T>(m00 * otra.m00 + m01 * otra.m10 + m02 * otra.m20,
                      m00 * otra.m01 + m01 * otra.m11 + m02 * otra.m21,
                      m00 * otra.m02 + m01 * otra.m12 + m02 * otra.m22,
                      m00 * otra.m03 + m01 * otra.m13 + m02 * otra.m23 + m03,

                      m10 * otra.m00 + m11 * otra.m10 + m12 * otra.m20,
                      m10 * otra.m01 + m11 * otra.m11 + m12 * otra.m21,
                      m10 * otra.m02 + m11 * otra.m12 + m12 * otra.m22,
                      m10 * otra.m03 + m11 * otra.m13 + m12 * otra.m23 + m13,

                      m20 * otra.m00 + m21 * otra.m10 + m22 * otra.m20,
                      m20 * otra.m01 + m21 * otra.m11 + m22 * otra.m21,
                      m20 * otra.m02 + m21 * otra.m12 + m22 * otra.m22,
                      m20 * otra.m03 + m21 * otra.m13 + m22 * otra.m23 + m23));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: producto matriz x escalar 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> Matrix34<T>::operator * (const T escalar) const
{
  return (Matrix34<T>(m00 * escalar, m01 * escalar, m02 * escalar, m03 * escalar,
                      m10 * escalar, m11 * escalar, m12 * escalar, m13 * escalar,
                      m20 * escalar, m21 * escalar, m22 * escalar, m23 * escalar));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético externo:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> operator * (const T escalar, const Matrix34<T>& mat)
{
  return (Matrix34<T>(mat.m00 * escalar, mat.m01 * escalar, mat.m02 * escalar, mat.m03 * escalar,  
                      mat.m10 * escalar, mat.m11 * escalar, mat.m12 * escalar, mat.m13 * escalar,  
                      mat.m20 * escalar, mat.m21 * escalar, mat.m22 * escalar, mat.m23 * escalar));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: cociente matriz / escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> Matrix34<T>::operator / (const T escalar) const
{
  T inv_escalar = One<T>();

#ifdef CHECK_MATH_SINGULARITY
  //------------------------------------------------------<<<
  // Con chequeo de división por cero
  ASSERTMSG(escalar != Zero<T>(), "División por cero en cociente Matriz3x4-escalar");
  if(escalar != Zero<T>())
  {
    inv_escalar /= escalar;
  }
  //------------------------------------------------------>>>
#else
  //------------------------------------------------------<<<
  // Sin chequeo
  inv_escalar /= escalar;
  //------------------------------------------------------>>>
#endif

  return (Matrix34<T>(m00 * inv_escalar, m01 * inv_escalar, m02 * inv_escalar, m03 * inv_escalar,
                      m10 * inv_escalar, m11 * inv_escalar, m12 * inv_escalar, m13 * inv_escalar,  
                      m20 * inv_escalar, m21 * inv_escalar, m22 * inv_escalar, m23 * inv_escalar));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: - Producto de matriz x vector 3D (considerado como punto).
///                      - Equivalente a producto de matrices [4 4]x[4 1].
///                      - Equivalente a una transformación de ROTACION + TRASLACION.
///                      - Se supone la ultima fila igual a [0 0 0 1].
/// \verbatim
///         | m00 m01 m02 m03 |     | x |     | x*m00 + y*m01 + z*m02 + m03 |
///         | m10 m11 m21 m13 |     | y |     | x*m10 + y*m11 + z*m12 + m13 |
///         | m20 m21 m22 m23 |  *  | z |  =  | x*m20 + y*m21 + z*m22 + m23 |
///         |-----------------|     |---|     |-----------------------------|
///         | (0) (0) (0) (1) |     |(1)|     |            (1)              |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Vector3<T> Matrix34<T>::operator * (const Vector3<T>& vector) const
{
  return Vector3<T>(vector.x * m00 + vector.y * m01 + vector.z * m02 + m03,
                    vector.x * m10 + vector.y * m11 + vector.z * m12 + m13,
                    vector.x * m20 + vector.y * m21 + vector.z * m22 + m23);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: Producto de matriz x vector 3D (usando solo la submatriz[0 0]-[2 2]).
///                      Equivalente a produto de matrices [3 3]x[3 1].
///                      Equivalente a una transformación de ROTACION.
/// \verbatim
///         | m00 m01 m02 |     | x |     | x*m00 + y*m01 + z*m02 |
///         | m10 m11 m12 |  ^  | y |  =  | x*m10 + y*m11 + z*m12 |
///         | m20 m21 m22 |     | z |     | x*m20 + y*m21 + z*m22 |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Vector3<T> Matrix34<T>::operator ^ (const Vector3<T>& vector) const
{
  return Vector3<T>(vector.x * m00 + vector.y * m01 + vector.z * m02,
                    vector.x * m10 + vector.y * m11 + vector.z * m12,
                    vector.x * m20 + vector.y * m21 + vector.z * m22);
}
  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: suma
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::operator += (const Matrix34<T>& otra)
{
  m00 += otra.m00; m01 += otra.m01; m02 += otra.m02;  m03 += otra.m03;  
  m10 += otra.m10; m11 += otra.m11; m12 += otra.m12;  m13 += otra.m13;
  m20 += otra.m20; m21 += otra.m21; m22 += otra.m22;  m23 += otra.m23;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: resta
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::operator -= (const Matrix34<T>& otra)
{
  m00 -= otra.m00; m01 -= otra.m01; m02 -= otra.m02;  m03 -= otra.m03;  
  m10 -= otra.m10; m11 -= otra.m11; m12 -= otra.m12;  m13 -= otra.m13;
  m20 -= otra.m20; m21 -= otra.m21; m22 -= otra.m22;  m23 -= otra.m23;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: producto
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::operator *= (const Matrix34<T>& otra)
{
  return (*this) = (*this) * otra;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: producto matriz x escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::operator *= (const T escalar)
{
  m00 *= escalar; m01 *= escalar; m02 *= escalar; m03 *= escalar; 
  m10 *= escalar; m11 *= escalar; m12 *= escalar; m13 *= escalar; 
  m20 *= escalar; m21 *= escalar; m22 *= escalar; m23 *= escalar;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: cociente matriz / escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::operator /= (const T escalar)
{
  T inv_escalar = One<T>();

#ifdef CHECK_MATH_SINGULARITY
  //---------------------------------------------------------<<<
  // Con chequeo de dicvisión por cero
  ASSERTMSG(escalar != Zero<T>(), "División por cero en cociente matriz3x4-escalar");
  if(escalar != Zero<T>())
  {
    inv_escalar /= escalar;
  }
  //--------------------------------------------------------->>>
#else
  //---------------------------------------------------------<<<
  // Sin chequeo
  inv_escalar /= escalar;
  //--------------------------------------------------------->>>
#endif

  m00 *= inv_escalar;  m01 *= inv_escalar;  m02 *= inv_escalar;  m03 *= inv_escalar;
  m10 *= inv_escalar;  m11 *= inv_escalar;  m12 *= inv_escalar;  m13 *= inv_escalar;  
  m20 *= inv_escalar;  m21 *= inv_escalar;  m22 *= inv_escalar;  m23 *= inv_escalar; 

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de igualdad absoluta
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Matrix34<T>::operator == (const Matrix34<T>& otra) const
{
  return (0 == memcmp(this, &otra, sizeof(Matrix34<T>)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de desigualdad absoluta
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Matrix34<T>::operator != (const Matrix34<T>& otra) const
{
  return (0 != memcmp(this, &otra, sizeof(Matrix34<T>)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Función de igualdad aproximada con epsilon genérico
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Matrix34<T>::IsEqualEpsilon (const Matrix34<T>& otra, const T Epsilon) const
{
  bool bIgual = true;
  for(int i = 0; i < 12; i++)
  {
    if(!mathUtils::EqualEpsilon( ((T*)this)[i], ((T*)&otra)[i], Epsilon ))
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
inline bool Matrix34<T>::IsNotEqualEpsilon (const Matrix34<T>& otra, const T Epsilon) const
{
  bool bDistinto = false;
  for(int i=0; i<12; i++)
  {
    if(!mathUtils::EqualEpsilon( ((T*)this)[i], ((T*)&otra)[i], Epsilon ))
    {
      bDistinto = true;
      break;
    }
  }

  return bDistinto;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Transformación del vector pasado en el parámetro.
/// Equivalente al producto de la subMatriz 3x3 de la matriz actual por el vector.
/// [ver notas en el operador '^', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix34<T>::TransformVector (Vector3<T>& vector) const
{
  vector = (*this) ^ vector; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Transformación del vector pasado en el parámetro, considerado como punto.
/// [ver notas en el operador '*', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix34<T>::TransformPoint (Vector3<T>& vector) const
{
  vector = (*this) * vector;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector que es la transformación del vector pasado en el parámetro.
/// Equivalente al producto de la submatriz 3x3 de la matriz actual por el vector.
/// [ver notas en el operador '^', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix34<T>::GetTransformVector (const Vector3<T>& vector) const
{
  return (*this) ^ vector;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector que es la transformación del vector pasado en el parámetro (considerado como punto)
/// [ver notas en el operador '*', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix34<T>::GetTransformPoint (const Vector3<T>& vector) const
{
  return (*this) * vector;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Aplicación de la transformación que contiene la subMatriz 3x3 (rotación + escalado) de la matriz actual
/// a un array de vectores entrada, situando el resultado en otro array de vectores salida.
/// [ver notas en el operador '^', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Matrix34<T>::TransformArrayVectors (int iElements, Vector3<T>* pVecOUT, Vector3<T>* pVecIN) const
{
  for(int i=0; i<iElements; i++)
  {
    pVecOUT[i] = (*this) ^ (pVecIN[i]);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Aplicación de la transformación completa (rotacion-escalado-traslación) que contiene la matriz a un array
/// de vectores entrada (considerados como puntos) situando el resultado en otro array de vectores salida.
/// [ver notas en el operador '*', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Matrix34<T>::TransformArrayPoints (int iElements, Vector3<T>* pVecOUT, Vector3<T>* pVecIN) const
{
  for(int i=0; i<iElements; i++)
  {
    pVecOUT[i] = (*this) * (pVecIN[i]);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Invierte la matriz actual
/// Calcula la inversa formal de la matriz actual, que cumple que M*M(inversa) = M(inversa)*M = I
/// \verbatim
///                  traspuesta matriz de cofactores de M
///   M(inversa) = ----------------------------------------
///                        Determinante de M
/// \endverbatim
/// La inversa solo existe para matrices cuadradas pero se puede definir una inversa utilizando
/// la de la matriz 4x4 con la última fila puesta a [0 0 0 1].
/// Esta inversa produce una matriz 4x4 con la última fila a [0 0 0 1], que se puede
/// reconvertir en matriz 3x4 sin perder información significativa.
/// NOTA: La matriz 3x4 sirve como una simplificación de la matriz 4x4 para agilizar
/// cálculos en los casos en que representa una transformación típica de rotacion/traslación/escalado
/// (en esos casos la matriz 4x4 tiene la última fila a [0 0 0 1] si se tiene presente
/// el orden de multiplicación de matriz x vector/punto definido en esta librería)
///
/// (Versión const que devuelve una copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> Matrix34<T>::GetInverted () const
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

		Matrix34<T> auxMat;

		auxMat.m00 = (m11*m22 - m12*m21) * inv_det;
		auxMat.m01 = -(m01*m22 - m02*m21) * inv_det;
		auxMat.m02 = (m01*m12 - m11*m02) * inv_det;
		auxMat.m03 = -Matrix33<T>(m01, m02, m03, m11, m12, m13, m21, m22, m23).Determinant() * inv_det;

		auxMat.m10 = -(m10*m22 - m12*m20) * inv_det;
		auxMat.m11 = (m00*m22 - m02*m20) * inv_det;
		auxMat.m12 = -(m00*m12 - m10*m02) * inv_det;
		auxMat.m13 = Matrix33<T>(m00, m02, m03, m10, m12, m13, m20, m22, m23).Determinant() * inv_det;

		auxMat.m20 = (m10*m21 - m20*m11) * inv_det;
		auxMat.m21 = -(m00*m21 - m20*m01) * inv_det;
		auxMat.m22 = (m00*m11 - m10*m01) * inv_det;
		auxMat.m23 = -Matrix33<T>(m00, m01, m03, m10, m11, m13, m20, m21, m23).Determinant() * inv_det;

		return auxMat;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión que modifica la matriz
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
template<typename T>
Matrix34<T>& Matrix34<T>::Invert ()
{
  return ((*this) = GetInverted());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Calcula la inversa de la matriz si esta cumple los siguientes requisitos:
/// (Asimilándola a una matriz 4x4 con la última fila puesta a [0 0 0 1])
///   - Es una transformación afín compuesta de TRASLACION + ROTACION + ESCALADO
///   - Si la matriz es resultado de multiplicar M(traslacion) * M(Rotacion) * M(escalado), cada
///     matriz de estas se define:
/// \verbatim
///          | 1 0 0 Tx |   | x1  x2  x3  0 |   | ex 0  0  0 |
///          | 0 1 0 Ty | * | y1  y2  y3  0 | * | 0  ey 0  0 |
///          | 0 0 1 Tz |   | z1  z2  z3  0 |   | 0  0  ez 0 |
///          ------------   -----------------   --------------
///          | 0 0 0  1 |   | 0   0   0   1 |   | 0  0  0  1 |
/// \endverbatim
/// Donde la matriz de rotación representa a una base 'ortonormal' en el espacio 3D
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> Matrix34<T>::GetInvertedTRS () const
{
  T div_x = One<T>();
  T div_y = One<T>();
  T div_z = One<T>();

  T esc_x = GetScaleX();
  T esc_y = GetScaleY();
  T esc_z = GetScaleZ();

#ifdef CHECK_MATH_SINGULARITY
  //----------------------------------------------------------<<<
  // Con chequeo de división por cero
  ASSERTMSG(esc_x != Zero<T>() &&
            esc_y != Zero<T>() &&
            esc_z != Zero<T>(),
            "División por cero al calcular inversa de matriz TRS");
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
  //---------------------------------------------------------->>>
#else
  //----------------------------------------------------------<<<
  // Sin chequeo
  div_x /= (esc_x * esc_x);
  div_y /= (esc_y * esc_y);
  div_z /= (esc_z * esc_z);
  //---------------------------------------------------------->>>
#endif

  Matrix44<T> auxMat;

  auxMat.m00 = m00 * div_x;
  auxMat.m01 = m10 * div_x;
  auxMat.m02 = m20 * div_x;

  auxMat.m10 = m01 * div_y;
  auxMat.m11 = m11 * div_y;
  auxMat.m12 = m21 * div_y;

  auxMat.m20 = m02 * div_z;
  auxMat.m21 = m12 * div_z;
  auxMat.m22 = m22 * div_z;

  auxMat.m03 = -m03*auxMat.m00 - m13*auxMat.m01 - m23*auxMat.m02;
  auxMat.m13 = -m03*auxMat.m10 - m13*auxMat.m11 - m23*auxMat.m12;
  auxMat.m23 = -m03*auxMat.m20 - m13*auxMat.m21 - m23*auxMat.m22;

  return auxMat;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::InvertTRS ()
{
  return ((*this) = GetInvertedTRS());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Calcula la inversa de la matriz si esta cumple los siguientes requisitos:
/// (Asimilándola a una matriz 4x4 con la última fila puesta a [0 0 0 1])
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
/// (Versión que devuelve una copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> Matrix34<T>::GetInvertedTR () const
{
  Matrix34<T> auxMat;

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

  return auxMat;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// (Versión que modifica el parámetro)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Matrix34<T>::GetInvertedTR (Matrix34<T>& retMat) const
{
  retMat.m00 = m00;
  retMat.m01 = m10;
  retMat.m02 = m20;

  retMat.m10 = m01;
  retMat.m11 = m11;
  retMat.m12 = m21;

  retMat.m20 = m02;
  retMat.m21 = m12;
  retMat.m22 = m22;

  retMat.m03 = -m03*retMat.m00 - m13*retMat.m01 - m23*retMat.m02;
  retMat.m13 = -m03*retMat.m10 - m13*retMat.m11 - m23*retMat.m12;
  retMat.m23 = -m03*retMat.m20 - m13*retMat.m21 - m23*retMat.m22;  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// (Versión que modifica la matriz)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::InvertTR ()
{
  return ((*this) = GetInvertedTR());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Se aplica una rotación a la matriz de 'angleX' radianes, alrededor del ejeX
/// del sistema de referencia del padre
/// (versión const que devuelve una matriz copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> Matrix34<T>::GetRotedByAngleX (const T angleX) const
{
  return Matrix34<T>(*this).RotByAngleX(angleX);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::RotByAngleX (const T angleX)
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
Matrix34<T> Matrix34<T>::GetRotedByAngleY (const T angleY) const
{
  return Matrix44<T>(*this).RotByAngleY(angleY);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::RotByAngleY (const T angleY)
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
Matrix34<T> Matrix34<T>::GetRotedByAngleZ (const T angleZ) const
{
  return Matrix34<T>(*this).RotByAngleY(angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::RotByAngleZ (const T angleZ)
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
Matrix34<T> Matrix34<T>::GetRotedByAnglesXZ (const T angleX, const T angleZ) const
{
  return Matrix34<T>(*this).RotByAnglesXZ(angleX, angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::RotByAnglesXZ (const T angleX, const T angleZ)
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
Matrix34<T> Matrix34<T>::GetRotedByAnglesYXZ (const T angleY, const T angleX, const T angleZ) const
{
  return Matrix34<T>(*this).RotByAnglesYXZ(angleY, angleX, angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::RotByAnglesYXZ (const T angleY, const T angleX, const T angleZ)
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
Matrix34<T> Matrix34<T>::GetTranslated (const Vector3<T>& pos) const
{
  return Matrix34<T>(*this).Translate(pos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::Translate (const Vector3<T>& pos)
{
  SetPos(pos);

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Escala la matriz (solo escala los vectores de la base y no la posición)
/// (versión const que devuelve una copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T> Matrix34<T>::GetScaled (const T escala_x, const T escala_y, const T escala_z) const
{
  return Matrix34<T>(*this).Scale(escala_x, escala_y, escala_z);  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix34<T>& Matrix34<T>::Scale (const T escala_x, const T escala_y, const T escala_z)
{
	m00 *= escala_x;  m01 *= escala_y;  m02 *= escala_z;
	m10 *= escala_x;  m11 *= escala_y;  m12 *= escala_z;
	m20 *= escala_x;  m21 *= escala_y;  m22 *= escala_z;

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
Matrix34<T>& Matrix34<T>::AlignYX (const Vector3<T>& vY, const Vector3<T>& vX)
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
Matrix34<T>& Matrix34<T>::AlignYZ (const Vector3<T>& vY, const Vector3<T>& vZ)
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
Matrix34<T>& Matrix34<T>::AlignXZ (const Vector3<T>& vX, const Vector3<T>& vZ)
{
	Vector3<T> x = vX;
	Vector3<T> y = vZ ^ vX;
	Vector3<T> z = vX ^ y;

	SetFromBasis(x, y , z);

	ResetScale();

  return (*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// El determinante solo se define para matrices cuadradas pero, como en el caso del
/// producto, podemos definir un determinante para la matriz 3x4, completando con
/// una cuarta fila [0 0 0 1], que nos servirá si la matriz respresenta una
/// transformación típica de rotacion + traslación + escalado
///
/// Para una matriz 4x4 se define como un escalar igual a:
/// \verbatim
///     | m00    m01    m02    m03    |
///     | m10    m11    m12    m13    |          | m11 m12 m13 |         | m10 m12 m13 |         | m10 m11 m13 |         | m10 m11 m12 |
///     | m20    m21    m22    m23    |  = m00 * | m21 m22 m23 | - m01 * | m20 m22 m23 | + m02 * | m20 m21 m23 | - m03 * | m20 m21 m22 |
///     | m30(0) m31(0) m32(0) m33(1) |          |  0   0   1  |         |  0   0   1  |         |  0   0   1  |         |  0   0   0  |
///
/// Coste:         [40 productos y 23 sumas]
/// Simplificado:  [9  productos y 5 sumas]
/// \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
template<typename T>
inline T Matrix34<T>::Determinant () const
{
  return (m00 * (m11*m22 - m21*m12) -
          m01 * (m10*m22 - m12*m20) +
          m02 * (m10*m21 - m20*m11));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve true si la parte de la matriz 3x3 que representa a los vectores 
/// de la base, considerando a la matriz como un sistema de referencia,
/// representa una base en el espacio 3D con sus ejes ortogonales entre sí.
/// (Utiliza un epsilon como margen de tolerancia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
bool Matrix34<T>::IsOrthogonalEpsilon () const
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
bool Matrix34<T>::IsOrthonormalEpsilon () const
{
  return(IsOrthogonalEpsilon() &&
         mathUtils::EqualEpsilon<T>(GetVectorBasis(0).SquaredLength(), One<T>()) && 
         mathUtils::EqualEpsilon<T>(GetVectorBasis(1).SquaredLength(), One<T>()) && 
         mathUtils::EqualEpsilon<T>(GetVectorBasis(2).SquaredLength(), One<T>()));
}
