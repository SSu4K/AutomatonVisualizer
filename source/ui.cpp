#include "ui.h"

extern SimulationSettings simulationSettings;
extern shared_ptr<SimulationWindow> sim_window;

bool ui::InputIntBouded(const char* label,
                    int* v,
                    int min,
                    int max,
                    int step,
                    int step_fast,
                    ImGuiInputTextFlags flags) {
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

bool ui::InputFloatBouded(const char* label,
                      float* v,
                      float min,
                      float max,
                      float step,
                      float step_fast,
                      char* format,
                      ImGuiInputTextFlags flags) {
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

bool ui::GradientPicker(const char* label, Gradient &v){
  std::vector<std::array<float, 4>> colors;
  for(auto node: v.nodes){
    colors.push_back({node.color.r/255.0f, node.color.g/255.0f, node.color.b/255.0f, node.color.a/255.0f});
  }
  bool update = false;
  for(size_t i=0; i<colors.size(); i++){
    ImGui::BeginGroup();
    string label = "Node #"+std::to_string(i+1);
    if(ImGui::ColorEdit3(label.c_str(), &(colors[i][0]), ImGuiColorEditFlags_NoTooltip)){
      update |= true;
    }
    ImGui::SameLine();
    ImGui::PushID(i);
    if(ImGui::Button("Remove")){
      colors.erase(colors.begin() + i, colors.begin() + i + 1);
      i--;
      update |= true;
    }
    ImGui::PopID();
    ImGui::EndGroup();
  }
  if(ImGui::Button("Add##AddGradientButton")){
    colors.push_back({0, 0, 0, 0});
    update |= true;
  }

  if(update){
    std::vector<sf::Color> updated_colors;
    for(auto color: colors){
      updated_colors.push_back(sf::Color(color[0]*255, color[1]*255, color[2]*255, color[3]*255));
    }
    v = Gradient(updated_colors, v.get_mode());

  }
  return true;
}

bool ui::showViewUI() {
  ImGui::Begin("View", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Image(sim_window->getTexture());
  ImGui::End();
  return true;
}

bool ui::showControlPanelUI() {
  ImGui::Begin("Control Panel");
  ImGui::SameLine();
  ImGui::BeginGroup();
  ImGui::Text("Simulation running...");

  if (ImGui::SliderInt("Framerate", &simulationSettings.simulation_framerate, 0, 20)) {
    sim_window->setSimulationFramerate(simulationSettings.simulation_framerate);
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
  return true;
}

bool ui::showSettingsUI() {
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
      InputFloatBouded("Percentage", &simulationSettings.fill_percentage, 0,
                       100, 0.1);
    } else if (simulationSettings.fill_type == 1) {
      static const char* fill_item_names[] = {"center", "top-left", "top-right",
                                              "bottom-left", "bottom-right"};
      if (ImGui::Combo("Point", &simulationSettings.selected_point,
                       fill_item_names, IM_ARRAYSIZE(fill_item_names))) {
      }
    }
  }
  if (ImGui::Button("Reload")) {
    ImGui::End();
    return false;
  }

  ImGui::End();
  return true;
}