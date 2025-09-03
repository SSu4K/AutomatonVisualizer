#pragma once
#include <imgui-SFML.h>
#include <imgui.h>
#include "window.h"

struct SimulationSettings {
  int simulation_width = 100;
  int simulation_height = 100;
  int simulation_framerate = 10;
  int selected_builder = 0;
  int selected_rule = 0;
  int fill_type = 0;
  int selected_point = 0;
  float fill_percentage = 10;
  bool clamped = false;
};

namespace ui {

bool InputIntBouded(const char* label,
                    int* v,
                    int min,
                    int max,
                    int step = 1,
                    int step_fast = 100,
                    ImGuiInputTextFlags flags = 0);

bool InputFloatBouded(const char* label,
                      float* v,
                      float min,
                      float max,
                      float step = 0,
                      float step_fast = 0,
                      char* format = (char*)"%.3f",
                      ImGuiInputTextFlags flags = 0);
bool showViewUI();
bool showControlPanelUI();
bool showSettingsUI();

}  // namespace ui