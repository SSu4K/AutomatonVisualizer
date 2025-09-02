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

SimulationWindow::SimulationWindow(const shared_ptr<Simulation> simulation, const shared_ptr<IVertexArrayBuilder> builder,
                                   const WindowSettings& settings)
    : simulation(simulation),
      renderer(simulation, builder),
      settings(settings),
      isPaused(false) {
  renderTexture.create(settings.window_size.x, settings.window_size.y);
  renderSprite.setTexture(renderTexture.getTexture());
  view = renderTexture.getDefaultView();

  sf::Vector2f simulation_size = renderer.get_simulation_size();
  float w_ratio = simulation_size.x / float(settings.window_size.x);
  float h_ratio = simulation_size.y / float(settings.window_size.y);

  sf::Vector2f center = sf::Vector2f(simulation_size) / 2.0f;
  view = sf::View(center, sf::Vector2f(settings.window_size)),
  view.zoom(std::max(w_ratio, h_ratio));

  view_body = Body::Body(
      50, 10,
      Body::calculate_resistance(settings.drag_force, settings.max_view_speed),
      0.01 * settings.max_view_speed);
  view_body.set_pos(center);

  //this->simulation->reset();
}

SimulationWindow::SimulationWindow(const shared_ptr<Simulation> simulation, const WindowSettings& settings)
    : SimulationWindow(simulation, make_shared<SquareNetBuilder>(), settings) {}

void SimulationWindow::handle_input() {
  inputSystem.update();

  if (inputSystem.zoomInAction.isPressed()) {
    view.zoom(0.8);
  }
  if (inputSystem.zoomOutAction.isPressed()) {
    view.zoom(1.2);
  }
  if (inputSystem.pauseAction.wasReleasedThisFrame()) {
    togglePaused();
  }
  if (inputSystem.resetAction.wasReleasedThisFrame()) {
    //simulation->reset();
    renderer.update_vertex_array(0);
  }

  direction_vector = inputSystem.arrowsAction.get_value();
}

void SimulationWindow::stepSimulation() {
  if (!isPaused) {
    simulation->step();
    renderer.push_color_buffer();
  }
}

void SimulationWindow::stepWindow(float dt) {
  handle_input();
  view_body.simulate(dt, settings.drag_force * direction_vector);
  view.setCenter(view_body.get_pos());
}

void SimulationWindow::interpolate(float t) {
  if (!isPaused) {
    renderer.update_vertex_array(t);
  }
}

void SimulationWindow::step(float dt){
    stepWindow(dt);
    float timePerFrame = 1.0/settings.simulation_framerate;
    float elapsed = deltaClock.getElapsedTime().asSeconds();
    if(elapsed >= timePerFrame){
        stepSimulation();
        deltaClock.restart();
    }
    else{
        interpolate(elapsed/timePerFrame);
    }
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

WindowSettings SimulationWindow::getSettings() const {
  return settings;
}

bool SimulationWindow::getIsPaused() const {
  return isPaused;
}

void SimulationWindow::setSimulationFramerate(float framerate) {
  if (framerate > 0) {
    settings.simulation_framerate = framerate;
  }
}

void SimulationWindow::setPaused(bool status) {
  isPaused = status;
}

void SimulationWindow::togglePaused() {
  isPaused = !isPaused;
}
