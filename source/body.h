#ifndef __BODY_H__
#define __BODY_H__

#include <SFML/System/Vector2.hpp>

using namespace sf;

namespace Body {

float calculate_resistance(float max_force, float max_speed);

class Body {
  float mass;
  float friction;
  float resistance;
  float speed_treshold;

  Vector2f pos;
  Vector2f speed;

 public:
  Body() = default;
  Body(float m, float frict, float res, float zero_speed_tresh = 0.1);
  void simulate(float dt, Vector2f force = {0, 0});

  void set_pos(Vector2f pos);

  Vector2f get_pos() const;
};
}  // namespace Body

#endif