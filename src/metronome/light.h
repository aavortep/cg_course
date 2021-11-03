#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"

class Light
{
public:
    Light() = default;
    Light(const Point pos);

private:
    Point pos;
};

#endif // LIGHT_H
