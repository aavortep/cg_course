#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"
#define LIGHT_REFLECT 1
#define BG_LIGHT 0.3

class Light
{
public:
    Light();
    Light(const Point &pos, const float power);
    ~Light() = default;

    void set_pos(const Point &pos);
    const Point& get_pos();

    void set_intensity(const float power);
    float get_intensity();

private:
    Point pos;
    float intensity;
};

#endif // LIGHT_H
