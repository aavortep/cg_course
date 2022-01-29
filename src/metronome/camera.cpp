#include "camera.h"
#include <cmath>

Camera::Camera() : pos(Point(0, 0, 0)), view(Point(0, 0, 0)), up(Point(0, 1, 0))
{

}

Camera::Camera(const Point &cam_pos, const Point &dir, const Point &cam_up) : pos(cam_pos), view(dir), up(cam_up)
{

}

void Camera::set_pos(const Point &pos)
{
    this->pos = pos;
}

Point Camera::get_pos()
{
    return this->pos;
}

void Camera::set_view(const Point &dir)
{
    this->view = dir;
}

Point Camera::get_view()
{
    return this->view;
}

void Camera::set_up(const Point &up)
{
    this->up = up;
}

Point Camera::get_up()
{
    return this->up;
}

void Camera::move(const int dx, const int dy, const int dz)
{
    this->pos.move(dx, dy, dz);
}

void Camera::rot(const double fi_x, const double fi_y, const double fi_z)
{
    this->pos.rot(fi_x, fi_y, fi_z);
}

Matrix Camera::look_at(Point& eye, Point& center, Point& up)
{
    Point z = (eye - center).normalize();
    Point x = (up ^ z).normalize();
    Point y = (z ^ x).normalize();

    Matrix Minv = Matrix::identity(4);
    Matrix Tr = Matrix::identity(4);

    for (int i = 0; i < 3; i++)
    {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];

        Tr[i][3] = -center[i];
    }

    return Matrix(Minv * Tr);

}

Matrix Camera::viewport(const int x, const int y, const int w, const int h)
{
    Matrix m = Matrix::identity(4);

    m[0][3] = x + w / 2.0f;
    m[1][3] = y + h / 2.0f;
    m[2][3] = DEPTH / 2.0f;

    m[0][0] = w / 2.0f;
    m[1][1] = h / 2.0f;
    m[2][2] = DEPTH / 2.0f;

    return Matrix(m);
}

Matrix Camera::perspective(float const fovy, float const aspect, float const zNear, float const zFar)
{
    float rad = (fovy / 2) * M_PI / 180;
    float tanHalfFovy = tan(rad);

    Matrix result = Matrix();
    result.fill(0.0f);

    result[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
    result[1][1] = static_cast<float>(1) / (tanHalfFovy);
    result[2][2] = - (zFar + zNear) / (zFar - zNear);
    result[2][3] = - static_cast<float>(1);
    result[3][2] = - (static_cast<float>(2) * zFar * zNear) / (zFar - zNear);

    return result;
}
