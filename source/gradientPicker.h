#pragma once

#include <imgui-SFML.h>
#include <imgui.h>
#include <vector>
#include <array>
#include <map>
#include "colors/gradient.h"
#include <memory>

namespace ui {
static std::map<ImGuiID, unique_ptr<sf::RenderTexture>> gradient_textures;
bool GradientPicker(const char* label, Gradient &v);

}  // namespace ui