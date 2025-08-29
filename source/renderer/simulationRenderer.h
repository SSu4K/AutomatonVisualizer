#pragma once

#include "simulation/simulation.h"
#include "colors/gradient.h"
#include "vertexArrayBuilder.h"
#include <SFML/Graphics.hpp>
#include "utils.hpp"

typedef function<sf::Color(const sf::Color&, const sf::Color&, float)> ColorInterpolationFunction;

class SimulationRenderer{
    protected:
    shared_ptr<Simulation> simulation;
    shared_ptr<IVertexArrayBuilder> builder;
    size_t segment_size;
    size_t segment_count;

    // index - new_colors
    // !index - old_colors 
    bool color_buffer_index;
    std::vector<sf::Color> color_buffer[2];

    ColorInterpolationFunction interp_func =
      utils::powrp<6>;

    public:
    sf::VertexArray vertex_array;
    sf::VertexArray net;

    SimulationRenderer() = default;
    SimulationRenderer(shared_ptr<Simulation> simulation, shared_ptr<IVertexArrayBuilder> builder);

    ~SimulationRenderer() = default;
    void update_vertex_array(double t);
    void push_color_buffer();
    sf::Vector2f get_simulation_size() const;
    void render(sf::RenderTarget& target, const Simulation& sim) const;
};