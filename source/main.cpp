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
  int fill_type = 0;
  int selected_point = 0;
  float fill_percentage = 10;
  bool clamped = false;
};

SimulationSettings simulationSettings;
shared_ptr<SimulationWindow> sim_window;

int simulationFramerate = 10;
bool reload = true;

void build_simulation_window() {
  size_vector size = {size_t(simulationSettings.simulation_width),
                      size_t(simulationSettings.simulation_height)};

  BSRule carpet_rule({2, 3, 4, 5}, {});
  // BSRule bs_gol_rule(6152);

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

  if(simulationSettings.fill_type == 0){
    simulation.random_fill(simulationSettings.fill_percentage/100.0);
  }
  else{
    int point = simulationSettings.selected_point;
    if(point == 0){
      simulation.set_cell_value(size[0]/2, size[1]/2, true);
    }
    else{
      point--;
      simulation.set_cell_value(size[0] * (point%2), size[1] * (point < 2), true);
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

bool InputIntBouded(const char* label,
                    int* v,
                    int min,
                    int max,
                    int step = 1,
                    int step_fast = 100,
                    ImGuiInputTextFlags flags = 0) {
  if (ImGui::InputInt(label, v, step, step_fast, flags)) {
    if (*v < min) {
      *v = min;
    } else if (*v > max) {
      *v = max;
    }
    return true;
  }
  return false;
}

bool InputFloatBouded(const char* label,
                      float* v,
                      float min,
                      float max,
                      float step = 0,
                      float step_fast = 0,
                      char* format = "%.3f",
                      ImGuiInputTextFlags flags = 0) {
  if (ImGui::InputFloat(label, v, step, step_fast, format, flags)) {
    if (*v < min) {
      *v = min;
    } else if (*v > max) {
      *v = max;
    }
    return true;
  }
  return false;
}

void showSettingsUI() {
  ImGui::Begin("Simulation settings");

  if (ImGui::CollapsingHeader("Shape")) {
    InputIntBouded("width", &simulationSettings.simulation_width, 1, 300);
    InputIntBouded("height", &simulationSettings.simulation_height, 1, 300);
    static const char* net_item_names[] = {"Square Net", "Triangle Net"};

    if (ImGui::Combo("Net", &simulationSettings.selected_builder,
                     net_item_names, IM_ARRAYSIZE(net_item_names))) {
    }
  }

  if (ImGui::CollapsingHeader("Simulation")) {

    ImGui::Checkbox("Clamped", &simulationSettings.clamped);

    static const char* rule_item_names[] = {"Game Of Life", "Carpet", "Flower"};
    if (ImGui::Combo("Rule", &simulationSettings.selected_rule, rule_item_names,
                     IM_ARRAYSIZE(rule_item_names))) {
    }

    static const char* fill_type_item_names[] = {"Random", "Point"};
    if (ImGui::Combo("Fill", &simulationSettings.fill_type,
                     fill_type_item_names,
                     IM_ARRAYSIZE(fill_type_item_names))) {
    }

    if (simulationSettings.fill_type == 0) {
      InputFloatBouded("Percentage", &simulationSettings.fill_percentage,
                        0, 100, 0.1);
    } else if (simulationSettings.fill_type == 1) {
      static const char* fill_item_names[] = {"center", "top-left", "top-right",
                                              "bottom-left", "bottom-right"};
      if (ImGui::Combo("Point", &simulationSettings.selected_point,
                       fill_item_names, IM_ARRAYSIZE(fill_item_names))) {
      }
    }
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
