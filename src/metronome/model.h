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
    Body(Point& center, QString& filename, QColor& color);
    ~Body();

    void set_center(Point& center);
    Point& get_center();

    void scale(const double kx, const double ky, const double kz);

private:
    vector<Point> points, norms;
    vector<vector<Point>> faces;

    Point center;
    QColor color;
};


class Pendulum : public QObject
{
    Q_OBJECT

public:
    Pendulum(Point& center, QString& filename, QColor& color);
    ~Pendulum();

    void set_center(Point& center);
    Point& get_center();

    void scale(const double kx, const double ky, const double kz);

    bool is_running();

public slots:
    void running(const int tempo);
    void staying();

signals:
    void run(const int tempo);
    void stop();

private:
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
