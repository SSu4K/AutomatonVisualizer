#include "utils.hpp"

double utils::clamp(double value, double min, double max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

int utils::clampi(int value, int min, int max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

sf::Color utils::lerp(const sf::Color& c1, const sf::Color& c2, double t) {
  t = utils::clamp(t, 0, 1);
  double k = 1 - t;
  return sf::Color(k * c1.r + t * c2.r, k * c1.g + t * c2.g,
                   k * c1.b + t * c2.b, k * c1.a + t * c2.a);
}

// Ax + By + C = 0

// Ax + C = -By
// (Ax + C)/(-B) = y
utils::LinearFunction::LinearFunction(float A, float B, float C) : A(A), B(B), C(C) {}
utils::LinearFunction::LinearFunction(float a, float b): A(a), B(-1), C(b) {}

float utils::LinearFunction::get_y_at(float x) const {
    return (A*x + C)/(-B);
}
float utils::LinearFunction::get_x_at(float y) const {
    return (B*y + C)/(-A);
}

std::vector<sf::Vector2f> utils::get_rect_intersections(LinearFunction &func, sf::Rect<float> rect){
    std::vector<sf::Vector2f> result;

    float left = rect.left;
    float right = left + rect.width;
    float top = rect.top;
    float bottom = top + rect.height;

    float intersect_y = func.get_y_at(left);
    if(intersect_y >= top && intersect_y <= bottom){
        result.push_back({left, intersect_y});
    }
    intersect_y = func.get_y_at(right);
    if(intersect_y >= top && intersect_y <= bottom){
        result.push_back({right, intersect_y});
    }
    float intersect_x = func.get_x_at(top);
    if(intersect_x >= left && intersect_x <= right){
        result.push_back({intersect_x, top});
    }
    intersect_x = func.get_x_at(bottom);
    if(intersect_x >= left && intersect_x <= right){
        result.push_back({intersect_x, bottom});
    }

    return result;
}