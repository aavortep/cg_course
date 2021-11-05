#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point() = default;
    Point(const int x, const int y, const int z);
    ~Point() = default;

    int get_x() const;
    int get_y() const;
    int get_z() const;

    Point& operator= (const Point &pt);

    void move(const int dx, const int dy, const int dz);
    void scale(const double kx, const double ky, const double kz);
    void rot(const double fi_x, const double fi_y, const double fi_z);

    void rot_x(const double fi);
    void rot_y(const double fi);
    void rot_z(const double fi);

private:
    int x, y, z;
};

#endif // POINT_H
