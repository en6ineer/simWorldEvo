#pragma once

#include <SDL2/SDL.h>
#include <vector>

class Organism;

// Constants
constexpr int FIELD_WIDTH = 120;  // Cells
constexpr int FIELD_HEIGHT = 60;  // Cells
constexpr int CELL_SIZE = 10;     // Pixels
constexpr int FIELD_X = 10;       // Screen offset
constexpr int FIELD_Y = 10;

class Field {
private:
    std::vector<std::vector<Organism*>> cells; // 2D grid of organisms
    SDL_Renderer* renderer;
    uint32_t tick_count = 0;
    bool simulate = true;
    float sun_intensity = 1.0f; // Added for photosynthesis

public:
    Field(SDL_Renderer* renderer);
    ~Field();

    void tick();
    void draw();
    void restart();
    bool is_simulating() const { return simulate; }
    bool add_organism(Organism* organism);
    bool is_in_bounds(int x, int y) const;
    Organism* get_organism(int x, int y) const;
    void set_organism(int x, int y, Organism* organism); // New method
    void toggle_simulation() { simulate = !simulate; }
    uint32_t get_tick_count() const { return tick_count; }
    uint32_t get_organism_count() const;
    float get_sun_intensity() const { return sun_intensity; }
    void set_sun_intensity(float intensity) { sun_intensity = std::max(0.0f, std::min(2.0f, intensity)); }
};