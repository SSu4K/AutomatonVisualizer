#include "simulationRenderer.h"

static const Gradient g1({sf::Color::Yellow, sf::Color::Red, sf::Color::Magenta,
                          sf::Color::Red},
                         PERIODIC);
static const QuantizedGradient g2(g1, 20);

//static const sf::Color GRID_COLOR = sf::Color(0, 0, 0);
static const sf::Color GRID_COLOR = sf::Color::Black;

static sf::Color get_color(bool value, bool loaded, size_t age) {
  if (value && loaded) {
    return sf::Color::White;
  }
  if (loaded) {
    return sf::Color::Black;
  }

  return g2.get_color_by_int(age);
}

SimulationRenderer::SimulationRenderer(shared_ptr<Simulation> simulation,
                                       shared_ptr<IVertexArrayBuilder> builder)
    : simulation(simulation),
        builder(builder),
      segment_size(builder->get_segment_size()),
      segment_count(simulation->get_cell_count()) {
  color_buffer_index = 0;
  color_buffer[0].resize(segment_count);
  color_buffer[1].resize(segment_count);

  vertex_array = builder->build(*simulation);
  net = builder->build_net(*simulation, GRID_COLOR);
}

SimulationRenderer::~SimulationRenderer() {
  // delete [] color_buffer[0];
  // delete [] color_buffer[1];
}

void SimulationRenderer::update_vertex_array(double t) {
  for (size_t i = 0; i < segment_count; i++) {
    sf::Color color = interp_func(color_buffer[!color_buffer_index][i],
                                  color_buffer[color_buffer_index][i], t);
    for (size_t j = 0; j < segment_size; j++) {
      vertex_array[segment_size * i + j].color = color;
    }
  }
}

void SimulationRenderer::push_color_buffer() {
  color_buffer_index = !color_buffer_index;
  for (size_t i = 0; i < segment_count; i++) {
    color_buffer[color_buffer_index][i] =
        get_color(simulation->value_buffer[i], simulation->loaded_buffer[i],
                  simulation->age_buffer[i]);
  }
}

sf::Vector2f SimulationRenderer::get_simulation_size() const{
    float width = simulation->get_size()[0];
    float height = simulation->get_size()[1];

    return sf::Vector2f(width*builder->get_x_ratio(), height * builder->get_y_ratio());
}