#include "camera.h"

Camera::Camera(const Point &pos, const Point &dir)
{
    this->pos = pos;
    this->dir = dir;
}

void Camera::move(const int dx, const int dy, const int dz)
{
    this->pos.move(dx, dy, dz);
}

void Camera::rot(const double fi_x, const double fi_y, const double fi_z)
{
    this->pos.rot(fi_x, fi_y, fi_z);
}
