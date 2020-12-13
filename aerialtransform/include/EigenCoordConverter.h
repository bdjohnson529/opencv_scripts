#ifndef __COORDCONVERTER_H__
#define __COORDCONVERTER_H__

#include <Eigen/Core>
#include <cmath>
#define _USE_MATH_DEFINES

#define RAD2DEG(alpha) (alpha*180.0/M_PI)
#define DEG2RAD(alpha) (alpha*M_PI/180.0)

class EigenCoordConverter
{
  private:
    double Oenu[3]; // East-north-up origin
    double ecr2enu[3][3];
    double enu2ecr[3][3];

    // Convert Unix seconds to Julian date
    double unixToJulian(double unixSecs);

    // Convert Julian date to Greenwich Mean Sidereal Time
    double julianToSidereal(double julian);

  public:

    EigenCoordConverter();
    EigenCoordConverter(const Eigen::Vector3d& coord0);

    Eigen::Vector3d geodeticToEcef(const Eigen::Vector3d& coord);
    Eigen::Vector3d ecefToGeodetic(const Eigen::Vector3d& ecef);

    Eigen::Vector3d ecefToNed(const Eigen::Vector3d& ecef);
    Eigen::Vector3d nedToEcef(const Eigen::Vector3d& ned);

    Eigen::Vector3d geodeticToNed(const Eigen::Vector3d& coord);
    Eigen::Vector3d nedToGeodetic(const Eigen::Vector3d& ned);

    Eigen::Vector3d ecefToEci(double time, const Eigen::Vector3d& ecef);
    Eigen::Vector3d eciToEcef(double time, const Eigen::Vector3d& eci);

    Eigen::Vector3d eciToGeodetic(double time, const Eigen::Vector3d& eci);
    Eigen::Vector3d geodeticToEci(double time, const Eigen::Vector3d& coord);
};

#endif
