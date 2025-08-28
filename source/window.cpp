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
  // setView(this->view);
  do_update_view = false;
  do_redraw = true;
}

SimulationWindow::SimulationWindow(const sf::Vector2i window_size,
                                   const shared_ptr<Simulation> simulation,
                                   const WindowSettings& settings)
    : simulation(simulation),
      renderer(simulation, make_shared<TriangleNetBuilder>()),
      settings(settings),
      frame_counter(0),
      do_update_view(false),
      do_redraw(false),
      paused(true) {
  renderTexture.create(window_size.x, window_size.y);
  renderSprite.setTexture(renderTexture.getTexture());
  view = renderTexture.getDefaultView();

  if (settings.simulation_framerate <= 0) {
    frames_per_step = 1;
  } else {
    frames_per_step = settings.framerate / settings.simulation_framerate;
    if (frames_per_step <= 0) {
      frames_per_step = 1;
    }
  }

  // this->setFramerateLimit(settings.framerate);

  sf::Vector2f simulation_size = renderer.get_simulation_size();
  float w_ratio = simulation_size.x / float(window_size.x);
  float h_ratio = simulation_size.y / float(window_size.y);

  sf::Vector2f center = sf::Vector2f(simulation_size) / 2.0f;
  view = sf::View(center, sf::Vector2f(window_size)),
  view.zoom(std::max(w_ratio, h_ratio));
  // setView(view);

  view_body = Body::Body(
      50, 10,
      Body::calculate_resistance(settings.drag_force, settings.max_view_speed),
      0.01 * settings.max_view_speed);
  view_body.set_pos(center);

  this->simulation->reset();
}

SimulationWindow::SimulationWindow(const sf::Vector2i window_size,
                                   const shared_ptr<Simulation> simulation)
    : SimulationWindow(window_size, simulation, DEFAULT_WINDOW_SETTINGS) {}

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
    renderer.update_vertex_array(0);
  }

  direction_vector = inputSystem.arrowsAction.get_value();
}

void SimulationWindow::step() {
  handle_input();

  if (frame_counter == 0 && !paused) {
    simulation->step();
    renderer.push_color_buffer();
  }

  if (!paused) {
    double t = double(frame_counter) / frames_per_step;
    renderer.update_vertex_array(t);
  }

  frame_counter++;
  frame_counter %= frames_per_step;

  float dt = 1.0 / settings.framerate;
  view_body.simulate(dt, settings.drag_force * direction_vector);
  view.setCenter(view_body.get_pos());
  this->update_view();
}

void SimulationWindow::renderSimulation() {
  renderTexture.clear(sf::Color::Black);

  renderTexture.setView(view);

  renderer.render(renderTexture, *simulation);

  renderTexture.display();
}

const sf::Texture& SimulationWindow::getTexture() const {
    return renderTexture.getTexture();
}
