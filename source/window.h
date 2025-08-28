#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SFML/Graphics.hpp>
#include <random>

#include "body.h"
#include "colors/gradient.h"
#include "input/inputsystem.h"
#include "simulation/simulation.h"
#include "renderer/simulationRenderer.h"
#include "utils.hpp"

#define DEFAULT_FRAMERATE 60
#define DEFAULT_SIMULATION_FRAMERATE 10

struct WindowSettings {
  int framerate;
  int simulation_framerate;
  bool show_grid;
  float max_view_speed;
  float drag_force;
};

const WindowSettings DEFAULT_WINDOW_SETTINGS = {
    DEFAULT_FRAMERATE, DEFAULT_SIMULATION_FRAMERATE, false, 100, 2000};

class SimulationWindow : public sf::RenderWindow {
 private:
  shared_ptr<Simulation> simulation;
  SimulationRenderer renderer;
  sf::View view;

  InputSystem inputSystem;

  WindowSettings settings;

  int frames_per_step;
  int frame_counter;

  sf::Vector2f direction_vector;
  Body::Body view_body;

  function<sf::Color(const sf::Color&, const sf::Color&, float)> interp_func =
      utils::powrp<6>;

  bool do_update_view;
  bool do_redraw;
  bool paused;

  void update_view();
  void handle_input();

 public:
  SimulationWindow() = default;
  SimulationWindow(const sf::Vector2i window_size,
                   const shared_ptr<Simulation> simulation,
                   const WindowSettings& settings);
  SimulationWindow(const sf::Vector2i window_size,
                   const shared_ptr<Simulation> simulation);
  void step();
};

#endif