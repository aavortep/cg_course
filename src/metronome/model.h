#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "edge.h"
#include "point.h"

using namespace std;

class Model
{
public:
    Model() = default;
    Model(vector<Point> pts, vector<int> edges);

    void scale(const double kx, const double ky, const double kz);

private:
    vector<Point> points;
    vector<Edge> edges;
};

#endif // MODEL_H
