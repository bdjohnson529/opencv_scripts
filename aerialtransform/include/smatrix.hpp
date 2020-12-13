// $Id: smatrix.hpp 4 2012-08-24 02:28:17Z mike $
// file:  smatrix.cpp
// author:  MJ Pfauth, 17 May 2003
//    5 Dec 03.  Re-wrote as templates.
//   19 Aug 05.  Renamed files and functions to smatrix.

// Copyright 2003, 2004, 2005 Michael J. Pfauth, mpfauth@hiwaay.net.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <algorithm>
#include <cmath>
#include <iostream>


using std::swap;

namespace SMatrix {

#ifndef sq
#define sq(a) ((a)*(a))
#endif

template <typename T>
void 
mtxMult(
    const T M1[3][3], 
    const T M2[3][3], 
    T P[3][3])
{
    P[0][0] = 
	M1[0][0] * M2[0][0] +
	M1[0][1] * M2[1][0] +
	M1[0][2] * M2[2][0];

    P[0][1] =
	M1[0][0] * M2[0][1] +
	M1[0][1] * M2[1][1] +
	M1[0][2] * M2[2][1];

    P[0][2] =
	M1[0][0] * M2[0][2] +
	M1[0][1] * M2[1][2] +
	M1[0][2] * M2[2][2];

    P[1][0] =
	M1[1][0] * M2[0][0] +
	M1[1][1] * M2[1][0] +
	M1[1][2] * M2[2][0];

    P[1][1] =
	M1[1][0] * M2[0][1] +
	M1[1][1] * M2[1][1] +
	M1[1][2] * M2[2][1];

    P[1][2] =
	M1[1][0] * M2[0][2] +
	M1[1][1] * M2[1][2] +
	M1[1][2] * M2[2][2];

    P[2][0] =
	M1[2][0] * M2[0][0] +
	M1[2][1] * M2[1][0] +
	M1[2][2] * M2[2][0];

    P[2][1] =
	M1[2][0] * M2[0][1] +
	M1[2][1] * M2[1][1] +
	M1[2][2] * M2[2][1];

    P[2][2] =
	M1[2][0] * M2[0][2] +
	M1[2][1] * M2[1][2] +
	M1[2][2] * M2[2][2];

}

// P = M1 * M2
template <typename T>
void 
mtxMult(
    const T M1[3][3], 
    const T M2[3], 
    T P[3])
{
    P[0] =
	M1[0][0] * M2[0] +
	M1[0][1] * M2[1] +
	M1[0][2] * M2[2];

    P[1] =
	M1[1][0] * M2[0] +
	M1[1][1] * M2[1] +
	M1[1][2] * M2[2];

    P[2] =
	M1[2][0] * M2[0] +
	M1[2][1] * M2[1] +
	M1[2][2] * M2[2];

}

// P = M1 * M2
template <typename T>
void 
mtxMult(
    const T M1[3], 
    const T M2[3][3], 
    T P[3])
{
    P[0] = 
	M1[0] * M2[0][0] +
	M1[1] * M2[1][0] +
	M1[2] * M2[2][0];

    P[1] = 
	M1[0] * M2[0][1] +
	M1[1] * M2[1][1] +
	M1[2] * M2[2][1];

    P[2] = 
	M1[0] * M2[0][2] +
	M1[1] * M2[1][2] +
	M1[2] * M2[2][2];

}

// P = k * M
template <typename T>
void 
mtxMult(
    const T k, 
    const T M[3], 
    T P[3])
{
    P[0] = k * M[0];
    P[1] = k * M[1];
    P[2] = k * M[2];
}

// P = k * M
template <typename T>
void 
mtxMult(
    const T k, 
    const T M[3][3], 
    T P[3][3])
{
    P[0][0] = k * M[0][0];
    P[0][1] = k * M[0][1];
    P[0][2] = k * M[0][2];

    P[1][0] = k * M[1][0];
    P[1][1] = k * M[1][1];
    P[1][2] = k * M[1][2];

    P[2][0] = k * M[2][0];
    P[2][1] = k * M[2][1];
    P[2][2] = k * M[2][2];
}

// P = M1 + M2
template <typename T>
void 
mtxAdd(
    const T M1[3][3], 
    const T M2[3][3], 
    T P[3][3])
{
    P[0][0] = M1[0][0] + M2[0][0];
    P[0][1] = M1[0][1] + M2[0][1];
    P[0][2] = M1[0][2] + M2[0][2];

    P[1][0] = M1[1][0] + M2[1][0];
    P[1][1] = M1[1][1] + M2[1][1];
    P[1][2] = M1[1][2] + M2[1][2];

    P[2][0] = M1[2][0] + M2[2][0];
    P[2][1] = M1[2][1] + M2[2][1];
    P[2][2] = M1[2][2] + M2[2][2];
}

// P = M1 + M2
template <typename T>
void 
mtxAdd(
    const T M1[3], 
    const T M2[3], 
    T P[3])
{
    P[0] = M1[0] + M2[0];
    P[1] = M1[1] + M2[1];
    P[2] = M1[2] + M2[2];
}

// P = M1 - M2
template <typename T>
void 
mtxSubtract(
    const T M1[3][3], 
    const T M2[3][3], 
    T P[3][3])
{
    P[0][0] = M1[0][0] - M2[0][0];
    P[0][1] = M1[0][1] - M2[0][1];
    P[0][2] = M1[0][2] - M2[0][2];

    P[1][0] = M1[1][0] - M2[1][0];
    P[1][1] = M1[1][1] - M2[1][1];
    P[1][2] = M1[1][2] - M2[1][2];

    P[2][0] = M1[2][0] - M2[2][0];
    P[2][1] = M1[2][1] - M2[2][1];
    P[2][2] = M1[2][2] - M2[2][2];
}

// P = M1 - M2
template <typename T>
void 
mtxSubtract(
    const T M1[3], 
    const T M2[3], 
    T P[3])
{
    P[0] = M1[0] - M2[0];
    P[1] = M1[1] - M2[1];
    P[2] = M1[2] - M2[2];
}

// transpose
template <typename T>
void 
mtxTranspose(
    T M[3][3])
{
    swap(M[1][0], M[0][1]);
    swap(M[2][0], M[0][2]);
    swap(M[2][1], M[1][2]);
}

// Pack
template <typename T>
void 
mtxPack(
    const T M[3][3], 
    T P[9])
{
    P[0] = M[0][0];
    P[1] = M[0][1];
    P[2] = M[0][2];

    P[3] = M[1][0];
    P[4] = M[1][1];
    P[5] = M[1][2];

    P[6] = M[2][0];
    P[7] = M[2][1];
    P[8] = M[2][2];
}

// Unpack
template <typename T>
void 
mtxUnpack(
    const T M[9], 
    T P[3][3])
{
    P[0][0] = M[0];
    P[0][1] = M[1];
    P[0][2] = M[2];

    P[1][0] = M[3];
    P[1][1] = M[4];
    P[1][2] = M[5];

    P[2][0] = M[6];
    P[2][1] = M[7];
    P[2][2] = M[8];
}

// Print
template <typename T>
void 
Print(
    const T M[3])
{
    std::cout << "[" << M[0] << ", " << M[1] << ", " << M[2] << "]\n";
}

// Print
template <typename T>
void 
Print(
    const T M[3][3])
{
    Print(M[0]);
    Print(M[1]);
    Print(M[2]);
}

// S = V1 + V2
template <typename T>
void 
vecAdd (
    const T V1[3], 
    const T V2[3], 
    T S[3])
{
    S[0] = V1[0] + V2[0];
    S[1] = V1[1] + V2[1];
    S[2] = V1[2] + V2[2];
}

// D = V1 - V2
template <typename T>
void 
vecSubtract (
    const T V1[3], 
    const T V2[3], 
    T D[3])
{
    D[0] = V1[0] - V2[0];
    D[1] = V1[1] - V2[1];
    D[2] = V1[2] - V2[2];
}

// V1 dot V2, not normalized
template <typename T>
T 
vecDot (
    const T V1[3], 
    const T V2[3])
{
    return V1[0] * V2[0] + V1[1] * V2[1] + V1[2] * V2[2];

}

// P = V1 cross V2, not normalized
template <typename T>
void 
vecCross (
    const T V1[3], 
    const T V2[3], 
    T P[3])
{
    P[0] = V1[1] * V2[2] - V1[2] * V2[1];
    P[1] = V1[2] * V2[0] - V1[0] * V2[2];
    P[2] = V1[0] * V2[1] - V1[1] * V2[0];
}

// Magnitude of a vector.
template <typename T>
T 
vecMagnitude (
    const T V[3])
{
    return sqrt ((sq(V[0]) + sq(V[1]) + sq(V[2])));
}

// Normalize a vector
template <typename T>
void 
vecNorm (
    T V[3])
{
    T Magnitude = vecMagnitude (V);

    V[0] /= Magnitude;
    V[1] /= Magnitude;
    V[2] /= Magnitude;
}

} // end namespace SMatrix
