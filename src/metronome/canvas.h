#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include "model.h"
#include "light.h"
#include "camera.h"
#include "matrix.h"

#define FAULT 500

class Scene
{
private:
    Camera cam;
    Body body;
    Pendulum pend;
    Light light;

public:
    Scene();

    // модель
    void set_body(const Point center);
    void set_pendulum(const Point center);
    Body& get_body();
    Pendulum& get_pendulum();

    void scale_model(const double kx, const double ky, const double kz);

    // свет
    void set_light(const Point &pos, const float power);
    Light& get_light();

    // камера
    void set_camera(const Point &pos, const Point &dir, const Point &up);
    Camera& get_camera();
    Point& get_camera_pos();
    Point& get_camera_view();
    Point& get_camera_up();

    void move_camera(const int dx, const int dy, const int dz);
    void rotate_camera(const double fi_x, const double fi_y, const double fi_z);
};


class ZBuffer
{
private:
    int w, h;
    std::vector<std::vector<int>> data;
    int background = std::numeric_limits<int>::min();

public:
    ZBuffer();
    ZBuffer(const int&, const int&);

    int width();
    int height();

    void clear();

    void set_depth(const int&, const int&, const int&);
    int  get_depth(const int&, const int&);
};


class Canvas : public QGraphicsScene
{
public:
    explicit Canvas(const int&, const int&, QObject *parent = nullptr);
    ~Canvas() = default;

    void draw_body();
    void draw_pendulum();

    // модель
    void add_body(Point& center, QString& filename, QColor& color);
    void add_pendulum(Point& center, QString& filename, QColor& color);

    void scale_model(const double kx, const double ky, const double kz);

    // свет
    void set_light(const Point &pos, const float power);

    // камера
    void move_camera(const int dx, const int dy, const int dz);
    void rotate_camera(const double fi_x, const double fi_y, const double fi_z);

private:
    int width = 1431, height = 971;
    Scene scene;
    QImage image;

    void init();
    void clear();
    void update();

    void update_screen();
    void clear_screen();

    ZBuffer zbuffer;
    void init_zbuffer();
    void clear_zbuffer();

    vector<vector<QColor>> color_cache;
    void init_color_cache();
    void clear_color_cache();

    void process_body(Body &body, Point &cam_pos, Point &cam_dir, Point &cam_up);
    void process_pend(Pendulum &pend, Point &cam_pos, Point &cam_dir, Point &cam_up);
    void process_triangle(Point &v1, Point &v2, Point &v3, const QColor &color,
                          float &i1, float &i2, float &i3);
    float process_light(const Point &vert, const Point &norm);

    int wPerm, hPerm;
    bool is_visible(const Point &pt);

    QColor bg_color = QColor(94, 115, 107);
    QColor pen_color, brush_color;
    QPen pen;
};

inline QRgb iColor(const QRgb& a, const float& i);

#endif // CANVAS_H
