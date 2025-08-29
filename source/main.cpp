#include <SFML/Graphics.hpp>
#include <iostream>

#include "simulation/rules.h"
#include "simulation/simulation.h"
#include "window.h"

#include <imgui-SFML.h>
#include <imgui.h>

const float IMGUI_WINDOW_FRAMERATE = 60;

typedef enum {
  Reload,
  Close,
} SimulationResult;

struct SimulationSettings {
  int simulation_width = 100;
  int simulation_height = 100;
  int selected_builder = 0;
  int selected_rule = 0;
};

SimulationSettings simulationSettings;
shared_ptr<SimulationWindow> sim_window;

int simulationFramerate = 10;
bool reload = true;

void build_simulation_window() {
  size_vector size = {size_t(simulationSettings.simulation_width),
                      size_t(simulationSettings.simulation_height)};
  BSRule bs_gol_rule(6152);
  Simulation simulation(size, std::make_shared<BSRule>(bs_gol_rule), 0.1);

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

void showViewUI() {
  ImGui::Begin("View", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Image(sim_window->getTexture());
  ImGui::End();
}

void showControlPanelUI() {
  ImGui::Begin("Control Panel");
  ImGui::SameLine();
  ImGui::BeginGroup();
  ImGui::Text("Simulation running...");

  if (ImGui::SliderInt("Framerate", &simulationFramerate, 0, 20)) {
    sim_window->setSimulationFramerate(simulationFramerate);
  }

  if (sim_window->getIsPaused()) {
    if (ImGui::Button("Run"))
      sim_window->setPaused(false);
  } else {
    if (ImGui::Button("Pause"))
      sim_window->setPaused(true);
  }

  ImGui::EndGroup();
  ImGui::End();
}

void showSettingsUI() {
  ImGui::Begin("Simulation settings");

  if (ImGui::CollapsingHeader("Size")) {
    if (ImGui::InputInt("width", &simulationSettings.simulation_width)) {
      if (simulationSettings.simulation_width <= 0) {
        simulationSettings.simulation_width = 1;
      } else if (simulationSettings.simulation_width > 1000) {
        simulationSettings.simulation_width = 1000;
      }
    }
    if (ImGui::InputInt("height", &simulationSettings.simulation_height)) {
      if (simulationSettings.simulation_height <= 0) {
        simulationSettings.simulation_height = 1;
      } else if (simulationSettings.simulation_height > 1000) {
        simulationSettings.simulation_height = 1000;
      }
    }
  }

  static const char* item_names[] = {"Square Net", "Triangle Net"};

  if (ImGui::Combo("Select net", &simulationSettings.selected_builder,
                   item_names, IM_ARRAYSIZE(item_names))) {
  }

  if (ImGui::Button("Reload")) {
    reload = true;
  }

  ImGui::End();
}

SimulationResult run(RenderWindow& window) {
  sf::Clock deltaClock;

  reload = false;
  while (window.isOpen() && !reload) {
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

    showViewUI();
    showSettingsUI();
    showControlPanelUI();

    // Render ImGui on top of the window
    ImGui::SFML::Render(window);

    sf::sleep(sf::seconds(1.f / IMGUI_WINDOW_FRAMERATE -
                          deltaClock.getElapsedTime().asSeconds()));

    window.display();
  }

  return Reload;
}

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
