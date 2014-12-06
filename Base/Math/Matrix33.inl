//-----------------------------------------------------------------------
// Matrix33 inline
/// Definiciones de funciones inline de la clase 'Matrix33'
/// Este fichero es realmente parte de la cabecera 'Matrix33.h' --> NO PONER INCLUDES
//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 1
/// Construcción sin inicialización de parámetros  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>::Matrix33()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 2
/// Constructor de copia
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>::Matrix33 (const Matrix33<T>& otra) :
  m00(otra.m00), m10(otra.m10), m20(otra.m20),
  m01(otra.m01), m11(otra.m11), m21(otra.m21),
  m02(otra.m02), m12(otra.m12), m22(otra.m22)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 3
/// Inserción de todos los datos de la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>::Matrix33 (const T _m00, const T _m01, const T _m02,
                              const T _m10, const T _m11, const T _m12,
                              const T _m20, const T _m21, const T _m22) :
  m00(_m00), m10(_m10), m20(_m20),
  m01(_m01), m11(_m11), m21(_m21),
  m02(_m02), m12(_m12), m22(_m22)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor version 4
/// (Ver comentario de SetFromBasis)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>::Matrix33 (const Vector3<T>& ejeX,
                              const Vector3<T>& ejeY,
                              const Vector3<T>& ejeZ)
{
  SetFromBasis(ejeX, ejeY, ejeZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versión 5
/// (Ver comentario en 'SetFromAnglesYXZ'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>::Matrix33 (const T angleY, const T angleX, const T angleZ)
{
  SetFromAnglesYXZ(angleY, angleX, angleZ); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Elimina la rotación de la matriz, considerándola como una transformación
/// afín ROTACION + ESCALADO. El escalado se mantiene invariable
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
template<typename T>
inline Matrix33<T>& Matrix33<T>::ResetRotation ()
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
/// de ROTACION + ESCALADO. La rotación se mantiene invariable
/// Equivale a normalizar la matriz 3x3 que representa a la rotación
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::ResetScale ()
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
/// Cambia los datos de la matriz por los de una 'matriz identidad'
/// \verbatim  
///   | 1 0 0 |
///   | 0 1 0 |
///   | 0 0 1 |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetIdentity ()
{
  m00 = One<T>();   m10 = Zero<T>();  m20 = Zero<T>();
  m01 = Zero<T>();  m11 = One<T>();   m21 = Zero<T>();
  m02 = Zero<T>();  m12 = Zero<T>();  m22 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pone a cero todas las componentes de la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetZero ()
{
  for(int i=0; i<9; i++)
  {
    ((T*)this)[i] = Zero<T>();
  }

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece los datos de la matriz a partir de 3 vectores 3D que forman una base.
/// Se construye una tranformación de ROTACION o una transformación
/// típica de sistema de coordenadas actual a sistema de coordenadas del padre (en cuyas
/// coordenadas deben darse los vectores parámetro y la posición)
/// En este caso los sistemas solo están rotados entre si
/// \verbatim
///                          | x1 x2 x3 |
///                          | y1 y2 y3 |
///                          | z1 z2 z3 |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetFromBasis (const Vector3<T>& ejeX,
																							 const Vector3<T>& ejeY,
																							 const Vector3<T>& ejeZ)
{
	m00 = ejeX.x;  m01 = ejeY.x;  m02 = ejeZ.x;
	m10 = ejeX.y;  m11 = ejeY.y;  m12 = ejeZ.y;
	m20 = ejeX.z;  m21 = ejeY.z;  m22 = ejeZ.z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleX' radianes alrededor del ejeX (normalmente inclinación)
/// que también es una matriz de sistema de referencia, con todos sus ejes rotados sobre el ejeX
/// del sistema de referencia padre.\n
/// Se usa la regla del pulgar de la mano derecha (con el pulgar en la dirección del eje de rotación,
/// la palma rodeando al eje indica el sentido de la rotación positiva).\n
/// La forma de la matriz resultante será:
/// \verbatim
///        | 1    0        0      |
///        | 0    cos a   -sen a  |
///        | 0    sen a    cos a  |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetFromAngleX (const T angleX)
{
  T sina;
  T cosa;
  mathUtils::SinCos(angleX, sina, cosa);

  m00 = One<T>();   m01 = Zero<T>();  m02 = Zero<T>();
  m10 = Zero<T>();  m11 = cosa;       m12 = -sina;
  m20 = Zero<T>();  m21 = sina;       m22 = cosa;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleY' radianes alrededor del ejeY (normalmente alabeo)
/// que también es una matriz de sistema de referencia, con todos sus ejes rotados sobre el ejeY
/// del sistema de referencia padre.\n
/// Se usa la regla del pulgar de la mano derecha (con el pulgar en la dirección del eje de rotación,
/// la palma rodeando al eje indica el sentido de la rotación positiva).\n
/// La forma de la matriz resultante será:
/// \verbatim
///        |  cos a    0     sin a  |
///        |  0        1     0      |
///        | -sin a    0     cos a  |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetFromAngleY (const T angleY)
{
  T sina;
  T cosa;
  mathUtils::SinCos(angleY, sina, cosa);

  m00 = cosa;       m01 = Zero<T>();  m02 = sina;
  m10 = Zero<T>();  m11 = One<T>();   m12 = Zero<T>();
  m20 = -sina;      m21 = Zero<T>();  m22 = cosa;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleZ' radianes alrededor del ejeZ (normalmente giro)
/// que también es una matriz de sistema de referencia, con todos sus ejes rotados sobre el ejeZ
/// del sistema de referencia padre.\n
/// Se usa la regla del pulgar de la mano derecha (con el pulgar en la dirección del eje de rotación,
/// la palma rodeando al eje indica el sentido de la rotación positiva).\n
/// La forma de la matriz resultante será:
/// \verbatim
///        | cos a   -sen a    0 |
///        | sen a    cos a    0 |
///        | 0        0        1 |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetFromAngleZ (const T angleZ)
{
  T sina;
  T cosa;
  mathUtils::SinCos(angleZ, sina, cosa);

  m00 = cosa;       m01 = -sina;      m02 = Zero<T>();
  m10 = sina;       m11 = cosa;       m12 = Zero<T>();
  m20 = Zero<T>();  m21 = Zero<T>();  m22 = One<T>();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleX' radianes alrededor del ejeX +
/// 'angleZ' alrededor del eje z (primero inclinación y luego giro).
/// También es una matriz de sistema de referencia, con todos sus ejes rotados de la misma
/// forma en el sistema de referencia padre.\n
/// Para saber el sentido de rotación sobre cada eje se usa la regla del pulgar de la mano derecha
/// (con el pulgar en la dirección del eje de rotación, la palma rodeando al eje indica el sentido
/// de la rotación positiva).\n
///
/// La forma de la matriz resultante será el producto: matriz de rotación sobre Z x matriz de rotación sobre X
/// \verbatim
///   | cos z   -sen z    0 |   | 1    0        0      |   | cos z    -sen z * cos x     sen z * sen x |
///   | sen z    cos z    0 | * | 0    cos x   -sen x  | = | sen z     cos z * cos x    -cos z * sen x |
///   | 0        0        1 |   | 0    sen x    cos x  |   |  0        sen x             cos x         |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetFromAnglesXZ (const T angleX, const T angleZ)
{
  T sinx, cosx, sinz, cosz;
  mathUtils::SinCos(angleX, sinx, cosx);
  mathUtils::SinCos(angleZ, sinz, cosz);

  m00 = cosz;         m01 = -sinz * cosx;   m02 = sinz * sinx;
  m10 = sinz;         m11 = cosz * cosx;    m12 = -cosz * sinx;
  m20 = Zero<T>();    m21 = sinx;           m22 = cosx;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Crea una matriz de rotación de 'angleY' radianes alrededor del ejeY +
/// 'angleX' radianes alrededor del ejeX + 'angleZ' alrededor del eje z
/// (primero alabeo, luego inclinación y luego giro).
/// También es una matriz de sistema de referencia, con todos sus ejes rotados de la misma
/// forma en el sistema de referencia padre.\n
/// Para saber el sentido de rotación sobre cada eje se usa la regla del pulgar de la mano derecha
/// (con el pulgar en la dirección del eje de rotación, la palma rodeando al eje indica el sentido
/// de la rotación positiva).\n
///
/// La forma de la matriz resultante será el producto:
/// matriz de rotación sobre Z x matriz de rotación sobre X x matriz de rotación sobre Y
/// \verbatim
///   | cos z   -sen z    0 |   | 1    0        0      |   | cos z    -sen z * cos x     sen z * sen x |
///   | sen z    cos z    0 | * | 0    cos x   -sen x  | = | sen z     cos z * cos x    -cos z * sen x |
///   | 0        0        1 |   | 0    sen x    cos x  |   |  0        sen x             cos x         |
///
///   | cos z  -sen z * cos x   sen z * sen x |   |  cos y    0     sin y  |   | cz*cy - sz*sx*sy    -sz*cx    cz*sy + sz*sx*cy |  
///   | sen z   cos z * cos x  -cos z * sen x | * |  0        1     0      | = | sz*cy + cz*sx*sy     cz*cx    sz*sy - cz*sx*cy |
///   |  0      sen x           cos x         |   | -sin y    0     cos y  |   | -cx*sy                sx      cx*cy            |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetFromAnglesYXZ (const T angleY, const T angleX, const T angleZ)
{
  T sx, cx, sy, cy, sz, cz;
  mathUtils::SinCos(angleX, sx, cx);
  mathUtils::SinCos(angleY, sy, cy);
  mathUtils::SinCos(angleZ, sz, cz);

  m00 = cz*cy - sz*sx*sy;    m01 = -sz*cx;    m02 = cz*sy + sz*sx*cy;
  m10 = sz*cy + cz*sx*sy;    m11 = cz*cx;     m12 = sz*sy - cz*sx*cy;
  m20 = -cx*sy;              m21 = sx;        m22 = cx*cy;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Construye una matriz de escala, actualizando la diagonal de la submatriz 3x3
/// en las 3 primeras columnas, a partir de una magnitud para cada eje de la base
/// que forma la transformación de la matriz
/// \verbatim
///          | escala_X     0         0     |
///          |    0      escala_Y     0     |
///          |    0         0      escala_Z |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetFromScale (const T escala_x, const T escala_y, const T escala_z)
{
  m00 = escala_x;    m01 = Zero<T>();   m02 = Zero<T>();
  m10 = Zero<T>();   m11 = escala_y;    m12 = Zero<T>();
  m20 = Zero<T>();   m21 = Zero<T>();   m22 = escala_z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece la matriz de forma que el eje Y tendrá la dirección y el sentido del vector que va
/// desde 'vPos' hasta 'vTarget', utilizando el vector 'vUp' como referencia del sentido del 'arriba' del
/// mundo, para poder construir un sistema de referencia con 3 vectores típico de una matriz de cámara, siendo
/// el eje Y el vector de vista.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetFromLookAt (const Vector3<T>& vPos, const Vector3<T>& vTarget, const Vector3<T>& vUp)
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
  
  SetFromBasis(vRight_eje_X, vUP_eje_Y, vLookAt_eje_Z);
  return (*this);
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
inline Matrix33<T>& Matrix33<T>::SetFromLookAt (const Vector3<T>& vPos, const Vector3<T>& vTarget)
{
  return SetFromLookAt(vPos, vTarget, v3fY);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*template<typename T>
inline Matrix33<T>& Matrix33<T>::SetFromLookAt (const Vector3<T>& vPos, const Vector3<T>& vTarget, const T angle)
{
  // [TODO: implementar]
}*/
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece los datos de la matriz a partir de un cuaternión
/// (Ver comentarios en la clase 'Quatn' del cuaternión)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetFromQuat (const Quatn<T>& quat)
{
  quat.GetMatrix33(*this);
  return (*this);
}*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Igual que 'SetFromAngleX' pero solo modifica la rotación
/// dejando el escalado invariable
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetRotByAngleX (const T angleX)
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
/// Igual que 'SetFromAngleY' pero solo modifica la rotación
/// dejando el escalado invariable
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetRotByAngleY (const T angleY)
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
/// Igual que 'SetFromAngleZ' pero solo modifica la parte de rotación
/// dejando el escalado invariable
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetRotByAngleZ (const T angleZ)
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
/// Igual que 'SetFromAngleXZ' pero solo modifica la parte de rotación
/// dejando el escalado invariable
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetRotByAnglesXZ (const T angleX, const T angleZ)
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
/// Igual que 'SetFromAngleYXZ' pero solo modifica la parte de rotación
/// dejando el escalado invariable
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetRotByAnglesYXZ (const T angleY, const T angleX, const T angleZ)
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
/// Establece la rotación de la matriz a partir de un cuaternión dejando el escalado invariable
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix34<T>& Matrix33<T>::SetRotByQuat(const Quatn<T>& quat)
{
  Vector3<T> vSaveScale = GetScale();
  SetFromQuat(quat);
  Scale(vSaveScale);
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece el escalado de la matriz considerada como una transformación.
/// La parte de rotación permenece invariable.
/// El proceso es:
///   - Eliminar el escalado actual (normalizando los vectores base que definen los ejes)
///   - Establecer el nuevo escalado. Multiplicando cada vector base por su escalado correspondiente.
///
/// NOTA: La matriz parámetro es considerada una matriz de escalado 3x3:
/// \verbatim
///       | escala_x     0         0     |
///       |    0      escala_y     0     |
///       |    0         0      escala_z |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetScale (const Matrix33<T>& mat_escala)
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
/// La parte de rotación permenece invariable.
/// El proceso es:
///   - Eliminar el escalado actual (normalizando los vectores base que definen los ejes)
///   - Establecer el nuevo escalado. Multiplicando cada vector base por su escalado correspondiente.
///
/// El parámetro es un vector 3D cuyas componentes son las escalas
/// en los tres ejes X, Y, Z.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T>& Matrix33<T>::SetScale (const Vector3<T>& vect_escala)
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
inline Matrix33<T>& Matrix33<T>::SetScale (const T escala_x, const T escala_y, const T escala_z)
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
/// Acceso a filas como vectores 3D
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix33<T>::GetRow (int i) const
{
#ifdef ROW_MAJOR_MATRICES
	return *((Vector3<T>*)this + i);
#else
  return Vector3<T>( ((T*)this)[i],
                     ((T*)this)[3 + i],
                     ((T*)this)[6 + i] );
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve el módulo del vector formado por los 3  elementos de una columna
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix33<T>::GetVectorBasisLength (int i) const
{
#ifdef ROW_MAJOR_MATRICES
	return Vector3<T>(((T*)this)[i],
										((T*)this)[3 + i],
										((T*)this)[6 + i] ).Length();
#else
	return (*((Vector3<T>*)this + i)).Length();
#endif

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve una columna en forma de vector 3D. Las columnas son los vectores
/// de la base si la matriz representa una tranformación afín
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix33<T>::GetColum (int i) const
{
#ifdef ROW_MAJOR_MATRICES
	return Vector3<T>(((T*)this)[i],
										((T*)this)[3 + i],
										((T*)this)[6 + i] );
#else
  return *((Vector3<T>*)this + i);
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve los 3  elementos de una columna, interpretados como un
/// vector 3D que forma la base que define la matriz en coordenadas del
/// sistema de referencia padre
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix33<T>::GetVectorBasis (int i) const
{
  return GetColum(i);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve los 3 elementos de cada columna, interpretados
/// como vectores 3D y como una base en coordenadas del
/// sistema de referencia padre
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix33<T>::GetBasis (Vector3<T>& vBasisX,
                                   Vector3<T>& vBasisY,
                                   Vector3<T>& vBasisZ) const
{
  vBasisX = GetColum(0);
  vBasisY = GetColum(1);
  vBasisZ = GetColum(2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve una matriz 3x3 con la parte de rotación, considerando
/// a la matriz una trasnformación compuesta por una traslacion + escalado
/// (Versión que anula el escalado)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T> Matrix33<T>::Get33RotationNormalized () const
{
  return Matrix33<T>(GetVectorBasis(0).GetNormalized(),
                     GetVectorBasis(1).GetNormalized(),
                     GetVectorBasis(2).GetNormalized());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve una matriz 3x3 con la parte de rotación, considerando
/// a la matriz una transformación compuesta por una traslación + escalado.
/// Devuelve una referencia a si misma
/// (Versión que contiene el escalado)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T> Matrix33<T>::Get33RotationScaled () const
{
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la matriz de escalado. Considerando a la matriz compuesta por
/// una rotación + escalado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Matrix33<T> Matrix33<T>::Get33Scale () const
{
  return Matrix33<T>(GetVectorBasisLength(0),              0.0          ,            0.0,
                              0.0              , GetVectorBasisLength(1),            0.0,
                              0.0              ,              0.0          , GetVectorBasisLength(2));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve las escalas de la matriz en los 3 ejes en forma de vector 3D
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix33<T>::GetScale() const
{
  return Vector3<T>(GetVectorBasisLength(0),
                    GetVectorBasisLength(1),
                    GetVectorBasisLength(2));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la escala de la matriz en el ejeX, considerando la base
/// del sistema de referencia que define.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix33<T>::GetScaleX () const
{
  return GetVectorBasisLength(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la escala de la matriz en el ejeY, considerando la base
/// del sistema de referencia que define.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix33<T>::GetScaleY () const
{
  return GetVectorBasisLength(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve la escala de la matriz en el ejeZ, considerando la base
/// del sistema de referencia que define.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix33<T>::GetScaleZ () const
{
  return GetVectorBasisLength(2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención de los angulos de Euler que representan la rotación contenida en la matriz 3x3,
/// en el orden de aplicación: Y -> X -> Z
///
/// Dada la matriz de aplicación de ángulos en ese orden (la que se establece mediante 'SetFromAnglesYXZ'
/// \verbatim
///   | cz*cy - sz*sx*sy    -sz*cx    cz*sy + sz*sx*cy |
///   | sz*cy + cz*sx*sy     cz*cx    sz*sy - cz*sx*cy |
///   | -cx*sy                sx      cx*cy            |
///
///   y = atan(-m20/m22)
///   x = asin(m21)
///   z = atan(-m01/m11)
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Matrix33<T>::GetAnglesYXZ(T& angleY, T& angleX, T& angleZ) const
{
  angleY = mathUtils::ATan2(-m20, m22);
  angleX = mathUtils::ASin(m21);
  angleZ = mathUtils::ATan2(-m01, m11); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención del angulo de Euler que representa la rotación sobre el eje X. El dato se extrae de la matriz
/// despejándolo de la expresión de la misma como función de los angulos de Euler en el orden Y->X->Z
/// (Ver comentario en 'GetAnglesYXZ')
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
T Matrix33<T>::GetAngleX() const
{
  return mathUtils::ASin(m21);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención del angulo de Euler que representa la rotación sobre el eje Y. El dato se extrae de la matriz
/// despejándolo de la expresión de la misma como función de los angulos de Euler en el orden Y->X->Z
/// (Ver comentario en 'GetAnglesYXZ')
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
T Matrix33<T>::GetAngleY() const
{
  return mathUtils::ATan2(-m20, m22);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtención del angulo de Euler que representa la rotación sobre el eje Z. El dato se extrae de la matriz
/// despejándolo de la expresión de la misma como función de los angulos de Euler en el orden Y->X->Z
/// (Ver comentario en 'GetAnglesYXZ')
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
T Matrix33<T>::GetAngleZ() const
{
  return mathUtils::ATan2(-m01, m11); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el ángulo de rotación sobre el eje Y, siguiendo el criterio de ángulos
/// y orden de concatenación expresado en 'SetPitchRollYaw'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
T Matrix33<T>::GetRoll() const
{
  return mathUtils::ATan2(m20, m00);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el ángulo de rotación sobre el eje X, siguiendo el criterio de ángulos
/// y orden de concatenación expresado en 'SetPitchRollYaw'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
T Matrix33<T>::GetPitch() const
{
  return mathUtils::ATan2(-m12, m11);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el ángulo de rotación sobre el eje Z, siguiendo el criterio de ángulos
/// y orden de concatenación expresado en 'SetPitchRollYaw'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
T Matrix33<T>::GetYaw() const
{
  return mathUtils::ASin(-m10);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Los ángulos de extraen a partir de esta matriz paramétrica, resultado de concatenar
/// las rotaciones en (-Y)(-Z)(X)   (Ver comentario en 'SetPitchRollYaw')
/// \verbatim
/// | cy*cz    cy*sz*cx - sy*sx     -cy*sz*sx - sy*cx  |
/// |  -sz      cz*cx                  -cz*sx          |
/// | sy*cz    sy*sz*cx + cy*sx     -sy*sz*sx + cy*cx  |
///
///  m10 = -sz                   -->  z = Asin(-m10)
///
///  m12    -cz * sx
/// ----- = --------- = -Tan(x)  -->  x = ATan(-m12/m11)
///  m11     cz * cx
///
///  m20     sy * cz
/// ----- = --------- = Tan(y)   -->  y = Atan(m20/m00)
///  m00     cy * cz
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Vector3<T> Matrix33<T>::GetPitchRollYaw () const
{
  return Vector3<T>(mathUtils::ATan2(-m12, m11),
                    mathUtils::ATan2(m20, m00),
                    mathUtils::ASin(-m10));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establecimiento a partir de ángulos de Euler con el criterio de "Pitch-Roll-Yaw".
/// Siguen el convenio (utilizando angulos positivos con el criterio del pulgar de la mano derecha):
///
/// Pitch -> Rotación positiva alrededor del eje X
/// Roll  -> Rotación NEGATIVA alrededor del eje Y
/// Yaw   -> Rotación NEGATIVA alrededor del eje Z
///
/// El orden de aplicación es:
/// \verbatim
///     (PITCH)       --->     (ROLL)        --->     (YAW)
/// Rotación sobre X  ---> Rotación sobre -Z ---> Rotación sobre -Y
/// \endverbatim
///
/// Las matrices de rotación con el ángulo de parámetro se encadenan al revés:
///  (-Y)(-Z)(X)
/// \verbatim
/// |  cos (-y)  0   sin (-y) |   | cos (-z)  -sen (-z) 0 |   | 1    0        0      |   
/// |  0         1   0        | * | sen (-z)   cos (-z) 0 | * | 0    cos x   -sen x  | =
/// | -sin (-y)  0   cos (-y) |   | 0          0        1 |   | 0    sen x    cos x  |   
///
/// | cos y   0  -sin y |   |  cos z   sen z   0 |   | 1    0        0      |   
/// | 0       1   0     | * | -sen z   cos z   0 | * | 0    cos x   -sen x  |  =
/// | sin y   0   cos y |   |  0       0       1 |   | 0    sen x    cos x  |   
///
/// | cy*cz   cy*sz  -sy |   | 1  0    0  |
/// | -sz      cz     0  | * | 0  cx  -sx | = 
/// | sy*cz   sy*sz  cy  |   | 0  sx   cx |
///
/// | cy*cz    cy*sz*cx - sy*sx     -cy*sz*sx - sy*cx  |
/// |  -sz      cz*cx                  -cz*sx          |
/// | sy*cz    sy*sz*cx + cy*sx     -sy*sz*sx + cy*cx  | 
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::SetPitchRollYaw (const Vector3<T>& v3PitchRollYaw)
{
  T sx, cx, sy, cy, sz, cz;
  mathUtils::SinCos(v3PitchRollYaw.x, sx, cx);
  mathUtils::SinCos(v3PitchRollYaw.y, sy, cy);
  mathUtils::SinCos(v3PitchRollYaw.z, sz, cz);

  m00 = cy*cz;  m01 = cy*sz*cx - sy*sx;  m02 = -cy*sz*sx - sy*cx;
  m10 = -sz;    m11 = cz*cx;             m12 = -cz*sx;
  m20 = sy*cz;  m21 = sy*sz*cx + cy*sx;  m22 = -sy*sz*sx + cy*cx;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: Suma de matrices
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> Matrix33<T>::operator + (const Matrix33<T>& otra) const
{
  return Matrix33<T>(m00 + otra.m00, m01 + otra.m01, m02 + otra.m02,
                     m10 + otra.m10, m11 + otra.m11, m12 + otra.m12,
                     m20 + otra.m20, m21 + otra.m21, m22 + otra.m22);
                      
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: Resta de matrices
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> Matrix33<T>::operator - (const Matrix33<T>& otra) const
{
  return Matrix33<T>(m00 - otra.m00, m01 - otra.m01, m02 - otra.m02,
                     m10 - otra.m10, m11 - otra.m11, m12 - otra.m12,
                     m20 - otra.m20, m21 - otra.m21, m22 - otra.m22);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: Producto de matrices [3 3] x [3 3]
/// \verbatim
///   | m00 m01 m02 |   | m00 m01 m02 |
///   | m10 m11 m12 | * | m10 m11 m12 |
///   | m20 m21 m22 |   | m20 m21 m22 |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> Matrix33<T>::operator * (const Matrix33<T>& otra) const
{
  return Matrix33<T>(m00 * otra.m00 + m01 * otra.m10 + m02 * otra.m20,
                     m00 * otra.m01 + m01 * otra.m11 + m02 * otra.m21,
                     m00 * otra.m02 + m01 * otra.m12 + m02 * otra.m22,

                     m10 * otra.m00 + m11 * otra.m10 + m12 * otra.m20,
                     m10 * otra.m01 + m11 * otra.m11 + m12 * otra.m21,
                     m10 * otra.m02 + m11 * otra.m12 + m12 * otra.m22,

                     m20 * otra.m00 + m21 * otra.m10 + m22 * otra.m20,
                     m20 * otra.m01 + m21 * otra.m11 + m22 * otra.m21,
                     m20 * otra.m02 + m21 * otra.m12 + m22 * otra.m22);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: producto matriz x escalar 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> Matrix33<T>::operator * (const T escalar) const
{
  return (Matrix33<T>(m00 * escalar, m01 * escalar, m02 * escalar,
                      m10 * escalar, m11 * escalar, m12 * escalar,
                      m20 * escalar, m21 * escalar, m22 * escalar));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador externo: producto escalar x matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> operator * (const T escalar, const Matrix33<T>& mat)
{
  return (Matrix33<T>(mat.m00 * escalar, mat.m01 * escalar, mat.m02 * escalar,
                      mat.m10 * escalar, mat.m11 * escalar, mat.m12 * escalar,
                      mat.m20 * escalar, mat.m21 * escalar, mat.m22 * escalar)); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: cociente matriz / escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> Matrix33<T>::operator / (const T escalar) const
{
#ifdef CHECK_MATH_SINGULARITY
  //----------------------------------------------------------------------------------<<<
  Matrix33<T> retMat(*this);

  ASSERTMSG(escalar != Zero<T>(), "División por cero en cociente matriz3x3-escalar");
  if(escalar != Zero<T>())
  {
    T inv_escalar = One<T>() / escalar;
    retMat *= inv_escalar;
  }

  return retMat;
  //---------------------------------------------------------------------------------->>>
#else
  //----------------------------------------------------------------------------------<<<
  T inv_escalar = One<T>() / escalar;
  return (Matrix33<T>(m00 * inv_escalar, m01 * inv_escalar, m02 * inv_escalar,
                      m10 * inv_escalar, m11 * inv_escalar, m12 * inv_escalar,
                      m20 * inv_escalar, m21 * inv_escalar, m22 * inv_escalar));
  //---------------------------------------------------------------------------------->>>
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético: producto de matriz x vector 3D
///                      equivalente a produto de matrices [3 3]x[3 1]
///                      equivalente a una transformación de ROTACION
/// \verbatim
///         | m00 m01 m02 |     | x |     | x*m00 + y*m01 + z*m02 |
///         | m10 m11 m12 |  ^  | y |  =  | x*m10 + y*m11 + z*m12 |
///         | m20 m21 m22 |     | z |     | x*m20 + y*m21 + z*m22 |
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Vector3<T> Matrix33<T>::operator * (const Vector3<T>& vector) const
{
  return Vector3<T>(vector.x * m00 + vector.y * m01 + vector.z * m02,
                    vector.x * m10 + vector.y * m11 + vector.z * m12,
                    vector.x * m20 + vector.y * m21 + vector.z * m22);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: suma
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::operator += (const Matrix33<T>& otra)
{
  m00 += otra.m00; m01 += otra.m01; m02 += otra.m02;
  m10 += otra.m10; m11 += otra.m11; m12 += otra.m12;
  m20 += otra.m20; m21 += otra.m21; m22 += otra.m22;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: resta
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::operator -= (const Matrix33<T>& otra)
{
  m00 -= otra.m00; m01 -= otra.m01; m02 -= otra.m02;
  m10 -= otra.m10; m11 -= otra.m11; m12 -= otra.m12;
  m20 -= otra.m20; m21 -= otra.m21; m22 -= otra.m22;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: producto
///
///         | m00 m01 m02 |     | n00 n01 n02 |
///         | m10 m11 m12 |  *  | n10 n11 n12 |
///         | m20 m21 m22 |     | n20 n21 n22 |
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::operator *= (const Matrix33<T>& otra)
{
  return (*this) = (*this) * otra;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritmético de actualización: producto por escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::operator *= (const T escalar)
{
  m00 *= escalar; m01 *= escalar; m02 *= escalar;
  m10 *= escalar; m11 *= escalar; m12 *= escalar;
  m20 *= escalar; m21 *= escalar; m22 *= escalar;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Operador aritmético de actualización: cociente por escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::operator /= (const T escalar)
{
#ifdef CHECK_MATH_SINGULARITY
  //----------------------------------------------------------------------------<<<
  ASSERTMSG(escalar != Zero<T>(), "División por cero en cociente matriz3x3-escalar");
  if(escalar != Zero<T>())
  {
    T inv_escalar = One<T>()/escalar;
    m00 *= inv_escalar;  m01 *= inv_escalar;  m02 *= inv_escalar;
    m10 *= inv_escalar;  m11 *= inv_escalar;  m12 *= inv_escalar;
    m20 *= inv_escalar;  m21 *= inv_escalar;  m22 *= inv_escalar;
  }
  //---------------------------------------------------------------------------->>>
#else
  //----------------------------------------------------------------------------<<<
  T inv_escalar = One<T>()/escalar;
  m00 *= inv_escalar;  m01 *= inv_escalar;  m02 *= inv_escalar;
  m10 *= inv_escalar;  m11 *= inv_escalar;  m12 *= inv_escalar;
  m20 *= inv_escalar;  m21 *= inv_escalar;  m22 *= inv_escalar;
  //---------------------------------------------------------------------------->>>
#endif
 
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de igualdad absoluta
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Matrix33<T>::operator == (const Matrix33<T>& otra) const
{
  return (0 == memcmp(this, &otra, sizeof(Matrix33<T>)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de desigualdad absoluta
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Matrix33<T>::operator != (const Matrix33<T>& otra) const
{
  return (0 != memcmp(this, &otra, sizeof(Matrix33<T>)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Función de igualdad aproximada con epsilon genérico
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Matrix33<T>::IsEqualEpsilon (const Matrix33<T>& otra, const T Epsilon) const
{
  bool bIgual = true;
  for(int i = 0; i < 9; i++)
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
inline bool Matrix33<T>::IsNotEqualEpsilon (const Matrix33<T>& otra, const T Epsilon) const
{
  bool bDistinto = false;
  for(int i=0; i<9; i++)
  {
    T valor    = ((T*)this)[i];
    T valorRef = ((T*)&otra)[i];

    if(valor < (valorRef - Epsilon) || valor > (valorRef + Epsilon))
    {
      bDistinto = true;
      break;
    }
  }

  return bDistinto;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Invierte la matriz. La matriz inversa es una matriz que cumple que:
/// \verbatim
///    inv{M}*M = I (matriz identidad)
///
/// Cada elemento de la matriz inversa se define como:
///
///                          (-1)^(i+j) |M'ji|             Matriz de cofactores traspuesta
///            inv{aij} = -----------------------           dividida por el determinante
///                                 |M|
///
/// |M'ji| es el menor del elemento aij traspuesto
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::Invert ()
{
  return ((*this) = GetInverted());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión 'const' que devuelve una copia
/// Coste: [36 productos y 14 sumas]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> Matrix33<T>::GetInverted () const
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

  Matrix33<T> auxMat;

  auxMat.m00 = (m11*m22 - m12*m21) * inv_det;
  auxMat.m10 = (m12*m20 - m10*m22) * inv_det;
  auxMat.m20 = (m10*m21 - m11*m20) * inv_det;

  auxMat.m01 = (m02*m21 - m01*m22) * inv_det;
  auxMat.m11 = (m00*m22 - m02*m20) * inv_det;
  auxMat.m21 = (m01*m20 - m00*m21) * inv_det;

  auxMat.m02 = (m01*m12 - m02*m11) * inv_det;
  auxMat.m12 = (m02*m10 - m00*m12) * inv_det;
  auxMat.m22 = (m00*m11 - m01*m10) * inv_det;

  return auxMat;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Invierte la matriz considerándola un producto  R * E
/// \verbatim
///   E----->escalado
///   R----->rotación (base ortonormal)
///
///  M(inversa) = E(inversa) * R(inversa)
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::InvertRS ()
{
  return ((*this) = GetInvertedRS());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión 'const' que devuelve una copia
/// Coste: [21 productos 6 sumas 3 raices]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> Matrix33<T>::GetInvertedRS () const
{
  T div_x = One<T>();
  T div_y = One<T>();
  T div_z = One<T>();

  T esc_x = GetScaleX();
  T esc_y = GetScaleY();
  T esc_z = GetScaleZ();

#ifdef CHECK_MATH_SINGULARITY
  //-----------------------------------------------------<<<
  // Con chequeo de división por cero
  ASSERTMSG(esc_x != Zero<T>() &&
            esc_y != Zero<T>() &&
            esc_z != Zero<T>(),
            "División por cero en cálculo de matriz inversa de RS");
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
  //----------------------------------------------------->>>
#else
  //-----------------------------------------------------<<<
  // Sin chequeo
  div_x /= (esc_x * esc_x);
  div_y /= (esc_y * esc_y);
  div_z /= (esc_z * esc_z);
  //----------------------------------------------------->>>
#endif

  Matrix33<T> auxMat;

  auxMat.m00 = m00 * div_x;
  auxMat.m01 = m10 * div_x;
  auxMat.m02 = m20 * div_x;

  auxMat.m10 = m01 * div_y;
  auxMat.m11 = m11 * div_y;
  auxMat.m12 = m21 * div_y;

  auxMat.m20 = m02 * div_z;
  auxMat.m21 = m12 * div_z;
  auxMat.m22 = m22 * div_z;

  return auxMat;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Trasposición de la matriz (cambio de filas por columnas)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::Transpose ()
{
  T valor;
  valor = m01;  m01 = m10;  m10 = valor;
  valor = m02;  m02 = m20;  m20 = valor;
  valor = m12;  m12 = m21;  m21 = valor;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve una matriz traspuesta de la actual
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> Matrix33<T>::GetTransposed () const
{
  return Matrix33<T>(m00, m10, m20,
                     m01, m11, m21,
                     m02, m12, m22);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Se aplica una rotación a la matriz de 'angleX' radianes, alrededor del ejeX
/// del sistema de referencia del padre
/// (versión const que devuelve una matriz copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> Matrix33<T>::GetRotedByAngleX (const T angleX) const
{
  return Matrix33<T>(*this).RotByAngleX(angleX);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
Matrix33<T>& Matrix33<T>::RotByAngleX (const T angleX)
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
Matrix33<T> Matrix33<T>::GetRotedByAngleY (const T angleY) const
{
  return Matrix33<T>(*this).RotByAngleY(angleY);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::RotByAngleY (const T angleY)
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
Matrix33<T> Matrix33<T>::GetRotedByAngleZ (const T angleZ) const
{ 
  return Matrix33<T>(*this).RotByAngleZ(angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::RotByAngleZ (const T angleZ)
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
Matrix33<T> Matrix33<T>::GetRotedByAnglesXZ (const T angleX, const T angleZ) const
{
  return Matrix33<T>(*this).RotByAnglesXZ(angleX, angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::RotByAnglesXZ (const T angleX, const T angleZ)
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
Matrix33<T> Matrix33<T>::GetRotedByAnglesYXZ (const T angleY, const T angleX, const T angleZ) const
{
  return Matrix33<T>(*this).RotByAnglesYXZ(angleY, angleX, angleZ);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::RotByAnglesYXZ (const T angleY, const T angleX, const T angleZ)
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
/// Escala la matriz (los tres vectores, que forman la base de la
/// transformación que define, por separado)
/// (versión const que devuelve una copia)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T> Matrix33<T>::GetScaled (const T escala_x, const T escala_y, const T escala_z) const
{
  return Matrix33<T>(*this).Scale(escala_x, escala_y, escala_z);  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Versión no const que modifica la matriz
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
Matrix33<T>& Matrix33<T>::Scale (const T escala_x, const T escala_y, const T escala_z)
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
Matrix33<T>& Matrix33<T>::AlignYX (const Vector3<T>& vY, const Vector3<T>& vX)
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
Matrix33<T>& Matrix33<T>::AlignYZ (const Vector3<T>& vY, const Vector3<T>& vZ)
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
Matrix33<T>& Matrix33<T>::AlignXZ (const Vector3<T>& vX, const Vector3<T>& vZ)
{
	Vector3<T> x = vX;
	Vector3<T> y = vZ ^ vX;
	Vector3<T> z = vX ^ y;

	SetFromBasis(x, y , z);

	ResetScale();

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Calcula el determinante de la matriz. Para una matriz 3x3 es un escalar definido como:
/// \verbatim
///   | m00 m01 m02 |
///   | m10 m11 m12 | = m00*m11*m22 + m10*m21*m02 + m01*m12*m20 - m02*m11*m20 - m01*m10*m22 - m12*m21*m00 = 
///   | m20 m21 m22 |   
///                     m00*(m11*m22 - m12*21) - m01*(m10*m22 - m12*m20) + m02*(m10*m21 - m11*m20)
///
/// Coste: [9 productos y 5 sumas]
/// \endverbatim
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Matrix33<T>::Determinant () const
{
  return m00*(m11*m22 - m12*m21) + m01*(m12*m20 - m10*m22) + m02*(m10*m21 - m11*m20);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve true si la matriz representa una base en el espacio 3D
/// con sus ejes ortogonales entre sí.
/// Utiliza un epsilon como margen de tolerancia
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
bool Matrix33<T>::IsOrthogonalEpsilon () const
{
  return(mathUtils::ZeroEpsilon<T>(GetVectorBasis(0) * GetVectorBasis(1)) &&
         mathUtils::ZeroEpsilon<T>(GetVectorBasis(0) * GetVectorBasis(2)) &&
         mathUtils::ZeroEpsilon<T>(GetVectorBasis(1) * GetVectorBasis(2)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve true si la matriz representa una base en el espacio 3D
/// con sus ejes ortogonales entre si y normalizados (módulo = 1).
/// Utiliza un epsilon como margen de tolerancia
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
bool Matrix33<T>::IsOrthonormalEpsilon () const
{
  return(IsOrthogonalEpsilon() &&
         mathUtils::EqualEpsilon<T>(GetVectorBasis(0).SquaredLength(), One<T>()) && 
         mathUtils::EqualEpsilon<T>(GetVectorBasis(1).SquaredLength(), One<T>()) && 
         mathUtils::EqualEpsilon<T>(GetVectorBasis(2).SquaredLength(), One<T>()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Transformación del vector pasado en el parámetro. Equivalente al producto de matriz x vector 3D.
/// Equivalente a produto de matrices [3 3]x[3 1]. Equivalente a una transformación de ROTACION.
/// [ver notas en el operador '*', producto por vector]                     
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix33<T>::TransformVector (Vector3<T>& vector) const
{
  vector = (*this) * vector; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector que es la versión transformada del vector pasado en el parámetro.
/// Equivalente a multiplicar la matriz actual por el vector dado.
/// [ver notas en el operador '*', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Matrix33<T>::GetTransformVector (const Vector3<T>& vector) const
{
  return (*this) * vector;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Aplicación de la transformación que contiene la matriz (rotación + escalado) a un array de vectores entrada,
/// situando el resultado en otro array de vectores salida
/// [ver notas en el operador '*', producto por vector]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Matrix33<T>::TransformArrayVectors (int iElements, Vector3<T>* pVecOUT, Vector3<T>* pVecIN) const
{
  for(int i=0; i<iElements; i++)
  {
    pVecOUT[i] = (*this) * (pVecIN[i]);
  }
}
