#include "Field.h"
#include "Organism.h"
#include <cstdlib>

Field::Field(SDL_Renderer* renderer) : renderer(renderer) {
    cells.resize(FIELD_WIDTH, std::vector<Organism*>(FIELD_HEIGHT, nullptr));

    // Spawn initial organisms
    for (int i = 0; i < 50; ++i) {
        int x = rand() % FIELD_WIDTH;
        int y = rand() % FIELD_HEIGHT;
        if (!cells[x][y]) {
            cells[x][y] = new Organism(x, y);
        }
    }
}

Field::~Field() {
    for (auto& row : cells) {
        for (auto& organism : row) {
            delete organism;
        }
    }
}

void Field::tick() {
    if (!simulate) return;
    ++tick_count;

    // Update organisms
    for (int x = 0; x < FIELD_WIDTH; ++x) {
        for (int y = 0; y < FIELD_HEIGHT; ++y) {
            if (cells[x][y]) {
                cells[x][y]->update(this);
            }
        }
    }
}

void Field::draw() {
    // Draw background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    SDL_Rect rect = { FIELD_X, FIELD_Y, FIELD_WIDTH * CELL_SIZE, FIELD_HEIGHT * CELL_SIZE };
    SDL_RenderFillRect(renderer, &rect);

    // Draw organisms
    for (int x = 0; x < FIELD_WIDTH; ++x) {
        for (int y = 0; y < FIELD_HEIGHT; ++y) {
            if (cells[x][y]) {
                cells[x][y]->draw(renderer);
            }
        }
    }
}

bool Field::add_organism(Organism* organism) {
    int x = organism->get_x();
    int y = organism->get_y();
    if (!is_in_bounds(x, y) || cells[x][y]) {
        return false;
    }
    cells[x][y] = organism;
    return true;
}


void Field::restart() {
    // Delete all organisms
    for (auto& row : cells) {
        for (auto& organism : row) {
            delete organism;
            organism = nullptr;
        }
    }

    // Reset tick count and simulation state
    tick_count = 0;
    simulate = true;

    // Spawn new organisms
    for (int i = 0; i < 50; ++i) {
        int x = rand() % FIELD_WIDTH;
        int y = rand() % FIELD_HEIGHT;
        if (!cells[x][y]) {
            cells[x][y] = new Organism(x, y);
        }
    }
}




bool Field::is_in_bounds(int x, int y) const {
    return x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT;
}

Organism* Field::get_organism(int x, int y) const {
    return is_in_bounds(x, y) ? cells[x][y] : nullptr;
}

void Field::set_organism(int x, int y, Organism* organism) {
    if (is_in_bounds(x, y)) {
        cells[x][y] = organism;
    }
}

uint32_t Field::get_organism_count() const {
    uint32_t count = 0;
    for (const auto& row : cells) {
        for (const auto& organism : row) {
            if (organism) ++count;
        }
    }
    return count;
}