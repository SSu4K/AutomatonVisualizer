#include "vertexArrayBuilder.h"

size_t SquareGridBuilder::get_segment_size() const{
    return 4;
}

float SquareGridBuilder::get_x_ratio() const{
    return 1;
}

float SquareGridBuilder::get_y_ratio() const{
    return 1;
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

static const float triangle_height = sqrt(3)/2;

size_t TriangleGridBuilder::get_segment_size() const{
    return 3;
}

float TriangleGridBuilder::get_x_ratio() const{
    return 0.5;
}

float TriangleGridBuilder::get_y_ratio() const{
    return triangle_height;
}

sf::VertexArray TriangleGridBuilder::build(const Simulation& simulation) const{
  int width = simulation.get_size()[0];
  int height = simulation.get_size()[1];
  sf::VertexArray vertex_array(sf::Triangles, 3 * simulation.get_cell_count());

  for (int i = 0; i < width * height; i++) {
    float x = i % width;
    float y = i / width;

    bool flipped = (int(x)+int(y)) % 2;
    float base_y = !flipped ? y*triangle_height : (y+1)*triangle_height;
    float spike_y = flipped ? y*triangle_height : (y+1)*triangle_height;

    
    vertex_array[3 * i + 0] = sf::Vertex({x*0.5f,        base_y}, sf::Color::Black);
    vertex_array[3 * i + 1] = sf::Vertex({x*0.5f + 1,    base_y}, sf::Color::Black);
    vertex_array[3 * i + 2] = sf::Vertex({x*0.5f + 0.5f,    spike_y}, sf::Color::Black);
    
  }

  return vertex_array;
}

sf::VertexArray TriangleGridBuilder::build_grid(const Simulation& simulation, const sf::Color color) const{
  sf::VertexArray grid;
  grid.setPrimitiveType(sf::PrimitiveType::Lines);
  return grid;
}