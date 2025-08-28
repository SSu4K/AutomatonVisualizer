#ifndef __UTILS_H__
#define __UTILS_H__

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cmath>
#include <vector>
#include <utility>

namespace utils{
    double clamp(const double value, const double min, const double max);
    int clampi(const int value, const int min, const int max);
    sf::Color lerp(const sf::Color &c1, const sf::Color &c2, double t);
    
    template <int p>
    sf::Color powrp(const sf::Color& c1, const sf::Color& c2, float t);

    // Ax + By + C = 0
    class LinearFunction{
        public:
        float A;
        float B;
        float C;

        LinearFunction(float A, float B, float C);
        LinearFunction(float a, float b);

        float get_y_at(float x) const;
        float get_x_at(float y) const;
    };

    std::vector<sf::Vector2f> get_rect_intersections(LinearFunction &func, sf::Rect<float> rect);
}


template <int p>
sf::Color utils::powrp(const sf::Color& c1, const sf::Color& c2, float t){
  float adjustedT = std::pow(t, static_cast<float>(p));
  return lerp(c1, c2, adjustedT);
}
#endif