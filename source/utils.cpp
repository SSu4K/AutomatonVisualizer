#include "utils.hpp"

double utils::clamp(double value, double min, double max){
  if(value<min) return min;
  if(value>max) return max;
  return value;
}

int utils::clampi(int value, int min, int max){
    if(value<min) return min;
    if(value>max) return max;
    return value;
}

sf::Color utils::lerp(const sf::Color &c1, const sf::Color &c2, double t){
  t = utils::clamp(t, 0, 1);
  double k = 1-t;
  return sf::Color(k*c1.r+t*c2.r, k*c1.g+t*c2.g, k*c1.b+t*c2.b, k*c1.a+t*c2.a);
}