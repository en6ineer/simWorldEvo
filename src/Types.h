#pragma once

#include <SDL2/SDL.h>
#include <random>

struct Color {
    Uint8 r, g, b;
    Color(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0) : r(r), g(g), b(b) {}
    void randomize(std::mt19937& rng) {
        std::uniform_int_distribution<Uint8> dist(0, 255);
        r = dist(rng);
        g = dist(rng);
        b = dist(rng);
    }
};

constexpr int MUTATION_MARKERS_COUNT = 4;
constexpr float MOVE_COST = 1.0f;
constexpr float ATTACK_COST = 1.0f;
constexpr float REPRODUCE_COST = 10.0f;
constexpr float PHOTOSYNTHESIS_BASE_RATE = 0.5f;
constexpr float MAX_ENERGY = 100.0f;
constexpr int FERTILITY_DELAY = 100;
constexpr float DENSITY_MUTATION_FACTOR = 0.0000001f; // Chance of mutation per neighbor