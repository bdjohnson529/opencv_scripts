// +AMGD    -*- c++ -*-
// $Id: wgs84.h 4 2012-08-24 02:28:17Z mike $
// file:  wgs84.h
// author:  MJ Pfauth, 27 Dec 2003

// Copyright 2003 to 2012 Michael J. Pfauth, mpfauth@hiwaay.net.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef WGS84_H
#define WGS84_H

namespace WGS84 {

// Earth angular velocity, rad/sec
const double OMEGA_E = 7.2921151e-5;

// Seconds in a sideral day, based on the above angular velocity
const double SIDEREAL_DAY = 86164.0994556;

// Earth polar radius, meters
const double POLAR_RADIUS = 6356752.3;
const double Rp2 = POLAR_RADIUS * POLAR_RADIUS;

// Earth equatorial radius, meters
const double EQUATORIAL_RADIUS = 6378137.0;
const double Re2 = EQUATORIAL_RADIUS * EQUATORIAL_RADIUS;

// Flattening coefficient
const double FLAT = (EQUATORIAL_RADIUS - POLAR_RADIUS) / EQUATORIAL_RADIUS;

// Eccentricity
const double ECC = 0.081819190842622;
const double ECC2 = ECC * ECC;

}

#endif

