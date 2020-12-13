// +AMGD      -*-c++-*-
// $Id: smatrix.h 4 2012-08-24 02:28:17Z mike $
// file:  smatrix.h
// author:  MJ Pfauth, 17 May 2003
//   5 Dec 03.  Re-wrote as templates.
//   19 Aug 05.  Renamed files and functions to smatrix.
//
// Notes
// 1.  matrix multiplications are right-side.

// Copyright 2003, 2004, 2005 Michael J. Pfauth, mpfauth@hiwaay.net.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef S_MATRIX_H
#define S_MATRIX_H

namespace SMatrix {

// P = M1 * M2
template <typename T>
void 
mtxMult(
    const T M1[3][3], 
    const T M2[3][3], 
    T P[3][3]);

// P = M1 * M2
// M2, P are column vectors
template <typename T>
void 
mtxMult(
    const T M1[3][3], 
    const T M2[3], 
    T P[3]);

// P = M1 * M2
// M1, P are row vectors
template <typename T>
void 
mtxMult(
    const T M1[3], 
    const T M2[3][3], 
    T P[3]);

// P = k * M
template <typename T>
void 
mtxMult(
    const T k, 
    const T M[3], 
    T P[3]);

// P = k * M
template <typename T>
void 
mtxMult(
    const T k, 
    const T M[3][3], 
    T P[3][3]);

// P = M1 + M2
template <typename T>
void 
mtxAdd(
    const T M1[3][3], 
    const T M2[3][3], 
    T P[3][3]);

// P = M1 + M2
// M1, M2, P are all either row or column matrices.
template <typename T>
void 
mtxAdd(
    const T M1[3], 
    const T M2[3], 
    T P[3]);

// P = M1 - M2
template <typename T>
void 
mtxSubtract(
    const T M1[3][3], 
    const T M2[3][3], 
    T P[3][3]);

// P = M1 - M2
template <typename T>
void 
mtxSubtract(
    const T M1[3], 
    const T M2[3], 
    T P[3]);

// transpose
template <typename T>
void 
mtxTranspose(
    T M[3][3]);

// Pack
template <typename T>
void 
mtxPack(
    const T M[3][3], 
    T P[9]);

// Unpack
template <typename T>
void 
mtxUnpack(
    const T M[9], 
    T P[3][3]);

// Print
template <typename T>
void 
Print(
    const T M[3]);

// Print
template <typename T>
void 
Print(
    const T M[3][3]);

// Some common vector functions.

// S = V1 + V2
template <typename T>
void 
vecAdd (
    const T V1[3], 
    const T V2[3], 
    T S[3]);

// D = V1 - V2
template <typename T>
void 
vecSubtract (
    const T V1[3], 
    const T V2[3], 
    T D[3]);

// V1 dot V2, not normalized
template <typename T>
T 
vecDot (
    const T V1[3], 
    const T V2[3]);

// P = V1 cross V2, not normalized
template <typename T>
void 
vecCross (
    const T V1[3], 
    const T V2[3], 
    T P[3]);

// Magnitude of a vector.
template <typename T>
T 
vecMagnitude (
    const T V[3]);

// Normalize a vector
template <typename T>
void 
vecNorm (
    T V[3]);

}

#include "smatrix.hpp"

#endif
