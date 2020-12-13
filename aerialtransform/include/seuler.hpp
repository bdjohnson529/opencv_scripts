// +AMGD   -*- c++ -*-
// $Id: seuler.hpp 4 2012-08-24 02:28:17Z mike $
// file:  seuler.cpp
// History:
//   18 May 2003.  Created, MJ Pfauth
//   6 Dec 03.  Re-wrote as templates.
//   19 Aug 05, MJ Pfauth, re-named to SEuler

// Copyright 2003 - 2009 Michael J. Pfauth, mpfauth@hiwaay.net.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <cmath>


namespace SEuler {

  template <typename T>
  T 
  DegToRad(
	   const T deg)
  {
    return deg * (M_PII / 180.0);
  }

  template <typename T>
  T 
  RadToDeg(
	   const T rad)
  {
    return rad * (180.0 / M_PII);
  }

  template <typename T>
  void 
  makeYawMatrix(
		const T yaw, 
		T M[3][3])
  {
    T sin_yaw = ::sin(yaw), cos_yaw = ::cos(yaw);

    M[0][0] = M[1][1] = cos_yaw;
    M[1][0] = -(M[0][1] = sin_yaw);
    M[0][2] = M[1][2] = M[2][0] = M[2][1] = 0.0;
    M[2][2] = 1.0;
  }

  template <typename T>
  void 
  makePitchMatrix(
		  const T pitch, 
		  T M[3][3])
  {
    T sin_pitch = ::sin(pitch), cos_pitch = ::cos(pitch);

    M[0][0] = M[2][2] = cos_pitch;
    M[0][2] = -(M[2][0] = sin_pitch);
    M[0][1] = M[1][0] = M[1][2] = M[2][1] = 0.0;
    M[1][1] = 1.0;
  }

  template <typename T>
  void 
  makeRollMatrix(
		 const T roll, 
		 T M[3][3])
  {
    T sin_roll = ::sin(roll), cos_roll = ::cos(roll);

    M[1][1] = M[2][2] = cos_roll;
    M[2][1] = -(M[1][2] = sin_roll);
    M[0][1] = M[0][2] = M[1][0] = M[2][0] = 0.0;
    M[0][0] = 1.0;
  }


  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<YAW_PITCH_ROLL> seq, 
		const T yaw, 
		const T pitch, 
		const T roll, 
		T YPR[3][3])
  {
    T sin_yaw = ::sin(yaw), cos_yaw = ::cos(yaw);
    T sin_pitch = ::sin(pitch), cos_pitch = ::cos(pitch);
    T sin_roll = ::sin(roll), cos_roll = ::cos(roll);

    YPR[0][0] = cos_pitch * cos_yaw;
    YPR[0][1] = cos_pitch * sin_yaw;
    YPR[0][2] = -sin_pitch;

    YPR[1][0] = sin_roll * sin_pitch * cos_yaw - cos_roll * sin_yaw;
    YPR[1][1] = sin_roll * sin_pitch * sin_yaw + cos_roll * cos_yaw;
    YPR[1][2] = sin_roll * cos_pitch;

    YPR[2][0] = cos_roll * sin_pitch * cos_yaw + sin_roll * sin_yaw;
    YPR[2][1] = cos_roll * sin_pitch * sin_yaw - sin_roll * cos_yaw;
    YPR[2][2] = cos_roll * cos_pitch;
  }

  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<YAW_ROLL_PITCH> seq, 
		const T yaw, 
		const T roll, 
		const T pitch, 
		T RotMatrix[3][3])
  {
    T sin_yaw = ::sin(yaw), cos_yaw = ::cos(yaw);
    T sin_pitch = ::sin(pitch), cos_pitch = ::cos(pitch);
    T sin_roll = ::sin(roll), cos_roll = ::cos(roll);

    RotMatrix[0][0] = cos_pitch * cos_yaw - 
      sin_roll * sin_pitch * sin_yaw;
    RotMatrix[0][1] = cos_pitch * sin_yaw + 
      sin_roll * sin_pitch * cos_yaw;
    RotMatrix[0][2] = -sin_pitch * cos_roll;

    RotMatrix[1][0] = -cos_roll * sin_yaw;
    RotMatrix[1][1] = cos_roll * cos_yaw;
    RotMatrix[1][2] = sin_roll;

    RotMatrix[2][0] = sin_pitch * cos_yaw + 
      sin_roll * cos_pitch * sin_yaw;
    RotMatrix[2][1] = sin_pitch * sin_yaw - 
      sin_roll * cos_pitch * cos_yaw;
    RotMatrix[2][2] = cos_roll * cos_pitch;
  }


  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<PITCH_YAW_ROLL> seq, 
		const T pitch, 
		const T yaw, 
		const T roll, 
		T RotMatrix[3][3])
  {
    T sin_yaw = ::sin(yaw), cos_yaw = ::cos(yaw);
    T sin_pitch = ::sin(pitch), cos_pitch = ::cos(pitch);
    T sin_roll = ::sin(roll), cos_roll = ::cos(roll);

    RotMatrix[0][0] = cos_yaw * cos_pitch;
    RotMatrix[0][1] = sin_yaw;
    RotMatrix[0][2] = -cos_yaw * sin_pitch;

    RotMatrix[1][0] = -cos_roll * sin_yaw * cos_pitch + 
      sin_roll * sin_pitch;
    RotMatrix[1][1] = cos_roll * cos_yaw;
    RotMatrix[1][2] = cos_roll * sin_yaw * sin_pitch + 
      sin_roll * cos_pitch;
 
    RotMatrix[2][0] = sin_roll * sin_yaw * cos_pitch + 
      cos_roll * sin_pitch;
    RotMatrix[2][1] = -sin_roll * cos_yaw;
    RotMatrix[2][2] = -sin_roll * sin_yaw * sin_pitch + 
      cos_roll * cos_pitch;
  }


  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<PITCH_ROLL_YAW> seq, 
		const T pitch, 
		const T roll, 
		const T yaw, 
		T RotMatrix[3][3])
  {
    T sin_yaw = ::sin(yaw), cos_yaw = ::cos(yaw);
    T sin_pitch = ::sin(pitch), cos_pitch = ::cos(pitch);
    T sin_roll = ::sin(roll), cos_roll = ::cos(roll);

    RotMatrix[0][0] = cos_yaw * cos_pitch + 
      sin_roll * sin_yaw * sin_pitch;
    RotMatrix[0][1] = cos_roll * sin_yaw;
    RotMatrix[0][2] = -cos_yaw * sin_pitch + 
      sin_roll * sin_yaw * cos_pitch;

    RotMatrix[1][0] = -sin_yaw * cos_pitch + 
      sin_roll * cos_yaw * sin_pitch;
    RotMatrix[1][1] = cos_roll * cos_yaw;
    RotMatrix[1][2] = sin_yaw * sin_pitch + 
      sin_roll * cos_yaw * cos_pitch;

    RotMatrix[2][0] = cos_roll * sin_pitch;
    RotMatrix[2][1] = -sin_roll;
    RotMatrix[2][2] = cos_roll * cos_pitch;
  }


  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<ROLL_YAW_PITCH> seq, 
		const T roll, 
		const T yaw, 
		const T pitch, 
		T RotMatrix[3][3])
  {
    T sin_yaw = ::sin(yaw), cos_yaw = ::cos(yaw);
    T sin_pitch = ::sin(pitch), cos_pitch = ::cos(pitch);
    T sin_roll = ::sin(roll), cos_roll = ::cos(roll);

    RotMatrix[0][0] = cos_yaw * cos_pitch;
    RotMatrix[0][1] = sin_yaw * cos_pitch * cos_roll + 
      sin_pitch * sin_roll;
    RotMatrix[0][2] = sin_roll * sin_yaw * cos_pitch - 
      cos_roll * sin_pitch;

    RotMatrix[1][0] = -sin_yaw;
    RotMatrix[1][1] = cos_roll * cos_yaw;
    RotMatrix[1][2] = sin_roll * cos_yaw;

    RotMatrix[2][0] = cos_yaw * sin_pitch;
    RotMatrix[2][1] = sin_yaw * sin_pitch * cos_roll - 
      cos_pitch * sin_roll;
    RotMatrix[2][2] = sin_roll * sin_yaw * sin_pitch + 
      cos_roll * cos_pitch;
  }


  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<ROLL_PITCH_YAW> seq, 
		const T roll, 
		const T pitch, 
		const T yaw, 
		T RotMatrix[3][3])
  {
    T sin_yaw = ::sin(yaw), cos_yaw = ::cos(yaw);
    T sin_pitch = ::sin(pitch), cos_pitch = ::cos(pitch);
    T sin_roll = ::sin(roll), cos_roll = ::cos(roll);

    RotMatrix[0][0] = cos_yaw * cos_pitch;
    RotMatrix[0][1] = sin_yaw * cos_roll + 
      cos_yaw * sin_pitch * sin_roll;
    RotMatrix[0][2] = sin_yaw * sin_roll - 
      cos_yaw * sin_pitch * cos_roll;

    RotMatrix[1][0] = -sin_yaw * cos_pitch;
    RotMatrix[1][1] = cos_yaw * cos_roll - 
      sin_yaw * sin_pitch * sin_roll;
    RotMatrix[1][2] = cos_yaw * sin_roll + 
      sin_yaw * sin_pitch * cos_roll;

    RotMatrix[2][0] = sin_pitch;
    RotMatrix[2][1] = -cos_pitch * sin_roll;
    RotMatrix[2][2] = cos_pitch * cos_roll;
  }


  template <typename T>
  void
  makeRotMatrix(
		euler_seq<YAW_PITCH_YAW> seq, 
		const T yaw1, 
		const T pitch, 
		const T yaw2, 
		T RotMatrix[3][3])
  {
    T sin_yaw1 = ::sin(yaw1), cos_yaw1 = ::cos(yaw1);
    T sin_pitch = ::sin(pitch), cos_pitch = ::cos(pitch);
    T sin_yaw2 = ::sin(yaw2), cos_yaw2 = ::cos(yaw2);

    RotMatrix[0][0] = cos_yaw2 * cos_pitch * cos_yaw1 - 
      sin_yaw2 * sin_yaw1;
    RotMatrix[0][1] = cos_yaw2 * cos_pitch * sin_yaw1 + 
      sin_yaw2 * cos_yaw1;
    RotMatrix[0][2] = -cos_yaw2 * sin_pitch;

    RotMatrix[1][0] = -sin_yaw2 * cos_pitch * cos_yaw1 - 
      cos_yaw2 * sin_yaw1;
    RotMatrix[1][1] = -sin_yaw2 * cos_pitch * sin_yaw1 + 
      cos_yaw2 * cos_yaw1;
    RotMatrix[1][2] = sin_yaw2 * sin_pitch;

    RotMatrix[2][0] = sin_pitch * cos_yaw1;
    RotMatrix[2][1] = sin_pitch * sin_yaw1;
    RotMatrix[2][2] = cos_pitch;
  }


  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<YAW_ROLL_YAW> seq, 
		const T yaw1,    
		const T roll, 
		const T yaw2, 
		T RotMatrix[3][3])
  {
    T sin_yaw1 = ::sin(yaw1), cos_yaw1 = ::cos(yaw1);
    T sin_roll = ::sin(roll), cos_roll = ::cos(roll);
    T sin_yaw2 = ::sin(yaw2), cos_yaw2 = ::cos(yaw2);

    RotMatrix[0][0] = cos_yaw2 * cos_yaw1 - 
      sin_yaw2 * cos_roll * sin_yaw1;
    RotMatrix[0][1] = cos_yaw2 * sin_yaw1 + 
      sin_yaw2 * cos_roll * cos_yaw1;
    RotMatrix[0][2] = sin_yaw2 * sin_roll;

    RotMatrix[1][0] = -sin_yaw2 * cos_yaw1 - 
      cos_yaw2 * cos_roll * sin_yaw1;
    RotMatrix[1][1] = -sin_yaw2 * sin_yaw1 + 
      cos_yaw2 * cos_roll * cos_yaw1;
    RotMatrix[1][2] = cos_yaw2 * sin_roll;

    RotMatrix[2][0] = sin_roll * sin_yaw1;
    RotMatrix[2][1] = -sin_roll * cos_yaw1;
    RotMatrix[2][2] = cos_roll;
  }


  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<PITCH_YAW_PITCH> seq, 
		const T pitch1, 
		const T yaw, 
		const T pitch2, 
		T RotMatrix[3][3])
  {
    T sin_pitch1 = ::sin(pitch1), cos_pitch1 = ::cos(pitch1);
    T sin_yaw = ::sin(yaw), cos_yaw = ::cos(yaw);
    T sin_pitch2 = ::sin(pitch2), cos_pitch2 = ::cos(pitch2);

    RotMatrix[0][0] = cos_pitch2 * cos_yaw * cos_pitch1 - 
      sin_pitch2 * sin_pitch1;
    RotMatrix[0][1] = cos_pitch2 * sin_yaw;
    RotMatrix[0][2] = -cos_pitch2 * cos_yaw * sin_pitch1 - 
      sin_pitch2 * cos_pitch1;

    RotMatrix[1][0] = -sin_yaw * cos_pitch1;
    RotMatrix[1][1] = cos_yaw;
    RotMatrix[1][2] = sin_yaw * sin_pitch1;

    RotMatrix[2][0] = sin_pitch2 * cos_yaw * cos_pitch1 + 
      cos_pitch2 * sin_pitch1;
    RotMatrix[2][1] = sin_pitch2 * sin_yaw;
    RotMatrix[2][2] = -sin_pitch2 * cos_yaw * sin_pitch1 + 
      cos_pitch2 * cos_pitch1;
  }


  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<PITCH_ROLL_PITCH> seq, 
		const T pitch1, 
		const T roll, 
		const T pitch2, 
		T RotMatrix[3][3])
  {
    T sin_pitch1 = ::sin(pitch1), cos_pitch1 = ::cos(pitch1);
    T sin_roll = ::sin(roll), cos_roll = ::cos(roll);
    T sin_pitch2 = ::sin(pitch2), cos_pitch2 = ::cos(pitch2);

    RotMatrix[0][0] = cos_pitch2 * cos_pitch1 - 
      sin_pitch2 * cos_roll * sin_pitch1;
    RotMatrix[0][1] = sin_pitch2 * sin_roll;
    RotMatrix[0][2] = -cos_pitch2 * sin_pitch1 - 
      sin_pitch2 * cos_roll * cos_pitch1;

    RotMatrix[1][0] = sin_roll * sin_pitch1;
    RotMatrix[1][1] = cos_roll;
    RotMatrix[1][2] = sin_roll * cos_pitch1;

    RotMatrix[2][0] = sin_pitch2 * cos_pitch1 + 
      cos_pitch2 * cos_roll * sin_pitch1;
    RotMatrix[2][1] = -cos_pitch2 * sin_roll;
    RotMatrix[2][2] = -sin_pitch2 * sin_pitch1 + 
      cos_pitch2 * cos_roll * cos_pitch1;
  }


  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<ROLL_YAW_ROLL> seq, 
		const T roll1, 
		const T yaw, 
		const T roll2, 
		T RotMatrix[3][3])
  {
    T sin_roll1 = ::sin(roll1), cos_roll1 = ::cos(roll1);
    T sin_yaw = ::sin(yaw), cos_yaw = ::cos(yaw);
    T sin_roll2 = ::sin(roll2), cos_roll2 = ::cos(roll2);

    RotMatrix[0][0] = cos_yaw;
    RotMatrix[0][1] = sin_yaw * cos_roll1;
    RotMatrix[0][2] = sin_yaw * sin_roll1;

    RotMatrix[1][0] = -cos_roll2 * sin_yaw;
    RotMatrix[1][1] = cos_roll2 * cos_yaw * cos_roll1 - 
      sin_roll2 * sin_roll1;
    RotMatrix[1][2] = cos_roll2 * cos_yaw * sin_roll1 + 
      sin_roll2 * cos_roll1;

    RotMatrix[2][0] = sin_roll2 * sin_yaw;
    RotMatrix[2][1] = -sin_roll2 * cos_yaw * cos_roll1 - 
      cos_roll2 * sin_roll1;
    RotMatrix[2][2] = -sin_roll2 * cos_yaw * sin_roll1 + 
      cos_roll2 * cos_roll1;
  }


  template <typename T>
  void 
  makeRotMatrix(
		euler_seq<ROLL_PITCH_ROLL> seq, 
		const T roll1, 
		const T pitch, 
		const T roll2, 
		T RotMatrix[3][3])
  {
    T sin_roll1 = ::sin(roll1), cos_roll1 = ::cos(roll1);
    T sin_pitch = ::sin(pitch), cos_pitch = ::cos(pitch);
    T sin_roll2 = ::sin(roll2), cos_roll2 = ::cos(roll2);

    RotMatrix[0][0] = cos_pitch;
    RotMatrix[0][1] = sin_pitch * sin_roll1;
    RotMatrix[0][2] = -sin_pitch * cos_roll1;

    RotMatrix[1][0] = sin_roll2 * sin_pitch;
    RotMatrix[1][1] = cos_roll2 * cos_roll1 - 
      sin_roll2 * cos_pitch * sin_roll1;
    RotMatrix[1][2] = cos_roll2 * sin_roll1 + 
      sin_roll2 * cos_pitch * cos_roll1;

    RotMatrix[2][0] = cos_roll2 * sin_pitch;
    RotMatrix[2][1] = -sin_roll2 * cos_roll1 - 
      cos_roll2 * cos_pitch * sin_roll1;
    RotMatrix[2][2] = -sin_roll2 * sin_roll1 + 
      cos_roll2 * cos_pitch * cos_roll1;
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<YAW_PITCH_ROLL> seq, 
		 const T RotMatrix[3][3],
		 T& yaw, 
		 T& pitch, 
		 T& roll)
  {
    if (RotMatrix[0][2] == -1.0)
      {
	yaw = atan2(RotMatrix[2][1], RotMatrix[1][1]);
	pitch = (M_PII / 2.0);
	roll = 0.0;
      }
    else if (RotMatrix[0][2] == 1.0)
      {
	yaw = atan2(RotMatrix[1][0], RotMatrix[2][0]);
	pitch = (-M_PII / 2.0);
	roll = 0.0;
      }
    else
      {
	yaw = atan2(RotMatrix[0][1], RotMatrix[0][0]);
	pitch = asin(-RotMatrix[0][2]);
	roll = atan2(RotMatrix[1][2], RotMatrix[2][2]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<YAW_ROLL_PITCH> seq,
		 const T RotMatrix[3][3],
		 T& yaw, 
		 T& roll, 
		 T& pitch)
  {
    if (RotMatrix[1][2] == 1.0)
      {
	yaw = atan2(RotMatrix[0][1], RotMatrix[0][0]);
	roll = (M_PII / 2.0);
	pitch = 0.0;
      }
    else if (RotMatrix[1][2] == -1.0)
      {
	yaw = atan2(RotMatrix[0][1], RotMatrix[0][0]);
	roll = (-M_PII / 2.0);
	pitch = 0.0;
      }
    else
      {
	yaw = atan2(-RotMatrix[1][0], RotMatrix[1][1]);
	roll = asin(RotMatrix[1][2]);
	pitch = atan2(-RotMatrix[0][2], RotMatrix[2][2]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<PITCH_YAW_ROLL> seq,
		 const T RotMatrix[3][3],
		 T& pitch,
		 T& yaw, 
		 T& roll)
  {
    if (RotMatrix[0][1] == 1.0)
      {
	pitch = atan2(RotMatrix[2][0], RotMatrix[2][2]);
	yaw = (M_PII / 2.0);
	roll = 0.0;
      }
    else if (RotMatrix[0][1] == -1.0)
      {
	pitch = atan2(RotMatrix[2][0], RotMatrix[1][0]);
	yaw = (-M_PII / 2.0);
	roll = 0.0;
      }
    else
      {
	pitch = atan2(-RotMatrix[0][2], RotMatrix[0][0]);
	yaw = asin(RotMatrix[0][1]);
	roll = atan2(-RotMatrix[2][1], RotMatrix[1][1]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<PITCH_ROLL_YAW> seq,
		 const T RotMatrix[3][3],
		 T& pitch,
		 T& roll, 
		 T& yaw)
  {
    if (RotMatrix[2][1] == -1.0)
      {
	pitch = atan2(RotMatrix[1][0], RotMatrix[0][0]);
	roll = (M_PII / 2.0);
	yaw = 0.0;
      }
    else if (RotMatrix[2][1] == 1.0)
      {
	pitch = atan2(-RotMatrix[1][0], RotMatrix[0][0]);
	roll = (-M_PII / 2.0);
	yaw = 0.0;
      }
    else
      {
	pitch = atan2(RotMatrix[2][0], RotMatrix[2][2]);
	roll = asin(-RotMatrix[2][1]);
	yaw = atan2(RotMatrix[0][1], RotMatrix[1][1]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<ROLL_PITCH_YAW> seq,
		 const T RotMatrix[3][3],
		 T& roll,
		 T& pitch, 
		 T& yaw)
  {
    if (RotMatrix[2][0] == 1.0)
      {
	roll = atan2(RotMatrix[0][1], RotMatrix[1][1]);
	pitch = (M_PII / 2.0);
	yaw = 0.0;
      }
    else if (RotMatrix[2][0] == -1.0)
      {
	roll = atan2(RotMatrix[1][2], RotMatrix[0][2]);
	pitch = (-M_PII / 2.0);
	yaw = 0.0;
      }
    else
      {
	roll = atan2(-RotMatrix[2][1], RotMatrix[2][2]);
	pitch = asin(RotMatrix[2][0]);
	yaw = atan2(-RotMatrix[1][0], RotMatrix[0][0]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<ROLL_YAW_PITCH> seq,
		 const T RotMatrix[3][3],
		 T& roll,
		 T& yaw, 
		 T& pitch)
  {
    if (RotMatrix[1][0] == -1.0)
      {
	roll = atan2(RotMatrix[0][2], RotMatrix[0][1]);
	yaw = (M_PII / 2.0);
	pitch = 0.0;
      }
    else if (RotMatrix[1][0] == 1.0)
      {
	roll = atan2(RotMatrix[2][1], RotMatrix[0][1]);
	yaw = (-M_PII / 2.0);
	pitch = 0.0;
      }
    else
      {
	roll = atan2(RotMatrix[1][2], RotMatrix[1][1]);
	yaw = asin(-RotMatrix[1][0]);
	pitch = atan2(RotMatrix[2][0], RotMatrix[0][0]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<YAW_PITCH_YAW> seq,
		 const T RotMatrix[3][3],
		 T& yaw1,
		 T& pitch, 
		 T& yaw2)
  {
    if(RotMatrix[2][2] == 1.0)
      {
	yaw1 = atan2(RotMatrix[0][1], RotMatrix[0][0]);
	pitch = 0.0;
	yaw2 = 0.0;
      }
    else if(RotMatrix[2][2] == -1.0)
      {
	yaw1 = atan2(RotMatrix[1][0], RotMatrix[0][0]);
	pitch = M_PII;
	yaw2 = 0.0;
      }
    else
      {
	yaw1 = atan2(RotMatrix[2][1], RotMatrix[2][0]);
	pitch = acos(RotMatrix[2][2]);
	yaw2 = atan2(RotMatrix[1][2], -RotMatrix[0][2]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<YAW_ROLL_YAW> seq,
		 const T RotMatrix[3][3],
		 T& yaw1,
		 T& roll, 
		 T& yaw2)
  {
    if(RotMatrix[2][2] == 1.0)
      {
	yaw1 = atan2(RotMatrix[0][1], RotMatrix[0][0]);
	roll = 0.0;
	yaw2 = 0.0;
      }
    else if(RotMatrix[2][2] == -1.0)
      {
	yaw1 = atan2(RotMatrix[0][1], RotMatrix[0][0]);
	roll = M_PII;
	yaw2 = 0.0;
      }
    else
      {
	yaw1 = atan2(RotMatrix[2][0], -RotMatrix[2][1]);
	roll = acos(RotMatrix[2][2]);
	yaw2 = atan2(RotMatrix[0][2], RotMatrix[1][2]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<PITCH_YAW_PITCH> seq,
		 const T RotMatrix[3][3],
		 T& pitch1,
		 T& yaw, 
		 T& pitch2)
  {
    if(RotMatrix[1][1] == 1.0)
      {
	pitch1 = atan2(RotMatrix[2][0], RotMatrix[0][0]);
	yaw = 0.0;
	pitch2 = 0.0;
      }
    else if(RotMatrix[1][1] == -1.0)
      {
	pitch1 = atan2(RotMatrix[2][0], RotMatrix[2][2]);
	yaw = M_PII;
	pitch2 = 0.0;
      }
    else
      {
	pitch1 = atan2(RotMatrix[1][2], -RotMatrix[1][0]);
	yaw = acos(RotMatrix[1][1]);
	pitch2 = atan2(RotMatrix[2][1], RotMatrix[0][1]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<PITCH_ROLL_PITCH> seq,
		 const T RotMatrix[3][3],
		 T& pitch1,
		 T& roll, 
		 T& pitch2)
  {
    if(RotMatrix[1][1] == 1.0)
      {
	pitch1 = atan2(RotMatrix[2][0], RotMatrix[0][0]);
	roll = 0.0;
	pitch2 = 0.0;
      }
    else if(RotMatrix[1][1] == -1.0)
      {
	pitch1 = atan2(-RotMatrix[2][0], RotMatrix[2][2]);
	roll = M_PII;
	pitch2 = 0.0;
      }
    else
      {
	pitch1 = atan2(RotMatrix[1][0], RotMatrix[1][2]);
	roll = acos(RotMatrix[1][1]);
	pitch2 = atan2(RotMatrix[0][1], -RotMatrix[2][1]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<ROLL_YAW_ROLL> seq,
		 const T RotMatrix[3][3],
		 T& roll1,
		 T& yaw, 
		 T& roll2)
  {
    if(RotMatrix[0][0] == 1.0)
      {
	roll1 = atan2(RotMatrix[1][2], RotMatrix[1][1]);
	yaw = 0.0;
	roll2 = 0.0;
      }
    else if(RotMatrix[0][0] == -1.0)
      {
	roll1 = atan2(RotMatrix[1][2], RotMatrix[1][1]);
	yaw = M_PII;
	roll2 = 0.0;
      }
    else
      {
	roll1 = atan2(RotMatrix[0][2], RotMatrix[0][1]);
	yaw = acos(RotMatrix[0][0]);
	roll2 = atan2(RotMatrix[2][0], -RotMatrix[1][0]);
      }
  }


  template <typename T>
  void 
  getEulerAngles(
		 euler_seq<ROLL_PITCH_ROLL> seq,
		 const T RotMatrix[3][3],
		 T& roll1,
		 T& pitch, 
		 T& roll2)
  {
    if(RotMatrix[0][0] == 1.0)
      {
	roll1 = atan2(RotMatrix[1][2], RotMatrix[1][1]);
	pitch = 0.0;
	roll2 = 0.0;
      }
    else if(RotMatrix[0][0] == -1.0)
      {
	roll1 = atan2(RotMatrix[1][2], RotMatrix[1][1]);
	pitch = M_PII;
	roll2 = 0.0;
      }
    else
      {
	roll1 = atan2(RotMatrix[0][1], -RotMatrix[0][2]);
	pitch = acos(RotMatrix[0][0]);
	roll2 = atan2(RotMatrix[1][0], RotMatrix[2][0]);
      }
  }

} // namespace SEuler
