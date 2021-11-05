#include <vector>
#include "model.h"
#include "point.h"

using namespace std;

Body::Body(vector<Point> pts, vector<Edge> edges)
{
    for (size_t i = 0; i < pts.size(); ++i)
        this->points.push_back(pts[i]);
    for (size_t i = 0; i < edges.size(); ++i)
        this->edges.push_back(edges[i]);
}

Body::~Body()
{
    this->points.clear();
    this->edges.clear();
}

void Body::scale(const double kx, const double ky, const double kz)
{
    for (size_t i = 0; i < this->points.size(); ++i)
        this->points[i].scale(kx, ky, kz);
}

Pendulum::Pendulum(vector<Point> pts, vector<Edge> edges)
{
    for (size_t i = 0; i < pts.size(); ++i)
        this->points.push_back(pts[i]);
    for (size_t i = 0; i < edges.size(); ++i)
        this->edges.push_back(edges[i]);

    QObject::connect(this, SIGNAL(run(const int tempo)), this, SLOT(running(const int tempo)));
    QObject::connect(this, SIGNAL(stop()), this, SLOT(staying()));
}

Pendulum::~Pendulum()
{
    this->points.clear();
    this->edges.clear();
}

bool Pendulum::is_running()
{
    if (this->state == STAY)
        return 0;
    else
        return 1;
}

void Pendulum::running(const int tempo)
{
    // колебания

    if (this->state == STAY)
        this->state = RUN;
    emit run(tempo);
}

void Pendulum::staying()
{
    // остановка метронома

    if (this->state == RUN)
        this->state = STAY;
}

void Pendulum::scale(const double kx, const double ky, const double kz)
{
    for (size_t i = 0; i < this->points.size(); ++i)
        this->points[i].scale(kx, ky, kz);
}
