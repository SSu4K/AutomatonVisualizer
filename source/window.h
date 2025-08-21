#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SFML/Graphics.hpp>
#include <random>

#include "body.h"
#include "colors/gradient.h"
#include "input/inputsystem.h"
#include "simulation.h"
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

const WindowSettings DEFAULT_SETTINGS = {
    DEFAULT_FRAMERATE, DEFAULT_SIMULATION_FRAMERATE, false, 100, 2000};

class SimulationWindow : public sf::RenderWindow {
 private:
  shared_ptr<Simulation> simulation;

  sf::View view;
  sf::VertexArray vertex_array;
  sf::VertexArray grid;

  vector<sf::Color> old_color_array;
  vector<sf::Color> new_color_array;

  InputSystem inputSystem;

  WindowSettings settings;

  int frames_per_step;
  int frame_counter;

  sf::Vector2f direction_vector;
  Body::Body view_body;

  function<sf::Color(const sf::Color&, const sf::Color&, float)> interp_func =
      utils::powrp<3>;

  bool do_update_view;
  bool do_redraw;
  bool paused;

  void update_view();
  sf::Color get_color(bool value, bool loaded, size_t age);

  vector<sf::Color> get_color_array();
  void create_vertex_array();
  void update_vertex_array(double t);
  void handle_input();
  void redraw();

 public:
  SimulationWindow() = default;
  SimulationWindow(const sf::Vector2i window_size, const shared_ptr<Simulation> simulation);
  SimulationWindow(const sf::Vector2i window_size,
                   const shared_ptr<Simulation> simulation,
                   const WindowSettings &settings);

  void step();
};

#endif