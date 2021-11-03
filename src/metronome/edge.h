#ifndef EDGE_H
#define EDGE_H

#include "point.h"

class Edge
{
public:
    Edge() = default;
    Edge(const Point a, const Point b);

private:
    Point a, b;
};

#endif // EDGE_H
