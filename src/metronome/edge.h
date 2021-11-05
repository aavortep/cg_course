#ifndef EDGE_H
#define EDGE_H

#include "point.h"

class Edge
{
public:
    Edge(const int ind_1, const int ind_2);
    ~Edge() = default;

private:
    int ind_1, ind_2;
};

#endif // EDGE_H
