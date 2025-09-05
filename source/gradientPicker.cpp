#include "gradientPicker.h"

static bool colorsEditor(std::vector<std::array<float, 4>> &colors){
  bool update = false;
  for (size_t i = 0; i < colors.size(); i++) {
    ImGui::BeginGroup();
    string label = "Node #" + std::to_string(i + 1);
    if (ImGui::ColorEdit3(label.c_str(), &(colors[i][0]),
                          ImGuiColorEditFlags_NoTooltip)) {
      update |= true;
    }
    ImGui::SameLine();
    ImGui::PushID(i);
    if (ImGui::Button("Remove")) {
      colors.erase(colors.begin() + i, colors.begin() + i + 1);
      i--;
      update |= true;
    }
    ImGui::PopID();
    ImGui::EndGroup();
  }
  if (ImGui::Button("Add##AddGradientButton")) {
    colors.push_back({0, 0, 0, 1});
    update |= true;
  }
  return update;
}

static bool gradientEditor(Gradient& gradient){
  std::vector<std::array<float, 4>> colors = {};
  for (auto node : gradient.nodes) {
    colors.push_back({node.color.r / 255.0f, node.color.g / 255.0f,
                      node.color.b / 255.0f, node.color.a / 255.0f});
  }

  if (colorsEditor(colors)) {
    std::vector<sf::Color> updated_colors;
    for (auto color : colors) {
      updated_colors.push_back(sf::Color(color[0] * 255, color[1] * 255,
                                         color[2] * 255, color[3] * 255));
    }
    gradient = Gradient(updated_colors, gradient.get_mode());
    return true;
  }
  return false;
}

static void renderGradientTexture(sf::RenderTexture* texture, Gradient &gradient, bool update_gradient){
  sf::Vector2f windowSize(ImGui::GetContentRegionAvail().x, 40.f);
  float triangle_height = 10;
  float triangle_width = 10;

  if(!update_gradient && texture->getSize().x == windowSize.x){
    return;
  }

  if (texture->getSize().x != windowSize.x) {
    texture->create(static_cast<unsigned int>(windowSize.x),
                    static_cast<unsigned int>(windowSize.y));
  }

  sf::VertexArray gradient_array(sf::TriangleStrip, 2 * gradient.nodes.size());
  sf::VertexArray triangle_array(sf::Triangles, 3 * gradient.nodes.size());

  for (size_t i = 0; i < gradient.nodes.size(); i++) {
    float x = gradient.nodes[i].pos * windowSize.x;
    sf::Color col = gradient.nodes[i].color;
    gradient_array[2 * i] = sf::Vertex({x, 0.f}, col);
    gradient_array[2 * i + 1] =
        sf::Vertex({x, windowSize.y - triangle_width}, col);

    triangle_array[3 * i] = sf::Vertex({x, windowSize.y - triangle_width}, col);
    triangle_array[3 * i + 1] =
        sf::Vertex({x - triangle_width, windowSize.y}, col);
    triangle_array[3 * i + 2] =
        sf::Vertex({x + triangle_width, windowSize.y}, col);
  }
  texture->clear(sf::Color::Transparent);
  texture->draw(gradient_array);
  texture->draw(triangle_array);
  texture->display();
}

bool ui::GradientPicker(const char* label, Gradient& v) {
  ImGuiID id = ImGui::GetID(label);
  if (!gradient_textures.count(id)) {
    gradient_textures[id] = std::make_unique<sf::RenderTexture>();
  }
  sf::RenderTexture* texture = gradient_textures[id].get();


  ImGui::Text(label);
  ImGui::Image(texture->getTexture());
  ImGui::Text("Color nodes:");

  bool update_gradient = gradientEditor(v);
  renderGradientTexture(texture, v, update_gradient);

  return true;
}