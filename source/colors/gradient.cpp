#include "gradient.h"
#include <math.h>
#include "utils.hpp"

GradnientMode GradientBase::get_mode() const{
    return this->mode;
}

double GradientBase::transform_t_double(double t) const{
    switch (mode){
    case CLAMPED:
        return utils::clamp(t, 0, 1);

    case PERIODIC:
        double tmp;
        return modf(t, &tmp);

    case LIMITED:
        if(t>=1 && t<=1){
            return t;
        }
    }
    return -1;
}

Gradient::Gradient(Color color1, Color color2, GradnientMode mode){
    this->mode = mode;
    nodes.push_back({color1, 0});
    nodes.push_back({color2, 1});
}

Gradient::Gradient(vector<Color> colors, GradnientMode mode){
    this->mode = mode;
    double increment = 1.0/(colors.size());
    for(size_t i=0; i<colors.size()-1; i++){
        nodes.push_back({colors[i], i*increment});
    }
    nodes.push_back({colors[colors.size()-1], 1});    
}

Gradient::Gradient(vector<ColorNode> nodes, GradnientMode mode){
    this->mode = mode;
    this->nodes = nodes;
    if(nodes[0].pos != 0){
        nodes.insert(nodes.begin(), {default_color, 0});
    }

    if(nodes[nodes.size()-1].pos != 1){
        nodes.push_back({default_color, 1});
    }
}

Color Gradient::get_color_by_float(double t) const{
    t = transform_t_double(t);
    if(t < 0) return default_color;
    if(t==0) return nodes[0].color;
    ColorNode left = {default_color, 0};
    ColorNode right = nodes[0];
    for(size_t i=1; i<nodes.size(); i++){
        left = right;
        right = nodes[i];
        if(left.pos <= t && t <= right.pos){
            double weight = (t-left.pos)/(right.pos-left.pos);
            return utils::lerp(left.color, right.color, weight);
        }
    }
    return nodes[nodes.size()-1].color;
}

Color Gradient::get_color_by_int(int t) const{
    if(t >= int(nodes.size())){
        return nodes[nodes.size()-1].color;
    }
    if(t < 0){
        return nodes[0].color;
    }

    return nodes[t].color;
}

Color Gradient::get_color() const{
    return nodes[0].color;
}

QuantizedGradient::QuantizedGradient(Gradient source_gradient, size_t range): range(range) {
    mode = source_gradient.get_mode();
    
    float rangef = float(range);
    for(size_t i = 0; i < range; i++){
        this->colors.push_back(source_gradient.get_color_by_float(i/rangef));
    }
}

size_t QuantizedGradient::transform_t_int(size_t t) const{
    switch (mode){
    case CLAMPED:
        return utils::clampi(t, 0, range-1);
    case PERIODIC:
        return t%range;
    case LIMITED:
        if(t<range){
            return t;
        }
    }
    return -1;
}

Color QuantizedGradient::get_color_by_float(double t) const{
    size_t i = transform_t_int(round(t*range));
    return colors[i];
}

Color QuantizedGradient::get_color_by_int(int t) const{ 
    size_t i = transform_t_int(t);
    return colors[i];
}

Color QuantizedGradient::get_color() const{
    return default_color;
}