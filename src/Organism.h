#pragma once

#include <SDL2/SDL.h>

class Field;

enum class OrganismType {
    Photosynthetic,
    Carnivorous
};

class Organism {
private:
    int x, y;
    float energy;
    OrganismType type;
    static constexpr float PHOTOSYNTHESIS_RATE = 0.5f;
    static constexpr float MUTATION_RATE = 0.01f;
    static constexpr float ENERGY_TO_REPRODUCE = 100.0f;

public:
    Organism(int x, int y);
    void update(Field* field);
    void draw(SDL_Renderer* renderer) const;
    int get_x() const { return x; }
    int get_y() const { return y; }
    float get_energy() const { return energy; }
    OrganismType get_type() const { return type; }
};