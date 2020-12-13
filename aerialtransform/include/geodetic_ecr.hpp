// +AMGD -*- c++ -*- $Id: geodetic_ecr.hpp 4 2012-08-24 02:28:17Z mike $ file: geodetic_ecr.cpp author: MJ Pfauth, 3
// Jan 2004

// Geodetic latitude, longitude and altitude (WGS 84 ellipsoid) to ECR

// Copyright 2003 to 2009 Michael J. Pfauth, mpfauth@hiwaay.net.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "wgs84.h"

#include <cmath>


namespace SCoord {

#ifndef sq
#define sq(a) ((a)*(a))
#endif

    // Prime vertical radius, Vanicek eqn 15.58.
    template <typename T>
    T
    primeVR (
        const T lat)
    {
        return WGS84::Re2 / sqrt (WGS84::Re2 * sq(cos(lat)) 
				  + WGS84::Rp2 * sq(sin(lat)));

    } // primeVR


    // Geodetic --> ECR, Vanicek 15.63.
    template <typename T>
    void
    posGeodeticToEcr (
        const T Plla[3],
        T Pecr[3])
    {
        T N = primeVR (Plla[0]);
        T t1 = (N + Plla[2]) * cos (Plla[0]);
        Pecr[0] = t1 * cos (Plla[1]);
        Pecr[1] = t1 * sin (Plla[1]);
        Pecr[2] = (N * WGS84::Rp2/WGS84::Re2 + Plla[2]) * sin (Plla[0]);

    }  // posGeodeticToEcr

    // ECR --> Geodetic, Vanicek 15.64 to 15.70.
    template <typename T>
    void
    posEcrToGeodetic (
        const T Pecr[3],
        T Plla[3])
    {
        // The angle subtended by 1 meter at the equator.
        T eps = 1.0 / WGS84::EQUATORIAL_RADIUS;
        T p = sqrt (sq(Pecr[0]) + sq(Pecr[1]));

        T lat1, lat2, h1, h2, N;

        lat2 = atan (Pecr[2] / (p * (1.0 - WGS84::ECC2)));
        N = primeVR (lat2);
        h2 = (p / cos (lat2)) - N;

        do
        {
            lat1 = lat2;
            h1 = h2;

            lat2 = atan (Pecr[2] / (p * (1.0 - (WGS84::ECC2 * N) / (N + h1))));
            N = primeVR (lat2);
            h2 = (p / cos (lat2)) - N;

        } while (fabs (h2 - h1) > 1.0 && fabs (lat2 - lat1) > eps);

        Plla[0] = lat2;
        Plla[1] = 2.0 * atan (Pecr[1] / (Pecr[0] + p));
        Plla[2] = h2;

    } // posEcrToGeodetic


    template <typename T>
    void
    velGeodeticToEcr (
        const T Plla[3],
        const T Vlla[3],
        T Vecr[3])
    {
        T Vg[3];  // "ENU" velocity components

        Vg[0] = Vlla[1] * sin (Vlla[0]);
        Vg[1] = Vlla[1] * cos (Vlla[0]);
        Vg[2] = Vlla[2];

        // The rotation matrix
        T Mtx[3][3];
        SCoord::makeEnuToEcrMatrix (Plla[0], Plla[1], Mtx);

        // Transform the velocity
        SCoord::velEnuToEcr (Mtx, Vg, Vecr);

    } // velGeodeticToEcr   


    template <typename T>
    void
    accGeodeticToEcr (
        const T Plla[3],
        const T Alla[3],
        T Aecr[3])
    {
        T Ag[3];  // "ENU" acceleration components

        Ag[0] = Alla[1] * sin (Alla[0]);
        Ag[1] = Alla[1] * cos (Alla[0]);
        Ag[2] = Alla[2];

        // The rotation matrix
        T Mtx[3][3];
        SCoord::makeEnuToEcrMatrix (Plla[0], Plla[1], Mtx);

        // Transform the acceleration
        SCoord::accEnuToEcr (Mtx, Ag, Aecr);

    } // accGeodeticToEcr


    template <typename T>
    void
    velEcrToGeodetic (
        const T Vecr[3],
        const T Plla[3],
        T Vlla[3])
    {
        T Mtx[3][3];
        T Vg[3];

        makeEcrToEnuMatrix (Plla[0], Plla[1], Mtx);

        velEcrToEnu (Mtx, Vecr, Vg);

        Vlla[0] = atan2 (Vg[0], Vg[1]);
        Vlla[1] = sqrt (sq (Vg[0]) + sq (Vg[1]));
        Vlla[2] = Vg[2];

    } // velEcrToGeodetic


    template <typename T>
    void
    accEcrToGeodetic (
        const T Aecr[3],
        const T Plla[3],
        T Alla[3])
    {
        T Mtx[3][3];
        T Ag[3];

        makeEcrToEnuMatrix (Plla[0], Plla[1], Mtx);

        accEcrToEnu (Mtx, Aecr, Ag);

        Alla[0] = atan2 (Ag[0], Ag[1]);
        Alla[1] = sqrt (sq (Ag[0]) + sq (Ag[1]));
        Alla[2] = Ag[2];

    } // accEcrToGeodetic

}  // namespace SCoord
