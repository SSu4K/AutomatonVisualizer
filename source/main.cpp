#include <SFML/Graphics.hpp>
#include <iostream>

#include "simulation/rules.h"
#include "simulation/simulation.h"
#include "window.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include "ui.h"

const float IMGUI_WINDOW_FRAMERATE = 60;

typedef enum {
  Reload,
  Close,
} SimulationResult;

SimulationSettings simulationSettings;
shared_ptr<SimulationWindow> sim_window;

Gradient gradient({sf::Color::Yellow, sf::Color::Red, sf::Color::Magenta,
                    sf::Color::Yellow},
                   PERIODIC);

void build_simulation_window();

SimulationResult run(RenderWindow& window);

int main() {
  srand(time(NULL));
  sf::RenderWindow window(sf::VideoMode(1000, 1000), "Simulation with ImGui");
  if (!ImGui::SFML::Init(window)) {
    return 0;
  }
  SimulationResult result = Reload;

  while (result == Reload) {
    build_simulation_window();
    result = run(window);
  }

  window.close();
  ImGui::SFML::Shutdown();

  return 0;
}

void build_simulation_window() {
  size_vector size = {size_t(simulationSettings.simulation_width),
                      size_t(simulationSettings.simulation_height)};

  BSRule carpet_rule({2, 3, 4, 5}, {});

  shared_ptr<BSRule> rule = nullptr;
  switch (simulationSettings.selected_rule) {
    case 0:
      rule = make_shared<BSRule>(6152);
      break;

    case 1:
      rule = make_shared<BSRule>(carpet_rule);
      break;

    default:
      rule = make_shared<BSRule>(6162);
      break;
  }

  Simulation simulation(size, rule, simulationSettings.clamped);

  if (simulationSettings.fill_type == 0) {
    simulation.random_fill(simulationSettings.fill_percentage / 100.0);
  } else {
    int point = simulationSettings.selected_point;
    if (point == 0) {
      simulation.set_cell_value(size[0] / 2, size[1] / 2, true);
    } else {
      point--;
      simulation.set_cell_value(size[0] * (point % 2), size[1] * (point < 2),
                                true);
    }
  }

  shared_ptr<IVertexArrayBuilder> builder = nullptr;
  if (simulationSettings.selected_builder == 0) {
    builder = make_shared<SquareNetBuilder>();
  } else {
    builder = make_shared<TriangleNetBuilder>();
  }

  WindowSettings settings = DEFAULT_WINDOW_SETTINGS;
  sim_window = make_shared<SimulationWindow>(
      std::make_shared<Simulation>(simulation), builder, settings);
}


SimulationResult run(RenderWindow& window) {
  sf::Clock deltaClock;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);
      if (event.type == sf::Event::Closed) {
        return Close;
      }
    }

    sf::Time dt = deltaClock.restart();
    ImGui::SFML::Update(window, dt);

    sim_window->renderSimulation();
    sim_window->step(dt.asSeconds());

    window.clear();

    // Main Menu
    ImGui::BeginMainMenuBar();
    ImGui::Text("Automaton Visualiser");
    ImGui::EndMainMenuBar();

    ImGui::Begin("TMP");
    ui::GradientPicker("Picker", gradient);
    ImGui::End();

    bool b = true;
    b &= ui::showViewUI();
    b &= ui::showSettingsUI();
    b &= ui::showControlPanelUI();

    // Render ImGui on top of the window
    ImGui::SFML::Render(window);

    sf::sleep(sf::seconds(1.f / IMGUI_WINDOW_FRAMERATE -
                          deltaClock.getElapsedTime().asSeconds()));

    window.display();
    if(!b){
      break;
    }
  }

  return Reload;
}
