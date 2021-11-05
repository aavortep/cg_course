#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"

class Light
{
public:
    Light(const Point pos);
    ~Light() = default;

private:
    Point pos;
};

#endif // LIGHT_H
