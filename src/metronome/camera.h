#ifndef CAMERA_H
#define CAMERA_H

#include "point.h"

class Camera
{
public:
    Camera(const Point &pos, const Point &dir);
    ~Camera() = default;

    Point get_pos();
    Point get_view();

    void move(const int dx, const int dy, const int dz);
    void rot(const double fi_x, const double fi_y, const double fi_z);

private:
    Point pos, dir;
};

#endif // CAMERA_H
