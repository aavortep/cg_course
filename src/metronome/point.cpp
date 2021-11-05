#include "point.h"
#include <cmath>

Point::Point(const int x, const int y, const int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

int Point::get_x() const
{
    return this->x;
}

int Point::get_y() const
{
    return this->y;
}

int Point::get_z() const
{
    return this->z;
}

Point& Point::operator = (const Point &pt)
{
    this->x = pt.x;
    this->y = pt.y;
    this->z = pt.z;
    return *this;
}

void Point::move(const int dx, const int dy, const int dz)
{
    this->x += dx;
    this->y += dy;
    this->z += dz;
}

void Point::scale(const double kx, const double ky, const double kz)
{
    this->x *= kx;
    this->y *= ky;
    this->z *= kz;
}

void Point::rot_x(const double fi)
{
    double rad = M_PI * fi / 180.0;
    int tmp_y = this->y, tmp_z = this->z;
    this->y = tmp_y * cos(rad) + tmp_z * sin(rad);
    this->z = -tmp_y * sin(rad) + tmp_z * cos(rad);
}

void Point::rot_y(const double fi)
{
    double rad = M_PI * fi / 180.0;
    int tmp_x = this->x, tmp_z = this->z;
    this->x = tmp_x * cos(rad) - tmp_z * sin(rad);
    this->z = tmp_x * sin(rad) + tmp_z * cos(rad);
}

void Point::rot_z(const double fi)
{
    double rad = M_PI * fi / 180.0;
    int tmp_x = this->x, tmp_y = this->y;
    this->x = tmp_x * cos(rad) + tmp_y * sin(rad);
    this->y = -tmp_x * sin(rad) + tmp_y * cos(rad);
}

void Point::rot(const double fi_x, const double fi_y, const double fi_z)
{
    this->rot_x(fi_x);
    this->rot_y(fi_y);
    this->rot_z(fi_z);
}
