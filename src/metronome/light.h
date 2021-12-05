#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"

class Light
{
public:
    Light(const Point &pos, const float power);
    ~Light() = default;

    void set_pos(const Point &pos);
    Point& get_pos();

    void set_intensity(const float power);
    float get_intensity();

private:
    Point pos;
    float intensity;
};

#endif // LIGHT_H
