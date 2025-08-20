#ifndef __COLORSOURCE_H__
#define __COLORSOURCE_H__
#include "SFML/Graphics.hpp"

using namespace sf;

class ColorSource{
    public:
    virtual Color get_color_by_float(double t) = 0;
    virtual Color get_color_by_int(int t) = 0;
    virtual Color get_color() = 0;

    virtual ~ColorSource() = default;
};

class FlatColor{
    Color color;
    public:

    FlatColor(Color);
    Color get_color_by_float(double t);
    Color get_color_by_int(int t);
    Color get_color();
};

#endif