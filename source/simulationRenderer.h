#pragma once

#include "simulation.h"
#include "colors/gradient.h"
#include "vertexArrayBuilder.h"
#include <SFML/Graphics.hpp>
#include "utils.hpp"

class SimulationRenderer{
    protected:
    shared_ptr<IVertexArrayBuilder> builder;
    shared_ptr<Simulation> simulation;
    size_t segment_size;
    size_t segment_count;

    // index - new_colors
    // !index - old_colors 
    bool color_buffer_index;
    std::vector<sf::Color> color_buffer[2];

    function<sf::Color(const sf::Color&, const sf::Color&, float)> interp_func =
      utils::powrp<6>;

    public:
    sf::VertexArray vertex_array;
    sf::VertexArray grid;

    SimulationRenderer() = default;
    SimulationRenderer(shared_ptr<Simulation> simulation, shared_ptr<IVertexArrayBuilder> builder);

    ~SimulationRenderer();
    void update_vertex_array(double t);
    void push_color_buffer();
};