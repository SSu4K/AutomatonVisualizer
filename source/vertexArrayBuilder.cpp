#include "vertexArrayBuilder.h"

size_t SquareGridBuilder::get_segment_size() const{
    return 4;
}

sf::VertexArray SquareGridBuilder::build(const Simulation& simulation) const{
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

sf::VertexArray SquareGridBuilder::build_grid(const Simulation& simulation, const sf::Color color) const{
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