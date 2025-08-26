#ifndef __COLORSOURCE_H__
#define __COLORSOURCE_H__
#include "SFML/Graphics.hpp"

using namespace sf;

class ColorSource{
    public:
    virtual Color get_color_by_float(double t) const = 0;
    virtual Color get_color_by_int(int t) const = 0;
    virtual Color get_color() const = 0;

    virtual ~ColorSource() = default;
};

class FlatColor: public ColorSource{
    Color color;
    public:

    FlatColor(Color);
    Color get_color_by_float(double t) const override;
    Color get_color_by_int(int t) const override;
    Color get_color() const override;
};

#endif