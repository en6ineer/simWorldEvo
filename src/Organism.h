#pragma once

#include <SDL2/SDL.h>
#include "Types.h"
#include "NeuralNet.h"

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
    int direction; // 0: up, 1: right, 2: down, 3: left
    Color color;
    int mutation_markers[MUTATION_MARKERS_COUNT];
    int age;
    NeuralNet* brain;
    std::mt19937 rng;

    void photosynthesis(Field* field);
    void move(Field* field);
    void attack(Field* field);
    void reproduce(Field* field);
    float get_density(Field* field) const;
    void mutate_type(Field* field);
    void change_marker();

public:
    Organism(int x, int y, OrganismType type = OrganismType::Photosynthetic);
    Organism(const Organism& parent, int x, int y); // For reproduction
    ~Organism();
    void update(Field* field);
    void draw(SDL_Renderer* renderer) const;
    int get_x() const { return x; }
    int get_y() const { return y; }
    float get_energy() const { return energy; }
    OrganismType get_type() const { return type; }
    const Color& get_color() const { return color; }
    int find_kinship(const Organism* other) const;
};