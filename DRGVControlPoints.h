#ifndef _DRGVCONTROLPOINTS_H_
#define _DRGVCONTROLPOINTS_H_

void getControlPointParams(int maptype, double wlong, double elong,
                           double nlat, double slat, int& num_across,
                           int& num_down, double& width, double& height);

void calcGroundCoords(int maptype, int utmzone, int datum,
                      double wlong, double elong, double nlat, double slat,
                      double x[], double y[], int& numpoints);
#endif
