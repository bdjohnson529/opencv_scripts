// +AMGD     -*- c++ -*-
// $Id: eci_ecr.hpp 4 2012-08-24 02:28:17Z mike $
// file:  eci_ecr.cpp
// author:  MJ Pfauth, 27 Dec 2003

// Copyright 2003 to 2009 Michael J. Pfauth, mpfauth@hiwaay.net.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "smatrix.h"
#include "seuler.h"
#include "wgs84.h"


namespace SCoord {

// ECI <--> ECR

    template <typename T>
    void 
    makeEciToEcrMatrix (
	const T time,
	T Teci2ecr[3][3])
    {
	SEuler::makeYawMatrix (time * (T)WGS84::OMEGA_E, Teci2ecr);
    }

    template <typename T>
    void 
    makeEcrToEciMatrix (
	const T time,
	T Tecr2eci[3][3])
    {
	SEuler::makeYawMatrix (-time * (T)WGS84::OMEGA_E, Tecr2eci);
    }

    template <typename T>
    void
    posEciToEcr (
	const T Teci2ecr[3][3], 
	const T Peci[3], 
	T Pecr[3])
    {
	SMatrix::mtxMult (Teci2ecr, Peci, Pecr);
    }

    template <typename T>
    void
    posEcrToEci (
	const T Tecr2eci[3][3], 
	const T Pecr[3], 
	T Peci[3])
    {
	SMatrix::mtxMult (Tecr2eci, Pecr, Peci);
    }


// Vecr = Teci2ecr * (Veci - Omega X Peci), note cross product
    template <typename T>
    void
    velEciToEcr (
	const T Teci2ecr[3][3],
	const T Peci[3],
	const T Veci[3],
	T Vecr[3])
    {
	const T omega[3] = {0.0, 0.0, WGS84::OMEGA_E};

	T V1[3], V2[3], V3[3];

	SMatrix::vecCross (omega, Peci, V1);
	SMatrix::vecSubtract (Veci, V1, V2);
	SMatrix::mtxMult (Teci2ecr, V2, Vecr);
    }


// Veci = Tecr2eci * Vecr + Omega X (Tecr2eci * Pecr)
    template <typename T>
    void
    velEcrToEci (
	const T Tecr2eci[3][3],
	const T Pecr[3],
	const T Vecr[3],
	T Veci[3])
    {
	const T omega[3] = {0.0, 0.0, WGS84::OMEGA_E};

	T V1[3], V2[3], V3[3];

	SMatrix::mtxMult (Tecr2eci, Pecr, V1);
	SMatrix::mtxMult (Tecr2eci, Vecr, V2);
	SMatrix::vecCross (omega, V1, V3);
	SMatrix::vecAdd (V2, V3, Veci);
    }

// Aecr = Teci2ecr * Aeci - 2 * Teci2ecr * OmegaCross * Veci + Teci2ecr * OmegaCross2 * Peci
    template <typename T>
    void
    accEciToEcr (
	const T Teci2ecr[3][3],
	const T Peci[3],
	const T Veci[3],
	const T Aeci[3],
	T Aecr[3])
    {
	const T OmegaCross[3][3] = { {0.0, -WGS84::OMEGA_E, 0.0},
				     {WGS84::OMEGA_E, 0.0, 0.0},
				     {0.0, 0.0, 0.0} };

	const T OmegaCross2[3][3] = { {-(WGS84::OMEGA_E)*(WGS84::OMEGA_E), 0.0, 0.0},
				      {0.0, -(WGS84::OMEGA_E)*(WGS84::OMEGA_E), 0.0},
				      {0.0, 0.0, 0.0} };

	T A1[3], A2[3][3], A3[3][3], A4[3], A5[3][3], A6[3], A7[3];

	SMatrix::mtxMult (Teci2ecr, Aeci, A1);
	SMatrix::mtxMult ((T)2.0, Teci2ecr, A2);
	SMatrix::mtxMult (A2, OmegaCross, A3);
	SMatrix::mtxMult (A3, Veci, A4);
	SMatrix::mtxMult (Teci2ecr, OmegaCross2, A5);
	SMatrix::mtxMult (A5, Peci, A6);
	SMatrix::mtxSubtract (A1, A4, A7);
	SMatrix::mtxAdd (A7, A6, Aecr);
    }

// Aeci = Tecr2eci * Aecr + 2 * OmegaCross * Tecr2eci * Vecr +
// OmegaCross2 * Tecr2eci * Pecr
    template <typename T>
    void
    accEcrToEci (
	const T Tecr2eci[3][3],
	const T Pecr[3],
	const T Vecr[3],
	const T Aecr[3],
	T Aeci[3])
    {
	const T OmegaCross[3][3] = { {0.0, -WGS84::OMEGA_E, 0.0},
				     {WGS84::OMEGA_E, 0.0, 0.0},
				     {0.0, 0.0, 0.0} };

	const T OmegaCross2[3][3] = { {-(WGS84::OMEGA_E)*(WGS84::OMEGA_E), 0.0, 0.0},
				      {0.0, -(WGS84::OMEGA_E)*(WGS84::OMEGA_E), 0.0},
				      {0.0, 0.0, 0.0} };

	T A1[3], A2[3][3], A3[3][3], A4[3], A5[3][3], A6[3], A7[3];

	SMatrix::mtxMult (Tecr2eci, Aecr, A1);
	SMatrix::mtxMult ((T)2.0, OmegaCross, A2);
	SMatrix::mtxMult (A2, Tecr2eci, A3);
	SMatrix::mtxMult (A3, Vecr, A4);
	SMatrix::mtxMult (OmegaCross2, Tecr2eci, A5);
	SMatrix::mtxMult (A5, Pecr, A6);
	SMatrix::mtxAdd (A1, A4, A7);
	SMatrix::mtxAdd (A7, A6, Aeci);
    }

} // namespace SCoord
