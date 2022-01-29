#ifndef CAMERA_H
#define CAMERA_H

#include "point.h"
#include "matrix.h"
#define DEPTH 255

class Camera
{
public:
    Camera();
    Camera(const Point &cam_pos, const Point &dir, const Point &cam_up);
    ~Camera() = default;

    void set_pos(const Point &pos);
    Point get_pos();

    void set_view(const Point &dir);
    Point get_view();

    void set_up(const Point &up);
    Point get_up();

    void move(const int dx, const int dy, const int dz);
    void rot(const double fi_x, const double fi_y, const double fi_z);

    static Matrix look_at(Point &eye, Point &center, Point &up);
    static Matrix viewport(const int x, const int y, const int w, const int h);
    static Matrix perspective(float const fovy, float const aspect, float const zNear, float const zFar);

private:
    Point pos, view, up;
};

#endif // CAMERA_H
