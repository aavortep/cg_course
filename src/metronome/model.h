#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <vector>
#include <cstdlib>
#include "edge.h"
#include "point.h"

using namespace std;

class Body : public QObject
{
    Q_OBJECT

public:
    Body(QString& filename = QString("model/body.obj"), QColor& color = QColor(0, 0, 0),
         Point& center = Point(0, 0, 0));
    ~Body();

    void set_center(const Point& center);
    Point& get_center();

    int get_verts_cnt();
    Point& vert(const int&);

    int get_faces_cnt();
    vector<int> face(const int&);

    int get_norms_cnt();
    void set_norm(const int&, const int&, const Point&);
    Point& norm(const int&, const int&);

    void set_color(const QColor&);
    QColor& get_color();

    void scale(const double kx, const double ky, const double kz);

    Body& operator = (const Body &body);

private:
    Point norm_calc(const Point&, const Point&, const Point&);
    void norms_processing();

    vector<Point> points, norms;
    vector<vector<Point>> faces;

    Point center;
    QColor color;
};


class Pendulum : public QObject
{
    Q_OBJECT

public:
    Pendulum(QString& filename = QString("model/pendulum.obj"), QColor& color = QColor(0, 0, 0),
             Point& center = Point(0, 0, 0));
    ~Pendulum();

    void set_center(const Point& center);
    Point& get_center();

    int get_verts_cnt();
    Point& vert(const int&);

    int get_faces_cnt();
    vector<Point> face(const int&);

    int get_norms_cnt();
    void set_norm(const int&, const int&, const Point&);
    Point& norm(const int&, const int&);

    void set_color(const QColor&);
    QColor& get_color();

    void scale(const double kx, const double ky, const double kz);

    Pendulum& operator = (const Body &body);

    bool is_running();

public slots:
    void running(const int tempo);
    void staying();

signals:
    void run(const int tempo);
    void stop();

private:
    Point norm_calc(const Point&, const Point&, const Point&);
    void norms_processing();

    vector<Point> points, norms;
    vector<vector<Point>> faces;

    Point center;
    QColor color;

    enum model_state
    {
        STAY,
        RUN
    };
    model_state state = STAY;
};

#endif // MODEL_H
