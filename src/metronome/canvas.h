#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include "model.h"
#include "light.h"
#include "camera.h"

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
    void set_body(const Body &body);
    void set_pendulum(const Pendulum &pend);
    Body& get_body();
    Pendulum& get_pendulum();

    void scale_model(const double kx, const double ky, const double kz);

    // свет
    void set_light(const Point &pos, const float power);
    Point& get_light();

    // камера
    void set_camera(const Point &pos, const Point &dir);
    Point& get_camera();

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
    void set_camera(const Point &pos, const Point &dir);

    void move_camera(const int dx, const int dy, const int dz);
    void rotate_camera(const double fi_x, const double fi_y, const double fi_z);

    float process_light(const Point &vert, const Point &norm);

private:
    int width = 1431, height = 971;
    Scene scene;
    QImage image;

    void init();
    void clear();
    void update();

    ZBuffer zbuffer;
    void init_zbuffer();
    void clear_zbuffer();

    vector<vector<QColor>> colorCache;
    void init_color_cache();
    void clear_color_cache();

    void process_body(Body &body, Point &cam_pos, Point &cam_dir);
    void process_triangle(Point &v1, Point &v2, Point &v3, const QColor &color,
                          float &i1, float &i2, float &i3);

    bool is_visible(const Point &pt);

    QColor bg_color = QColor(94, 115, 107);
    QColor pen_color, brush_color;
    QPen pen;
};

#endif // CANVAS_H
