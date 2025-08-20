#ifndef __UTILS_H__
#define __UTILS_H__

#include <SFML/Graphics/Color.hpp>
#include <cmath>

namespace utils{
    double clamp(const double value, const double min, const double max);
    int clampi(const int value, const int min, const int max);
    sf::Color lerp(const sf::Color &c1, const sf::Color &c2, double t);
    
    template <int p>
    sf::Color powrp(const sf::Color& c1, const sf::Color& c2, float t);
}


template <int p>
sf::Color utils::powrp(const sf::Color& c1, const sf::Color& c2, float t){
  float adjustedT = std::pow(t, static_cast<float>(p));
  return lerp(c1, c2, adjustedT);
}
#endif