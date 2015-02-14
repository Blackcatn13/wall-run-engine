#include "Math/Matrix33.h"
#include "Math/Matrix34.h"
#include "Math/Matrix44.h"
#include "Math\Color.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Constantes númericas de los distintos tipos de la librería matemática
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------
// CColor
//---------------------------------------------------------------------------------------------------------------------
const CColor colBLACK   (0.0f, 0.0f, 0.0f);
const CColor colWHITE   (1.0f, 1.0f, 1.0f);
const CColor colRED     (1.0f, 0.0f, 0.0f);
const CColor colGREEN   (0.0f, 1.0f, 0.0f);
const CColor colBLUE    (0.0f, 0.0f, 1.0f);
const CColor colMAGENTA (1.0f, 0.0f, 1.0f);
const CColor colYELLOW  (1.0f, 1.0f, 0.0f);
const CColor colCYAN    (0.0f, 1.0f, 1.0f);


//------------
// Matrix33
//---------------------------------------------------------------------------------------------------------------------

// CONSTANTES FLOAT
const Mat33f  m33fIDENTITY  ( 1.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 1.0f );

const Mat33f  m33fZERO      ( 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f );

const Mat33f  m33fONES      ( 1.0f, 1.0f, 1.0f,
                              1.0f, 1.0f, 1.0f,
                              1.0f, 1.0f, 1.0f );

// CONSTANTES DOUBLE
const Mat33d  m33dIDENTITY  ( 1.0, 0.0, 0.0,
                              0.0, 1.0, 0.0,
                              0.0, 0.0, 1.0 );

const Mat33d  m33dZERO      ( 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0 );

const Mat33d  m33dONES      ( 1.0, 1.0, 1.0,
                              1.0, 1.0, 1.0,
                              1.0, 1.0, 1.0 );

//------------
// Matrix34
//---------------------------------------------------------------------------------------------------------------------
// FLOAT
const Mat34f  m34fIDENTITY  ( 1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f );

const Mat34f  m34fZERO      ( 0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f );

const Mat34f  m34fONES      ( 1.0f, 1.0f, 1.0f, 1.0f,
                              1.0f, 1.0f, 1.0f, 1.0f,
                              1.0f, 1.0f, 1.0f, 1.0f );

// DOUBLE
const Mat34d  m34dIDENTITY  ( 1.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0 );

const Mat34d  m34dZERO      ( 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0 );

const Mat34d  m34dONES      ( 1.0, 1.0, 1.0, 1.0,
                              1.0, 1.0, 1.0, 1.0,
                              1.0, 1.0, 1.0, 1.0 );

//-------------
// Matrix44
//---------------------------------------------------------------------------------------------------------------------
// FLOAT
const Mat44f  m44fIDENTITY  ( 1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f );

const Mat44f  m44fZERO      ( 0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f );

const Mat44f  m44fONES      ( 1.0f, 1.0f, 1.0f, 1.0f,
                              1.0f, 1.0f, 1.0f, 1.0f,
                              1.0f, 1.0f, 1.0f, 1.0f,
                              1.0f, 1.0f, 1.0f, 1.0f );

//  CONSTANTES DOUBLE
const Mat44d  m44dIDENTITY  ( 1.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0 );

const Mat44d  m44dZERO      ( 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0 );

const Mat44d  m44dONES      ( 1.0, 1.0, 1.0, 1.0,
                              1.0, 1.0, 1.0, 1.0,
                              1.0, 1.0, 1.0, 1.0,
                              1.0, 1.0, 1.0, 1.0 );
/*
//---------
// Quatn
//---------------------------------------------------------------------------------------------------------------------
// FLOAT
const Quatf qfIDENTITY (0.0f, 0.0f, 0.0f, 1.0f);
const Quatf qfINVALID  (FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

// DOUBLE
const Quatd qdIDENTITY (0.0, 0.0, 0.0, 1.0);
const Quatd qdINVALID  (DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX);

//-----------
// QuatPos
//---------------------------------------------------------------------------------------------------------------------
// FLOAT
const Transformf quatposfIDENTITY (0.0f, 0.0f, 0.0f, 1.0f,
                                   0.0f, 0.0f, 0.0f);
// DOUBLE
const Transformd quatposdIDENTITY (0.0, 0.0, 0.0, 1.0,
                                   0.0, 0.0, 0.0);
*/
//-----------
// Vector2
//---------------------------------------------------------------------------------------------------------------------
// FLOAT
const Vect2f  v2fX       ( 1.0f, 0.0f);
const Vect2f  v2fY       ( 0.0f, 1.0f);
const Vect2f  v2fONE     ( 1.0f, 1.0f);
const Vect2f  v2fNEGX    (-1.0f, 0.0f);
const Vect2f  v2fNEGY    ( 0.0f, -1.0f);
const Vect2f  v2fNEGONE  (-1.0f, -1.0f);
const Vect2f  v2fZERO    ( 0.0f, 0.0f);
const Vect2f  v2fMAX     ( FLT_MAX, FLT_MAX);
const Vect2f  v2fMIN     (-FLT_MAX, -FLT_MAX);
const Vect2f  v2fFRONT   ( 0.0f, 1.0f);
const Vect2f  v2fRIGHT   ( 1.0f, 0.0f);
const Vect2f  v2fUNIT    ( 1.0f, 1.0f);

// DOUBLE
const Vect2d  v2dX       ( 1.0, 0.0);
const Vect2d  v2dY       ( 0.0, 1.0);
const Vect2d  v2dONE     ( 1.0, 1.0);
const Vect2d  v2dNEGX    (-1.0, 0.0);
const Vect2d  v2dNEGY    ( 0.0, -1.0);
const Vect2d  v2dNEGONE  (-1.0, -1.0);
const Vect2d  v2dZERO    ( 0.0, 0.0);
const Vect2d  v2dMAX     ( DBL_MAX, DBL_MAX);
const Vect2d  v2dMIN     (-DBL_MAX, -DBL_MAX);
const Vect2d  v2dTOP     ( 0.0, 0.0);
const Vect2d  v2dFRONT   ( 0.0, 1.0);
const Vect2d  v2dRIGHT   ( 1.0, 0.0);
const Vect2d  v2dUNIT    ( 1.0, 1.0);

// INT
const Vect2i  v2iX       ( 1, 0);
const Vect2i  v2iY       ( 0, 1);
const Vect2i  v2iONE     ( 1, 1);
const Vect2i  v2iNEGX    (-1, 0);
const Vect2i  v2iNEGY    ( 0, -1);
const Vect2i  v2iNEGONE  (-1, -1);
const Vect2i  v2iZERO    ( 0, 0);
const Vect2i  v2iTOP     ( 0, 0);
const Vect2i  v2iFRONT   ( 0, 1);
const Vect2i  v2iRIGHT   ( 1, 0);
const Vect2i  v2iUNIT    ( 1, 1);

//-----------
// Vector3
//---------------------------------------------------------------------------------------------------------------------
// FLOAT
const Vect3f  v3fX       ( 1.0f, 0.0f, 0.0f);
const Vect3f  v3fY       ( 0.0f, 1.0f, 0.0f);
const Vect3f  v3fZ       ( 0.0f, 0.0f, 1.0f);
const Vect3f  v3fONE     ( 1.0f, 1.0f, 1.0f);
const Vect3f  v3fNEGX    (-1.0f, 0.0f, 0.0f);
const Vect3f  v3fNEGY    ( 0.0f, -1.0f, 0.0f);
const Vect3f  v3fNEGZ    ( 0.0f, 0.0f, -1.0f);
const Vect3f  v3fNEGONE  (-1.0f, -1.0f, -1.0f);
const Vect3f  v3fZERO    ( 0.0f, 0.0f, 0.0f);
const Vect3f  v3fMAX     ( FLT_MAX, FLT_MAX, FLT_MAX);
const Vect3f  v3fMIN     (-FLT_MAX, -FLT_MAX, -FLT_MAX);
const Vect3f  v3fTOP     ( 0.0f, 0.0f, 1.0f);
const Vect3f  v3fBOTTOM  (0.0f, 0.0f, -1.0f);
const Vect3f  v3fFRONT   ( 0.0f, 1.0f, 0.0f);
const Vect3f  v3fRIGHT   ( 1.0f, 0.0f, 0.0f);
const Vect3f  v3fUNIT    ( 1.0f, 1.0f, 1.0f);

// DOUBLE
const Vect3d  v3dX       ( 1.0, 0.0, 0.0);
const Vect3d  v3dY       ( 0.0, 1.0, 0.0);
const Vect3d  v3dZ       ( 0.0, 0.0, 1.0);
const Vect3d  v3dONE     ( 1.0, 1.0, 1.0);
const Vect3d  v3dNEGX    (-1.0, 0.0, 0.0);
const Vect3d  v3dNEGY    ( 0.0, -1.0, 0.0);
const Vect3d  v3dNEGZ    ( 0.0, 0.0, -1.0);
const Vect3d  v3dNEGONE  (-1.0, -1.0, -1.0);
const Vect3d  v3dZERO    ( 0.0, 0.0, 0.0);
const Vect3d  v3dMAX     ( DBL_MAX, DBL_MAX, DBL_MAX);
const Vect3d  v3dMIN     (-DBL_MAX, -DBL_MAX, -DBL_MAX);
const Vect3d  v3dTOP     ( 0.0, 0.0, 1.0);
const Vect3d  v3dFRONT   ( 0.0, 1.0, 0.0);
const Vect3d  v3dRIGHT   ( 1.0, 0.0, 0.0);
const Vect3d  v3dUNIT    ( 1.0, 1.0, 1.0);

// INT
const Vect3i  v3iX       ( 1, 0, 0);
const Vect3i  v3iY       ( 0, 1, 0);
const Vect3i  v3iZ       ( 0, 0, 1);
const Vect3i  v3iONE     ( 1, 1, 1);
const Vect3i  v3iNEGX    (-1, 0, 0);
const Vect3i  v3iNEGY    ( 0, -1, 0);
const Vect3i  v3iNEGZ    ( 0, 0, -1);
const Vect3i  v3iNEGONE  (-1, -1, -1);
const Vect3i  v3iZERO    ( 0, 0, 0);
const Vect3i  v3iTOP     ( 0, 0, 1);
const Vect3i  v3iFRONT   ( 0, 1, 0);
const Vect3i  v3iRIGHT   ( 1, 0, 0);
const Vect3i  v3iUNIT    ( 1, 1, 1);

//------------
// Vector4
//---------------------------------------------------------------------------------------------------------------------
// FLOAT
const Vect4f  v4fX       ( 1.0f, 0.0f, 0.0f, 0.0f);
const Vect4f  v4fY       ( 0.0f, 1.0f, 0.0f, 0.0f);
const Vect4f  v4fZ       ( 0.0f, 0.0f, 1.0f, 0.0f);
const Vect4f  v4fW       ( 0.0f, 0.0f, 0.0f, 1.0f);
const Vect4f  v4fONE     ( 1.0f, 1.0f, 1.0f, 1.0f);
const Vect4f  v4fNEGX    (-1.0f, 0.0f, 0.0f, 0.0f);
const Vect4f  v4fNEGY    ( 0.0f, -1.0f, 0.0f, 0.0f);
const Vect4f  v4fNEGZ    ( 0.0f, 0.0f, -1.0f, 0.0f);
const Vect4f  v4fNEGW    ( 0.0f, 0.0f, 0.0f, -1.0f);
const Vect4f  v4fNEGONE  (-1.0f, -1.0f, -1.0f, -1.0f);
const Vect4f  v4fZERO    ( 0.0f, 0.0f, 0.0f, 0.0f);
const Vect4f  v4fMAX     ( FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
const Vect4f  v4fMIN     (-FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX);
const Vect4f  v4fTOP     ( 0.0f, 0.0f, 1.0f, 1.0f);
const Vect4f  v4fFRONT   ( 0.0f, 1.0f, 0.0f, 1.0f);
const Vect4f  v4fRIGHT   ( 1.0f, 0.0f, 0.0f, 1.0f);

// DOUBLE
const Vect4d  v4dX       ( 1.0, 0.0, 0.0, 0.0);
const Vect4d  v4dY       ( 0.0, 1.0, 0.0, 0.0);
const Vect4d  v4dZ       ( 0.0, 0.0, 1.0, 0.0);
const Vect4d  v4dW       ( 0.0, 0.0, 0.0, 1.0);
const Vect4d  v4dONE     ( 1.0, 1.0, 1.0, 1.0);
const Vect4d  v4dNEGX    (-1.0, 0.0, 0.0, 0.0);
const Vect4d  v4dNEGY    ( 0.0, -1.0, 0.0, 0.0);
const Vect4d  v4dNEGZ    ( 0.0, 0.0, -1.0, 0.0);
const Vect4d  v4dNEGW    ( 0.0, 0.0, 0.0, -1.0);
const Vect4d  v4dNEGONE  (-1.0, -1.0, -1.0, -1.0);
const Vect4d  v4dZERO    ( 0.0, 0.0, 0.0, 0.0);
const Vect4d  v4dMAX     ( DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX);
const Vect4d  v4dMIN     (-DBL_MAX, -DBL_MAX, -DBL_MAX, DBL_MAX);
const Vect4d  v4dTOP     ( 0.0, 0.0, 1.0, 1.0);
const Vect4d  v4dFRONT   ( 0.0, 1.0, 0.0, 1.0);
const Vect4d  v4dRIGHT   ( 1.0, 0.0, 0.0, 1.0);
/*
//-------
// OBox
//---------------------------------------------------------------------------------------------------------------------
const OBox  obNULL (v3fZERO, v3fX, v3fY, v3fZ, v3fZERO);
const OBox  obBIG  (v3fZERO, v3fX, v3fY, v3fZ, v3fMAX);
const OBox  obUNIT (v3fZERO, v3fX, v3fY, v3fZ, v3fUNIT);

*/