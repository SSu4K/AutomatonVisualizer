#include "window.h"
#include <iostream>

static Gradient g1({sf::Color::Yellow, sf::Color::Red, sf::Color::Magenta,
                    sf::Color::Yellow},
                   PERIODIC);
static QuantizedGradient g2(g1, 20);

sf::VertexArray generate_grid(const sf::Vector2i size, const sf::Color color) {
  sf::VertexArray grid;
  grid.setPrimitiveType(sf::PrimitiveType::Lines);

  for (int i = 0; i < size.x; i++) {
    grid.append({sf::Vector2f(i, 0), color});
    grid.append({sf::Vector2f(i, size.y), color});
  }

  for (int i = 0; i < size.y; i++) {
    grid.append({sf::Vector2f(0, i), color});
    grid.append({sf::Vector2f(size.x, i), color});
  }
  return grid;
}

void SimulationWindow::update_view() {
  setView(this->view);
  do_update_view = false;
  do_redraw = true;
}

sf::Color SimulationWindow::get_color(bool value, bool loaded, size_t age) {
  if (value && loaded) {
    return sf::Color::White;
  }
  if (loaded) {
    return sf::Color::Black;
  }

  return g2.get_color_by_int(age);
}

vector<sf::Color> SimulationWindow::get_color_array() {
  vector<sf::Color> result;
  for (size_t i = 0; i < simulation->get_cell_count(); i++) {
    sf::Color color =
        get_color(simulation->value_buffer[i], simulation->loaded_buffer[i],
                  simulation->age_buffer[i]);
    result.push_back(color);
    color.toInteger();
  }

  return result;
}

void SimulationWindow::create_vertex_array() {
  vertex_array = sf::VertexArray(sf::PrimitiveType::Quads,
                                 4 * simulation->get_cell_count());
  const int width = simulation->get_size()[0];
  const int height = simulation->get_size()[1];
  float x, y;
  for (int i = 0; i < width * height; i++) {
    x = i % width;
    y = i / width;
    vertex_array[4 * i + 0] = sf::Vertex({x, y}, sf::Color::Black);
    vertex_array[4 * i + 1] = sf::Vertex({x + 1, y}, sf::Color::Black);
    vertex_array[4 * i + 2] = sf::Vertex({x + 1, y + 1}, sf::Color::Black);
    vertex_array[4 * i + 3] = sf::Vertex({x, y + 1}, sf::Color::Black);
  }
}

void SimulationWindow::update_vertex_array(double t) {
  for (size_t i = 0; i < simulation->get_cell_count(); i++) {
    sf::Color color = interp_func(old_color_array[i], new_color_array[i], t);
    vertex_array[4 * i + 0].color = color;
    vertex_array[4 * i + 1].color = color;
    vertex_array[4 * i + 2].color = color;
    vertex_array[4 * i + 3].color = color;
  }
}

SimulationWindow::SimulationWindow(const sf::Vector2i window_size,
                                   const shared_ptr<Simulation> simulation,
                                   const WindowSettings& settings)
    : sf::RenderWindow(sf::VideoMode(window_size.x, window_size.y),
                       "Simulation"),
      simulation(simulation),
      settings(settings),
      frame_counter(0),
      do_update_view(false),
      do_redraw(false),
      paused(true) {
  if (settings.simulation_framerate <= 0) {
    frames_per_step = 1;
  } else {
    frames_per_step = settings.framerate / settings.simulation_framerate;
    if (frames_per_step <= 0) {
      frames_per_step = 1;
    }
  }
  this->setFramerateLimit(settings.framerate);
  sf::Vector2i simulation_size = {(int)simulation->get_size()[0],
                                  (int)simulation->get_size()[1]};
  float w_ratio = simulation_size.x / float(window_size.x);
  float h_ratio = simulation_size.y / float(window_size.y);

  sf::Vector2f center = sf::Vector2f(simulation_size) / 2.0f;
  view = sf::View(center, sf::Vector2f(window_size)),
  view.zoom(std::max(w_ratio, h_ratio));
  setView(view);

  view_body = Body::Body(
      50, 10,
      Body::calculate_resistance(settings.drag_force, settings.max_view_speed),
      0.01 * settings.max_view_speed);
  view_body.set_pos(center);

  new_color_array = get_color_array();
  old_color_array = new_color_array;
  create_vertex_array();

  grid = generate_grid(simulation_size, sf::Color(40, 40, 40));

  this->simulation->reset();
}

SimulationWindow::SimulationWindow(const sf::Vector2i window_size,
                                   const shared_ptr<Simulation> simulation): SimulationWindow(window_size, simulation, DEFAULT_WINDOW_SETTINGS) {}

void SimulationWindow::handle_input() {
  inputSystem.update();

  if (inputSystem.zoomInAction.isPressed()) {
    view.zoom(0.8);
    do_update_view = true;
  }
  if (inputSystem.zoomOutAction.isPressed()) {
    view.zoom(1.2);
    do_update_view = true;
  }
  if (inputSystem.pauseAction.wasReleasedThisFrame()) {
    paused = !paused;
  }
  if (inputSystem.resetAction.wasReleasedThisFrame()) {
    simulation->reset();
    update_vertex_array(0);
  }

  direction_vector = inputSystem.arrowsAction.get_value();
}

void SimulationWindow::step() {
  handle_input();

  if (frame_counter == 0 && !paused) {
    simulation->step();
    old_color_array = vector<sf::Color>(new_color_array);
    new_color_array = get_color_array();
  }

  if (!paused) {
    double t = double(frame_counter) / frames_per_step;
    update_vertex_array(t);
  }

  clear();
  draw(vertex_array);
  if (settings.show_grid) {
    draw(grid);
  }

  display();
  frame_counter++;
  frame_counter %= frames_per_step;

  float dt = 1.0 / settings.framerate;
  view_body.simulate(dt, settings.drag_force * direction_vector);
  view.setCenter(view_body.get_pos());
  this->update_view();
}
