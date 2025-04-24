#include "Organism.h"
#include "Field.h"
#include <cstdlib>

Organism::Organism(int x, int y) : x(x), y(y), energy(50.0f), type(OrganismType::Photosynthetic) {}

void Organism::update(Field* field) {
    // Gain energy
    if (type == OrganismType::Photosynthetic) {
        energy += PHOTOSYNTHESIS_RATE;
    } else {
        // Carnivorous: try to eat nearby organisms
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                Organism* neighbor = field->get_organism(x + dx, y + dy);
                if (neighbor && neighbor->get_type() == OrganismType::Photosynthetic) {
                    energy += neighbor->get_energy();
                    delete neighbor;
                    field->set_organism(x + dx, y + dy, nullptr);
                }
            }
        }
    }

    // Move randomly
    if (rand() % 100 < 10) { // 10% chance to move
        int dx = (rand() % 3) - 1; // -1, 0, 1
        int dy = (rand() % 3) - 1;
        int new_x = x + dx;
        int new_y = y + dy;
        if (field->is_in_bounds(new_x, new_y) && !field->get_organism(new_x, new_y)) {
            field->set_organism(x, y, nullptr); // Clear current position
            x = new_x;
            y = new_y;
            field->set_organism(x, y, this); // Set new position
        }
    }

    // Mutation
    if ((rand() % 10000) / 10000.0f < MUTATION_RATE) {
        type = OrganismType::Carnivorous;
    }

    // Reproduce
    if (energy >= ENERGY_TO_REPRODUCE) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int new_x = x + dx;
                int new_y = y + dy;
                if (field->is_in_bounds(new_x, new_y) && !field->get_organism(new_x, new_y)) {
                    Organism* child = new Organism(new_x, new_y);
                    child->type = type;
                    field->add_organism(child);
                    energy -= ENERGY_TO_REPRODUCE / 2;
                    return;
                }
            }
        }
    }

    // Die if energy is too low
    if (energy <= 0) {
        field->set_organism(x, y, nullptr);
        delete this;
    }
}

void Organism::draw(SDL_Renderer* renderer) const {
    SDL_Rect rect = { FIELD_X + x * CELL_SIZE, FIELD_Y + y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
    if (type == OrganismType::Photosynthetic) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    }
    SDL_RenderFillRect(renderer, &rect);
}