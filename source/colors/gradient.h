#ifndef __GRADIENT_H__
#define __GRADIENT_H__

#include "SFML/Graphics.hpp"
#include "ColorSource.h"
#include <vector>

using namespace sf;
using namespace std;

struct ColorNode{
    Color color;
    double pos; // <0, 1>
};

enum GradnientMode{
    CLAMPED,
    PERIODIC,
    LIMITED,
};

class GradientBase: public ColorSource{
    protected:
    Color default_color = Color::Black;
    GradnientMode mode;
    double transform_t_double(double t);
    public:
    GradnientMode get_mode();
};

class Gradient: public GradientBase{
    vector<ColorNode> nodes;
    public:
    Gradient(Color color1, Color color2, GradnientMode mode = CLAMPED);
    Gradient(vector<Color> colors, GradnientMode mode = CLAMPED);
    Gradient(vector<ColorNode> nodes, GradnientMode mode = CLAMPED);

    Color get_color_by_float(double t);
    Color get_color_by_int(int t);
    Color get_color();
};

class QuantizedGradient: public GradientBase{
    vector<Color> colors;
    size_t range;
    size_t transform_t_int(size_t t);
    public:
    QuantizedGradient(Gradient source_gradient, size_t range);
    
    Color get_color_by_float(double t);
    Color get_color_by_int(int t);
    Color get_color();
};

#endif