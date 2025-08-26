#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "simulation.h"

class IVertexArrayBuilder {
public:
    virtual ~IVertexArrayBuilder() = default;
    virtual size_t get_segment_size() const = 0;
    virtual float get_x_ratio() const = 0;
    virtual float get_y_ratio() const = 0;

    virtual sf::VertexArray build(const Simulation& sim) const = 0;
    virtual sf::VertexArray build_grid(const Simulation& sim, const sf::Color color) const {
        return sf::VertexArray();
    }
};

class SquareGridBuilder : public IVertexArrayBuilder {
public:
    size_t get_segment_size() const override;
    float get_x_ratio() const override;
    float get_y_ratio() const override;
    sf::VertexArray build(const Simulation& simulation) const override;
    sf::VertexArray build_grid(const Simulation& simulation, const sf::Color color) const override;
};

class TriangleGridBuilder : public IVertexArrayBuilder {
public:
    size_t get_segment_size() const override;
    float get_x_ratio() const override;
    float get_y_ratio() const override;
    sf::VertexArray build(const Simulation& simulation) const override;
    sf::VertexArray build_grid(const Simulation& simulation, const sf::Color color) const override;
};

