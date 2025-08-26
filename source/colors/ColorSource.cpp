#include "ColorSource.h"
FlatColor::FlatColor(Color color){
    this->color = color;
}

Color FlatColor::get_color_by_float(double t) const{
    return color;
}

Color FlatColor::get_color_by_int(int t) const{
    return color;
}

Color FlatColor::get_color() const{
    return color;
}