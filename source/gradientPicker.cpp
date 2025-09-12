#include "gradientPicker.h"

static bool colorsEditor(std::vector<std::array<float, 4>>& colors) {
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
static bool positionsEditor(std::vector<float>& positions) {
  bool update = false;
  for (size_t i = 0; i < positions.size(); i++) {
    float min = (i == 0) ? 0 : positions[i - 1];
    float max = (i == positions.size() - 1) ? 1 : positions[i + 1];
    string label = "Node pos " + std::to_string(i + 1);
    update |= ImGui::SliderFloat(label.c_str(), &positions[i], min, max);
  }

  if (ImGui::Button("Distribute")) {
    positions[0] = 0;
    positions[positions.size()-1] = 1;
    float t = 1.0f/(positions.size()-1);
    for (size_t i = 1; i < positions.size() - 1; i++) {
      positions[i] = t*i;
    }
    update = true;
  }
  return update;
}

static bool gradientEditor(Gradient& gradient) {
  std::vector<std::array<float, 4>> colors = {};
  std::vector<float> positions = {};
  for (auto node : gradient.nodes) {
    colors.push_back({node.color.r / 255.0f, node.color.g / 255.0f,
                      node.color.b / 255.0f, node.color.a / 255.0f});
    positions.push_back(node.pos);
  }

  if (colorsEditor(colors) || positionsEditor(positions)) {
    std::vector<ColorNode> updated_nodes;
    for (int i = 0; i < gradient.nodes.size(); i++) {
      updated_nodes.push_back(
          {sf::Color(colors[i][0] * 255, colors[i][1] * 255, colors[i][2] * 255,
                     colors[i][3] * 255),
           positions[i]});
    }
    gradient = Gradient(updated_nodes, gradient.get_mode());
    return true;
  }
  return false;
}

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
  return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

bool MySliderScalar(const char* label,
                    float* p_data,
                    const float p_min,
                    const float p_max,
                    const float width = 100,
                    const char* format = "%f",
                    ImGuiSliderFlags flags = 0) {
  ImGuiDataType data_type = ImGuiDataType_Float;
  using namespace ImGui;
  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& g = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID id = window->GetID(label);
  const float w = width;
  const float h = 40;

  const ImVec2 old_cursor = window->DC.CursorPos;

  const ImRect frame_bb(window->DC.CursorPos + ImVec2(p_min * width, 0),
                        window->DC.CursorPos + ImVec2(p_max * width, h));
  const ImRect total_bb(frame_bb.Min, frame_bb.Max);

  const bool temp_input_allowed = false;
  ItemSize(total_bb);
  if (!ItemAdd(total_bb, id, &frame_bb, 0))
    return false;

  // Default format string when passing NULL
  if (format == NULL)
    format = DataTypeGetInfo(data_type)->PrintFmt;

  const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.InFlags);

  const bool clicked = hovered && IsMouseClicked(0, id);
  const bool make_active = (clicked || g.NavActivateId == id);
  if (make_active && clicked)
    SetKeyOwner(ImGuiKey_MouseLeft, id);

  if (make_active) {
    SetActiveID(id, window);
    SetFocusID(id, window);
    FocusWindow(window);
    g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
  }

  // Draw frame
  const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive
                                      : hovered        ? ImGuiCol_FrameBgHovered
                                                       : ImGuiCol_FrameBg);
  RenderNavHighlight(frame_bb, id);
  RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true,
              g.Style.FrameRounding);

  // Slider behavior
  ImRect grab_bb;
  const bool value_changed = SliderBehavior(
      frame_bb, id, data_type, p_data, &p_min, &p_max, format, flags, &grab_bb);
  if (value_changed)
    MarkItemEdited(id);

  // Render grab
  if (grab_bb.Max.x > grab_bb.Min.x)
    window->DrawList->AddRectFilled(
        grab_bb.Min, grab_bb.Max,
        GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive
                                     : ImGuiCol_SliderGrab),
        style.GrabRounding);

  window->DC.CursorPos = old_cursor;
  return value_changed;
}

static void renderGradientTexture(sf::RenderTexture* texture,
                                  Gradient& gradient,
                                  bool update_gradient) {
  ImGuiWindow* window = ImGui::GetCurrentWindow();

  sf::Vector2f windowSize(ImGui::GetContentRegionAvail().x, 40.f);

  float triangle_height = 10;
  float triangle_width = 10;

  if (!update_gradient && texture->getSize().x == windowSize.x) {
    return;
  }

  if (texture->getSize().x != windowSize.x) {
    texture->create(static_cast<unsigned int>(windowSize.x),
                    static_cast<unsigned int>(windowSize.y));
  }

  sf::VertexArray gradient_array(sf::TriangleStrip,
                                 2 * (gradient.nodes.size() + 2));
  sf::VertexArray triangle_array(sf::Triangles, 3 * gradient.nodes.size());

  gradient_array[0] = sf::Vertex({0, 0.f}, gradient.nodes[0].color);
  gradient_array[1] =
      sf::Vertex({0, windowSize.y - triangle_width}, gradient.nodes[0].color);

  gradient_array[gradient_array.getVertexCount() - 2] = sf::Vertex(
      {windowSize.x, 0.f}, gradient.nodes[gradient.nodes.size() - 1].color);
  gradient_array[gradient_array.getVertexCount() - 1] =
      sf::Vertex({windowSize.x, windowSize.y - triangle_width},
                 gradient.nodes[gradient.nodes.size() - 1].color);

  for (size_t i = 0; i < gradient.nodes.size(); i++) {
    float x = gradient.nodes[i].pos * windowSize.x;
    sf::Color col = gradient.nodes[i].color;
    gradient_array[2 * (i + 1)] = sf::Vertex({x, 0.f}, col);
    gradient_array[2 * (i + 1) + 1] =
        sf::Vertex({x, windowSize.y - triangle_width}, col);
    float y = triangle_height / 2;
    triangle_array[3 * i] =
        sf::Vertex({x, windowSize.y - triangle_height}, col);
    triangle_array[3 * i + 1] =
        sf::Vertex({x - triangle_width, windowSize.y - y}, col);
    triangle_array[3 * i + 2] =
        sf::Vertex({x + triangle_width, windowSize.y - y}, col);
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

  return update_gradient;
}

#undef IMGUI_DEFINE_MATH_OPERATORS