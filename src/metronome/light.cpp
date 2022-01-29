#include "light.h"

Light::Light() : pos(Point(0, 0, 0)), intensity(500)
{

}

Light::Light(const Point &light_pos, const float power) : pos(light_pos), intensity(power)
{

}

void Light::set_pos(const Point &pos)
{
    this->pos = pos;
}

const Point& Light::get_pos()
{
    return this->pos;
}

void Light::set_intensity(const float power)
{
    this->intensity = power;
}

float Light::get_intensity()
{
    return this->intensity;
}
