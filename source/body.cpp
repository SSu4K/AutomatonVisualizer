#include "body.h"

#include <cmath>
namespace Body {

float calculate_resistance(float max_force, float max_speed) {
  return max_force / powf(max_speed, 2);
}

float length(Vector2f vec) {
  return hypot(vec.x, vec.y);
}

Vector2f normal(Vector2f vec) {
  float len = length(vec);
  if (len == 0) {
    return vec;
  }
  return {vec.x / len, vec.y / len};
}

void decompose(Vector2f vec, Vector2f& direction, float& value) {
  value = length(vec);
  if (value == 0) {
    direction = {0, 0};
  } else {
    direction = {vec.x / value, vec.y / value};
  }
}

Body::Body(float m, float frict, float res, float zero_speed_tresh)
    : mass(m),
      friction(frict),
      resistance(res),
      speed_treshold(zero_speed_tresh),
      pos({0, 0}),
      speed({0, 0}) {}

void Body::simulate(float dt, Vector2f drag_force) {
  float v;
  Vector2f direction;
  decompose(speed, direction, v);

  if (v < speed_treshold) {
    v = 0;
  }

  Vector2f friction_force = -friction * mass * direction;
  Vector2f resistance_force = -resistance * powf(v, 2) * direction;

  Vector2f total_force = drag_force + friction_force + resistance_force;
  Vector2f a = total_force / mass;

  pos += (a / 2.0f) * powf(dt, 2) + speed * dt;
  speed += a * dt;
}

void Body::set_pos(Vector2f pos) {
  this->pos = pos;
}

Vector2f Body::get_pos() const {
  return pos;
}

}  // namespace Body
