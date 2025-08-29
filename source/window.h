#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SFML/Graphics.hpp>
#include <random>

#include "body.h"
#include "colors/gradient.h"
#include "input/inputsystem.h"
#include "renderer/simulationRenderer.h"
#include "simulation/simulation.h"
#include "utils.hpp"

#define DEFAULT_SIMULATION_FRAMERATE 10

struct WindowSettings {
  sf::Vector2i window_size;
  int simulation_framerate;
  bool show_grid;
  float max_view_speed;
  float drag_force;
};

const WindowSettings DEFAULT_WINDOW_SETTINGS = {
    {800, 800}, DEFAULT_SIMULATION_FRAMERATE, false, 100,
    2000};

class SimulationWindow {
 private:
  shared_ptr<Simulation> simulation;
  SimulationRenderer renderer;
  sf::RenderTexture renderTexture;
  sf::Sprite renderSprite;
  sf::View view;
  sf::Clock deltaClock;

  InputSystem inputSystem;
  WindowSettings settings;

  sf::Vector2f direction_vector;
  Body::Body view_body;

  function<sf::Color(const sf::Color&, const sf::Color&, float)> interp_func =
      utils::powrp<6>;

  bool isPaused;

  void handle_input();

 public:
  SimulationWindow() = default;
  SimulationWindow(const shared_ptr<Simulation> simulation,
                   const WindowSettings& settings);
  SimulationWindow(const shared_ptr<Simulation> simulation);
  void stepSimulation();
  void stepWindow(float dt);
  void interpolate(float t);
  void step(float dt);

  void renderSimulation();

  const sf::Texture& getTexture() const;
  WindowSettings getSettings() const;
  bool getIsPaused() const;

  void setSimulationFramerate(float framerate);
  void setPaused(bool state);
  void togglePaused();
};

#endif