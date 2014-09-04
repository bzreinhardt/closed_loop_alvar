#include "Filter.h"
#include "Kalman.h"

class KalmanSensorOwn : public KalmanSensorEkf {
    virtual void h(CvMat *x_pred, CvMat *_z_pred) {
        double x = cvmGet(x_pred, 0, 0);
        double y = cvmGet(x_pred, 1, 0);
        double dx = cvmGet(x_pred, 2, 0);
        double dy = cvmGet(x_pred, 3, 0);
        cvmSet(_z_pred, 0, 0, x);
        cvmSet(_z_pred, 1, 0, y);
    }
public:
    KalmanSensorOwn(int _n, int _m) : KalmanSensorEkf(_n, _m) {}
};

class KalmanOwn : public KalmanEkf {
    virtual void f(CvMat *_x, CvMat *_x_pred, double dt) {
        double x = cvmGet(_x, 0, 0);
        double y = cvmGet(_x, 1, 0);
        double dx = cvmGet(_x, 2, 0);
        double dy = cvmGet(_x, 3, 0);
        cvmSet(_x_pred, 0, 0, x + dt*dx);
        cvmSet(_x_pred, 1, 0, y + dt*dy);
        cvmSet(_x_pred, 2, 0, dx);
        cvmSet(_x_pred, 3, 0, dy);
    }
public:
    KalmanOwn(int _n) : KalmanEkf(_n) {}
};

