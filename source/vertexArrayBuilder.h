#pragma once
#include <SFML/Graphics.hpp>
#include "simulation.h"

class IVertexArrayBuilder {
public:
    virtual size_t get_segment_size() const = 0;
    virtual ~IVertexArrayBuilder() = default;

    virtual sf::VertexArray build(const Simulation& sim) const = 0;

    virtual sf::VertexArray build_grid(const Simulation& sim, const sf::Color color) const {
        return sf::VertexArray();
    }
};

class SquareGridBuilder : public IVertexArrayBuilder {
public:
    size_t get_segment_size() const override;
    sf::VertexArray build(const Simulation& simulation) const override;
    sf::VertexArray build_grid(const Simulation& simulation, const sf::Color color) const override;
};

