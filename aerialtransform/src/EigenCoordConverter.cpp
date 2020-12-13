#include "EigenCoordConverter.h"
//#include <pcl/pcl_macros.h>
#include <cmath>
#include <eci_ecr.hpp>
#include <ecr_enu.hpp>
#include <geodetic_ecr.hpp>

using namespace std;
using namespace SCoord;

EigenCoordConverter::EigenCoordConverter():EigenCoordConverter(Eigen::Vector3d(0, 0, 0))
{
}

EigenCoordConverter::EigenCoordConverter(const Eigen::Vector3d& coord0)
{
  double lla[3];
  lla[0] = DEG2RAD(coord0(0));
  lla[1] = DEG2RAD(coord0(1));
  lla[2] = coord0(2);

  makeEcrToEnuMatrix(lla[0], lla[1], ecr2enu);
  makeEnuToEcrMatrix(lla[0], lla[1], enu2ecr);

  // Set the NED coordinate origin
  posGeodeticToEcr(lla, Oenu);
}

Eigen::Vector3d EigenCoordConverter::geodeticToEcef(const Eigen::Vector3d& coord)
{
  double lla[3];
  double ecr[3];

  lla[0] = DEG2RAD(coord(0));
  lla[1] = DEG2RAD(coord(1));
  lla[2] = coord(2);

  posGeodeticToEcr(lla, ecr);
  return Eigen::Vector3d(ecr[0], ecr[1], ecr[2]);
}

Eigen::Vector3d EigenCoordConverter::ecefToGeodetic(const Eigen::Vector3d& ecef)
{
  double ecr[3];
  double lla[3];

  ecr[0] = ecef(0);
  ecr[1] = ecef(1);
  ecr[2] = ecef(2);

  posEcrToGeodetic(ecr, lla);
  return Eigen::Vector3d(RAD2DEG(lla[0]), RAD2DEG(lla[1]), lla[2]);
}

Eigen::Vector3d EigenCoordConverter::ecefToNed(const Eigen::Vector3d& ecef)
{
  double ecr[3];
  double enu[3];

  ecr[0] = ecef(0);
  ecr[1] = ecef(1);
  ecr[2] = ecef(2);

  posEcrToEnu(ecr2enu, Oenu, ecr, enu);
  return Eigen::Vector3d(enu[1], enu[0], -enu[2]);
}

Eigen::Vector3d EigenCoordConverter::nedToEcef(const Eigen::Vector3d& ned)
{
  double enu[3];
  double ecr[3];

  enu[0] = ned(1);
  enu[1] = ned(0);
  enu[2] = -ned(2);

  posEnuToEcr(enu2ecr, Oenu, enu, ecr);
  return Eigen::Vector3d(ecr[0], ecr[1], ecr[2]);
}

Eigen::Vector3d EigenCoordConverter::geodeticToNed(const Eigen::Vector3d& coord)
{
  Eigen::Vector3d ecef = geodeticToEcef(coord);
  return ecefToNed(ecef);
}

Eigen::Vector3d EigenCoordConverter::nedToGeodetic(const Eigen::Vector3d& ned)
{
  Eigen::Vector3d ecef = nedToEcef(ned);
  return ecefToGeodetic(ecef);
}

Eigen::Vector3d EigenCoordConverter::eciToEcef(double time, const Eigen::Vector3d& in_eci)
{
  double eci2ecr[3][3];
  makeEciToEcrMatrix(time, eci2ecr);

  double eci[3];
  double ecr[3];

  eci[0] = in_eci(0);
  eci[1] = in_eci(1);
  eci[2] = in_eci(2);

  posEciToEcr(eci2ecr, eci, ecr);

  return Eigen::Vector3d(ecr[0], ecr[1], ecr[2]);
}

Eigen::Vector3d EigenCoordConverter::ecefToEci(double time, const Eigen::Vector3d& ecef)
{
  double ecr2eci[3][3];
  makeEcrToEciMatrix(time, ecr2eci);

  double ecr[3];
  double eci[3];

  ecr[0] = ecef(0);
  ecr[1] = ecef(1);
  ecr[2] = ecef(2);

  posEcrToEci(ecr2eci, ecr, eci);

  return Eigen::Vector3d(eci[0], eci[1], eci[2]);
}

Eigen::Vector3d EigenCoordConverter::eciToGeodetic(double time, const Eigen::Vector3d& eci)
{
  const Eigen::Vector3d& ecef = eciToEcef(time, eci);
  return ecefToGeodetic(ecef);
}

Eigen::Vector3d EigenCoordConverter::geodeticToEci(double time, const Eigen::Vector3d& coord)
{
  const Eigen::Vector3d& ecef = geodeticToEcef(coord);
  return ecefToEci(time, ecef);
}

double EigenCoordConverter::unixToJulian(double unixSecs)
{
  return (unixSecs / 86400.0) + 2440587.5;
}

double EigenCoordConverter::julianToSidereal(double julian)
{
  double jdMin = floor(julian) - 0.5;
  double jdMax = floor(julian) + 0.5;
  double jd0 = julian > jdMax ? jdMax : jdMin;
  double ut = (julian - jd0) * 24.0;
  double t = (jd0 - 2451545.0) / 36525.0;
  double t0 = 6.697374558 + (2400.051336 * t) + (0.000025862 * t * t) + (ut * 1.0027379093);
  return DEG2RAD(fmod(t0, 24.0) * 15.0);
}
