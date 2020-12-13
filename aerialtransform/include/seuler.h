// +AMGD        -*-c++-*-
// $Id: seuler.h 4 2012-08-24 02:28:17Z mike $
// file:  seuler.h
// author:  MJ Pfauth
//    Created.  18 May 2003
//    Re-wrote as templates.  6 Dec 03.
//    19 Aug 05:  MJ Pfauth, re-named to SEuler

// Copyright 2003 to 20012 Michael J. Pfauth, mpfauth@hiwaay.net.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef S_EULER_H
#define S_EULER_H

namespace SEuler 
{

const double M_PII = 3.1415926535897932384626433832795;

template<const int v>
struct euler_seq 
{
  enum { val = v };
};


// Euler angle sequences
const int YAW_PITCH_ROLL   = 1;
const int YAW_ROLL_PITCH   = 2;
const int PITCH_YAW_ROLL   = 3;
const int PITCH_ROLL_YAW   = 4;
const int ROLL_YAW_PITCH   = 5;
const int ROLL_PITCH_YAW   = 6;
const int YAW_PITCH_YAW    = 7;
const int YAW_ROLL_YAW     = 8;
const int PITCH_YAW_PITCH  = 9;
const int PITCH_ROLL_PITCH = 10;
const int ROLL_YAW_ROLL    = 11;
const int ROLL_PITCH_ROLL  = 12;


// Use these as arguments
typedef euler_seq<YAW_PITCH_ROLL>   YPR_SEQUENCE;
typedef euler_seq<YAW_ROLL_PITCH>   YRP_SEQUENCE;
typedef euler_seq<PITCH_YAW_ROLL>   PYR_SEQUENCE;
typedef euler_seq<PITCH_ROLL_YAW>   PRY_SEQUENCE;
typedef euler_seq<ROLL_YAW_PITCH>   RYP_SEQUENCE;
typedef euler_seq<ROLL_PITCH_YAW>   RPY_SEQUENCE;
typedef euler_seq<YAW_PITCH_YAW>    YPY_SEQUENCE;
typedef euler_seq<YAW_ROLL_YAW>     YRY_SEQUENCE;
typedef euler_seq<PITCH_YAW_PITCH>  PYP_SEQUENCE;
typedef euler_seq<PITCH_ROLL_PITCH> PRP_SEQUENCE;
typedef euler_seq<ROLL_YAW_ROLL>    RYR_SEQUENCE;
typedef euler_seq<ROLL_PITCH_ROLL>  RPR_SEQUENCE;


// All angle parameters (execpt for DegToRad) in radians.



// Deg-Radian conversion

template <typename T>
T 
DegToRad(
    const T deg);

template <typename T>
T 
RadToDeg(
    const T rad);



// Primitive rotations.

template <typename T>
void 
makeYawMatrix(
    const T yaw, 
    T M[3][3]);

template <typename T>
void 
makePitchMatrix(
    const T pitch, 
    T M[3][3]);

template <typename T>
void 
makeRollMatrix(
    const T roll, 
    T M[3][3]);



// Primary Euler angle sequences

template <typename T>
void 
makeRotMatrix(
    euler_seq<YAW_PITCH_ROLL> seq, 
    const T yaw, 
    const T pitch, 
    const T roll, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<YAW_ROLL_PITCH> seq, 
    const T yaw, 
    const T roll, 
    const T pitch, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<PITCH_YAW_ROLL> seq, 
    const T pitch, 
    const T yaw, 
    const T roll, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<PITCH_ROLL_YAW> seq, 
    const T pitch, 
    const T roll, 
    const T yaw, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<ROLL_YAW_PITCH> seq, 
    const T roll, 
    const T yaw, 
    const T pitch, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<ROLL_PITCH_YAW> seq, 
    const T roll, 
    const T pitch, 
    const T yaw, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<YAW_PITCH_YAW> seq, 
    const T yaw1, 
    const T pitch, 
    const T yaw2, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<YAW_ROLL_YAW> seq, 
    const T yaw1, 
    const T roll, 
    const T yaw2, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<PITCH_YAW_PITCH> seq, 
    const T pitch1, 
    const T yaw, 
    const T pitch, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<PITCH_ROLL_PITCH> seq, 
    const T pitch1, 
    const T roll, 
    const T pitch2, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<ROLL_YAW_ROLL> seq, 
    const T roll1, 
    const T yaw, 
    const T roll2, 
    T RotMatrix[3][3]);

template <typename T>
void 
makeRotMatrix(
    euler_seq<ROLL_PITCH_ROLL> seq, 
    const T roll1, 
    const T pitch, 
    const T roll2, 
    T RotMatrix[3][3]);



// Get Euler angles from a rotation matrix

template <typename T>
void 
getEulerAngles(
    euler_seq<YAW_PITCH_ROLL> seq,
    const T RotMatrix[3][3],
    T& yaw, 
    T& pitch, 
    T& roll);

template <typename T>
void 
getEulerAngles(
    euler_seq<YAW_ROLL_PITCH> seq,
    const T RotMatrix[3][3],
    T& yaw, 
    T& roll, 
    T& pitch);

template <typename T>
void 
getEulerAngles(
    euler_seq<PITCH_YAW_ROLL> seq,
    const T RotMatrix[3][3],
    T& pitch,
    T& yaw, 
    T& roll); 

template <typename T>
void 
getEulerAngles(
    euler_seq<PITCH_ROLL_YAW> seq,
    const T RotMatrix[3][3],
    T& pitch,
    T& roll, 
    T& yaw); 

template <typename T>
void 
getEulerAngles(
    euler_seq<ROLL_PITCH_YAW> seq,
    const T RotMatrix[3][3],
    T& roll,
    T& pitch, 
    T& yaw); 

template <typename T>
void 
getEulerAngles(
    euler_seq<ROLL_YAW_PITCH> seq,
    const T RotMatrix[3][3],
    T& roll,
    T& yaw, 
    T& pitch); 

template <typename T>
void 
getEulerAngles(
    euler_seq<YAW_PITCH_YAW> seq,
    const T RotMatrix[3][3],
    T& yaw1,
    T& pitch, 
    T& yaw2); 

template <typename T>
void 
getEulerAngles(
    euler_seq<YAW_ROLL_YAW> seq,
    const T RotMatrix[3][3],
    T& yaw1,
    T& roll, 
    T& yaw2); 

template <typename T>
void 
getEulerAngles(
    euler_seq<PITCH_YAW_PITCH> seq,
    const T RotMatrix[3][3],
    T& pitch1,
    T& yaw, 
    T& pitch2); 

template <typename T>
void 
getEulerAngles(
    euler_seq<PITCH_ROLL_PITCH> seq,
    const T RotMatrix[3][3],
    T& pitch1,
    T& roll, 
    T& pitch2); 


template <typename T>
void 
getEulerAngles(
    euler_seq<ROLL_YAW_ROLL> seq,
    const T RotMatrix[3][3],
    T& roll1,
    T& yaw, 
    T& roll2);

template <typename T>
void 
getEulerAngles(
    euler_seq<ROLL_PITCH_ROLL> seq,
    const T RotMatrix[3][3],
    T& roll1,
    T& pitch, 
    T& roll2);

} // namespace SEuler

#include "seuler.hpp"

#endif
