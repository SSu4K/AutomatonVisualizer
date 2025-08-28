#include "vertexArrayBuilder.h"

size_t SquareNetBuilder::get_segment_size() const {
  return 4;
}

float SquareNetBuilder::get_x_ratio() const {
  return 1;
}

float SquareNetBuilder::get_y_ratio() const {
  return 1;
}

sf::VertexArray SquareNetBuilder::build(const Simulation& simulation) const {
  int width = simulation.get_size()[0];
  int height = simulation.get_size()[1];
  sf::VertexArray vertex_array(sf::Quads, 4 * simulation.get_cell_count());

  for (int i = 0; i < width * height; i++) {
    float x = i % width;
    float y = i / width;

    vertex_array[4 * i + 0] = sf::Vertex({x, y}, sf::Color::Black);
    vertex_array[4 * i + 1] = sf::Vertex({x + 1, y}, sf::Color::Black);
    vertex_array[4 * i + 2] = sf::Vertex({x + 1, y + 1}, sf::Color::Black);
    vertex_array[4 * i + 3] = sf::Vertex({x, y + 1}, sf::Color::Black);
  }

  return vertex_array;
}

sf::VertexArray SquareNetBuilder::build_net(const Simulation& simulation,
                                              const sf::Color color) const {
  sf::VertexArray grid;
  int width = simulation.get_size()[0];
  int height = simulation.get_size()[1];

  grid.setPrimitiveType(sf::PrimitiveType::Lines);

  for (int i = 0; i < width; i++) {
    grid.append({sf::Vector2f(i, 0), color});
    grid.append({sf::Vector2f(i, height), color});
  }

  for (int i = 0; i < height; i++) {
    grid.append({sf::Vector2f(0, i), color});
    grid.append({sf::Vector2f(width, i), color});
  }
  return grid;
}

static const float triangle_height = sqrt(3) / 2;

size_t TriangleNetBuilder::get_segment_size() const {
  return 3;
}

float TriangleNetBuilder::get_x_ratio() const {
  return 0.5;
}

float TriangleNetBuilder::get_y_ratio() const {
  return triangle_height;
}

sf::VertexArray TriangleNetBuilder::build(const Simulation& simulation) const {
  int width = simulation.get_size()[0];
  int height = simulation.get_size()[1];
  sf::VertexArray vertex_array(sf::Triangles, 3 * simulation.get_cell_count());

  for (int i = 0; i < width * height; i++) {
    float x = i % width;
    float y = i / width;

    bool flipped = (int(x) + int(y)) % 2;
    float base_y = !flipped ? y * triangle_height : (y + 1) * triangle_height;
    float spike_y = flipped ? y * triangle_height : (y + 1) * triangle_height;

    vertex_array[3 * i + 0] = sf::Vertex({x * 0.5f, base_y}, sf::Color::Black);
    vertex_array[3 * i + 1] =
        sf::Vertex({x * 0.5f + 1, base_y}, sf::Color::Black);
    vertex_array[3 * i + 2] =
        sf::Vertex({x * 0.5f + 0.5f, spike_y}, sf::Color::Black);
  }

  return vertex_array;
}

sf::VertexArray TriangleNetBuilder::build_net(const Simulation& simulation,
                                                const sf::Color color) const {
  sf::VertexArray grid;
  int width = simulation.get_size()[0];
  int height = simulation.get_size()[1];

  grid.setPrimitiveType(sf::PrimitiveType::Lines);

  // horizontal lines
  for (int i = 0; i <= height; i++) {
    float x_offset = i % 2 ? 0.5 : 0;
    grid.append({sf::Vector2f(0 + x_offset, i * triangle_height), color});
    grid.append(
        {sf::Vector2f(width * 0.5 + x_offset, i * triangle_height), color});
  }

  const float tan60 = tan(M_PI / 3);
  const float total_width = width * 0.5 + 0.5;
  const float total_height = height * triangle_height;

  // skewed lines +
  utils::LinearFunction linear1(tan60, 0);
  float c = -triangle_height * width;
  while (c <= total_height) {
    linear1.C = c;
    auto intersections = utils::get_rect_intersections(
        linear1, {0, 0, total_width, total_height});
    if (intersections.size() == 2) {
      grid.append({intersections[0], color});
      grid.append({intersections[1], color});
    }
    c += 2 * triangle_height;
  }

  // skewed lines -
  utils::LinearFunction linear2(-tan60, 0);
  c = 0;
  while (c <= total_height + (width+1) * triangle_height) {
    linear2.C = c;
    auto intersections = utils::get_rect_intersections(
        linear2, {0, 0, total_width, total_height});
    if (intersections.size() == 2) {
      grid.append({intersections[0], color});
      grid.append({intersections[1], color});
    }
    c += 2 * triangle_height;
  }

  return grid;
}