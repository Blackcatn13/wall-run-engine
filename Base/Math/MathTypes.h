//-----------------------------------------------------------------------
// Math Types
// Tipos y constantes de la librería matemática
//-----------------------------------------------------------------------

#ifndef __MATH_TYPES_H__
#define __MATH_TYPES_H__

// Constantes float
#include "Utils/Types.h"

#include <float.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constantes numéricas
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const double ALG_EPSILON_DOUBLE = (double)1e-05;
const float  ALG_EPSILON_FLOAT  = (float)ALG_EPSILON_DOUBLE;

const double QUAT_POLE_EPSILON_DOUBLE = (double)0.999950;
const float  QUAT_POLE_EPSILON_FLOAT  = (float)0.99990f;

const double DOUBLE_PI_VALUE = 3.14159265358979323846;
const float  FLOAT_PI_VALUE  = (float)DOUBLE_PI_VALUE;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constantes numéricas con tipo templatizado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>  inline T      Epsilon           () { return 0; }
template<>            inline float  Epsilon<float>    () { return ALG_EPSILON_FLOAT; }
template<>            inline double Epsilon<double>   () { return ALG_EPSILON_DOUBLE; }

template<typename T>  inline T      One               () { return 1;    }
template<>            inline float  One<float>        () { return 1.0f; }
template<>            inline double One<double>       () { return 1.0;  }

template<typename T>  inline T      MinusOne          () { return -1;    }
template<>            inline float  MinusOne<float>   () { return -1.0f; }
template<>            inline double MinusOne<double>  () { return -1.0;  }

template<typename T>  inline T      Zero              () { return 0;    }
template<>            inline float  Zero<float>       () { return 0.0f; }
template<>            inline double Zero<double>      () { return 0.0;  }

template<typename T>  inline T      Half              () { return (T)0.5; }
template<>            inline float  Half<float>       () { return 0.5f;   }
template<>            inline double Half<double>      () { return 0.5;    }

template<typename T>  inline T      Two               () { return 2;    }
template<>            inline float  Two<float>        () { return 2.0f; }
template<>            inline double Two<double>       () { return 2.0;  }

// Epsilon utilizado en la determinación de ángulos de Euler
template<typename T>  inline T      QuatPoleEpsilon         () { return 0; }
template<>            inline float  QuatPoleEpsilon<float>  () { return QUAT_POLE_EPSILON_FLOAT; }
template<>            inline double QuatPoleEpsilon<double> () { return QUAT_POLE_EPSILON_DOUBLE; }

// Constantes relacionadas con el número PI
template<typename T>  inline T      ePI            () { return (T)DOUBLE_PI_VALUE; }
template<>            inline float  ePI<float>     () { return FLOAT_PI_VALUE;     }
template<>            inline double ePI<double>    () { return DOUBLE_PI_VALUE;    }

template<typename T>  inline T      e2PI           () { return (T)(2.0 * DOUBLE_PI_VALUE); }
template<>            inline float  e2PI<float>    () { return (2.0f * FLOAT_PI_VALUE);    }
template<>            inline double e2PI<double>   () { return (2.0 * DOUBLE_PI_VALUE);    }

template<typename T>  inline T      ePI2           () { return (T)(0.5 * DOUBLE_PI_VALUE); }
template<>            inline float  ePI2<float>    () { return (0.5f * FLOAT_PI_VALUE);    }
template<>            inline double ePI2<double>   () { return (0.5 * DOUBLE_PI_VALUE);    }

template<typename T>  inline T      eInPI          () { return (T)(1.0 / DOUBLE_PI_VALUE); }
template<>            inline float  eInPI<float>   () { return (1.0f / FLOAT_PI_VALUE);    }
template<>            inline double eInPI<double>  () { return (1.0 / DOUBLE_PI_VALUE);    }

template<typename T>  inline T      eIn2PI         () { return (T)(1.0 / (2.0 * DOUBLE_PI_VALUE)); }
template<>            inline float  eIn2PI<float>  () { return (1.0f / (2.0f * FLOAT_PI_VALUE));   }
template<>            inline double eIn2PI<double> () { return (1.0 / (2.0 * DOUBLE_PI_VALUE));    }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CONSTANTES DE PI CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const float ePIf    = FLOAT_PI_VALUE;
static const float e2PIf   = 2.0f * ePIf;
static const float ePI2f   = 0.5f * ePIf;
static const float eInPIf  = 1.0f / ePIf;
static const float eIn2PIf = 1.0f / e2PIf;

static const double ePId    = DOUBLE_PI_VALUE;
static const double e2PId   = 2.0 * ePId;
static const double ePI2d   = 0.5 * ePId;
static const double eInPId  = 1.0 / ePId;
static const double eIn2PId = 1.0 / e2PId;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE MATRICES [3x3] CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> class Matrix33;
typedef Matrix33<float>   Mat33f;
typedef Matrix33<double>  Mat33d;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE MATRICES [3x4] CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Matrix34;
typedef Matrix34<float>   Mat34f;
typedef Matrix34<double>  Mat34d;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE MATRICES [4x4] CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Matrix44;
typedef Matrix44<float>   Mat44f;
typedef Matrix44<double>  Mat44d;
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE CUATERNIONES CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Quatn;
typedef Quatn<float>  Quatf;
typedef Quatn<double> Quatd;
typedef Quatn<int32>  Quati;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE CUATERNION+POSICION CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class QuatPos;
typedef QuatPos<float>  Transformf;
typedef QuatPos<double> Transformd;
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE VECTORES 2D CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Vector2;
typedef Vector2<float>  Vect2f;
typedef Vector2<double> Vect2d;
typedef Vector2<int32>  Vect2i;
typedef Vector2<uint32> Vect2u;
typedef Vector2<uint16> Vect2w;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE VECTORES 3D CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Vector3;
typedef Vector3<float>  Vect3f;
typedef Vector3<double> Vect3d;
typedef Vector3<uint16> Vect3w;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE VECTORES 4D CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Vector4;
typedef Vector4<float>  Vect4f;
typedef Vector4<double> Vect4d;
typedef Vector4<int32>  Vect4i;
typedef Vector4<uint8>  Vect4u8;


#endif
