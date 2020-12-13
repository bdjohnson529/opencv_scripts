// +AMGD   -*- c++ -*-
// $Id: ecr_enu.hpp 4 2012-08-24 02:28:17Z mike $
// file:  ecr_enu.cpp
// author:  MJ Pfauth, 23 Dec 2003

// Copyright 2003 to 2009 Michael J. Pfauth, mpfauth@hiwaay.net.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "smatrix.h"
#include "seuler.h"
#include "wgs84.h"


namespace SCoord {


    template <typename T>
    T
    getGeocentricLatFromLLA (
	const T Plla[3])
    {
	T Pecr[3];

	posGeodeticToEcr (Plla, Pecr);

	return asin (Pecr[2] / sqrt (sq (Pecr[0]) + sq (Pecr[1])));
    }

    template <typename T>
    T
    getGeocentricLatFromEcr (
	const T Pecr[3])
    {
	return asin (Pecr[2] / sqrt (sq (Pecr[0]) + sq (Pecr[1])));
    }

    template <typename T>
    T
    getGeocentricLongFromEcr (
	const T Pecr[3])
    {
	return atan2 (Pecr[1], Pecr[0]);
    }


    template <typename T>
    void
    makeEcrToEnuMatrix (
	const T gd_lat,
	const T gd_long,
	T Tecr2enu[3][3])
    {
	T sinLat = sin (gd_lat);
	T cosLat = cos (gd_lat);
	T sinLong = sin (gd_long);
	T cosLong = cos (gd_long);

	Tecr2enu[0][0] = -sinLong;
	Tecr2enu[0][1] = cosLong;
	Tecr2enu[0][2] = 0.0;

	Tecr2enu[1][0] = -sinLat * cosLong;
	Tecr2enu[1][1] = -sinLat * sinLong;
	Tecr2enu[1][2] = cosLat;

	Tecr2enu[2][0] = cosLat * cosLong;
	Tecr2enu[2][1] = cosLat * sinLong;
	Tecr2enu[2][2] = sinLat;
    }

    template <typename T>
    void
    makeEnuToEcrMatrix (
	const T gd_lat,
	const T gd_long,
	T Tenu2ecr[3][3])
    {
	T sinLat = sin (gd_lat);
	T cosLat = cos (gd_lat);
	T sinLong = sin (gd_long);
	T cosLong = cos (gd_long);

	Tenu2ecr[0][0] = -sinLong;
	Tenu2ecr[0][1] = -sinLat * cosLong;
	Tenu2ecr[0][2] = cosLat * cosLong;

	Tenu2ecr[1][0] = cosLong;
	Tenu2ecr[1][1] = -sinLat * sinLong;
	Tenu2ecr[1][2] = cosLat * sinLong;

	Tenu2ecr[2][0] = 0.0;
	Tenu2ecr[2][1] = cosLat;
	Tenu2ecr[2][2] = sinLat;
    }

    template <typename T>
    void
    posEcrToEnu (
	const T Tecr2enu[3][3],
	const T Oenu[3],
	const T Pecr[3],
	T Penu[3])
    {
	T transPos[3];

	SMatrix::mtxSubtract (Pecr, Oenu, transPos);

	SMatrix::mtxMult (Tecr2enu, transPos, Penu);
    }

    template <typename T>
    void
    posEnuToEcr (
	const T Tenu2ecr[3][3],
	const T Oenu[3],
	const T Penu[3],
	T Pecr[3])
    {
	T transPos[3];

	SMatrix::mtxMult (Tenu2ecr, Penu, transPos);

	SMatrix::mtxAdd (transPos, Oenu, Pecr);

    }


    template <typename T>
    void
    velEcrToEnu (
	const T Tenu2ecr[3][3],
	const T Vecr[3],
	T Venu[3])
    {
	SMatrix::mtxMult (Tenu2ecr, Vecr, Venu);
    }


    template <typename T>
    void
    velEnuToEcr (
	const T Tecr2enu[3][3],
	const T Venu[3],
	T Vecr[3])
    {
	SMatrix::mtxMult (Tecr2enu, Venu, Vecr);
    }

    template <typename T>
    void
    accEcrToEnu (
	const T Tecr2enu[3][3],
	const T Aecr[3],
	T Aenu[3])
    {
	SMatrix::mtxMult (Tecr2enu, Aecr, Aenu);
    }


    template <typename T>
    void
    accEnuToEcr (
	const T Tenu2ecr[3][3],
	const T Aenu[3],
	T Aecr[3])
    {
	SMatrix::mtxMult (Tenu2ecr, Aenu, Aecr);
    }

} // namespace SCoord

