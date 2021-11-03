#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point() = default;
    Point(const int x, const int y, const int z);

    void move(const int dx, const int dy, const int dz);
    void scale(const double kx, const double ky, const double kz);
    void rot(const double fi_x, const double fi_y, const double fi_z);

private:
    int x, y, z;
};

#endif // POINT_H
