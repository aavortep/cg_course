#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include "model.h"
#include "light.h"
#include "camera.h"

class Canvas : public QLabel
{
public:
    explicit Canvas(QWidget *parent = 0);
    ~Canvas() = default;

    void draw_body(const Body &body);
    void draw_pendulum(const Pendulum &pend);
    void draw_point(const Point &pt);
    void draw_line(const Edge &edge);

    void set_color(const QColor &color);
    void clear_image();

    void update();

private:
    QGraphicsScene *scene;
    QImage image;

    QColor pen_color;
    QColor brush_color;
    QPen pen;
};

#endif // CANVAS_H
