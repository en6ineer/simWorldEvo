#include "Organism.h"
#include "Field.h"
#include <random>
#include <algorithm>

Organism::Organism(int x, int y, OrganismType type)
    : x(x), y(y), energy(50.0f), type(type), direction(rand() % 4), age(0), rng(std::random_device{}()) {
    if (type == OrganismType::Photosynthetic) {
        color = Color(0, 255, 0); // Green
    } else {
        color = Color(255, 0, 0); // Red
    }
    for (int i = 0; i < MUTATION_MARKERS_COUNT; ++i) {
        mutation_markers[i] = std::uniform_int_distribution<int>(0, 1000)(rng);
    }
}

Organism::Organism(const Organism& parent, int x, int y)
    : x(x), y(y), energy(parent.energy * 0.5f), type(parent.type), direction(rand() % 4),
      color(parent.color), age(0), rng(std::random_device{}()) {
    std::copy(parent.mutation_markers, parent.mutation_markers + MUTATION_MARKERS_COUNT, mutation_markers);
    change_marker();
}

void Organism::photosynthesis(Field* field) {
    if (type == OrganismType::Photosynthetic) {
        float free_space = 8.0f - get_density(field); // 0 to 8 free cells
        float rate = PHOTOSYNTHESIS_BASE_RATE * (free_space / 8.0f) * field->get_sun_intensity();
        energy += rate;
        if (energy > MAX_ENERGY) energy = MAX_ENERGY;
    }
}

void Organism::move(Field* field) {
    static const int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; // up, right, down, left
    int new_x = x + directions[direction][0];
    int new_y = y + directions[direction][1];
    if (field->is_in_bounds(new_x, new_y) && !field->get_organism(new_x, new_y)) {
        field->set_organism(x, y, nullptr);
        x = new_x;
        y = new_y;
        field->set_organism(x, y, this);
        energy -= MOVE_COST;
    }
}

void Organism::attack(Field* field) {
    static const int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    int target_x = x + directions[direction][0];
    int target_y = y + directions[direction][1];
    Organism* neighbor = field->get_organism(target_x, target_y);
    if (neighbor && neighbor->get_type() == OrganismType::Photosynthetic) {
        energy += neighbor->get_energy();
        if (energy > MAX_ENERGY) energy = MAX_ENERGY;
        delete neighbor;
        field->set_organism(target_x, target_y, nullptr);
        energy -= ATTACK_COST;
    }
}

void Organism::reproduce(Field* field) {
    if (age < FERTILITY_DELAY || energy < REPRODUCE_COST) return;
    static const int offsets[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    std::vector<int> free_cells;
    for (int i = 0; i < 8; ++i) {
        int new_x = x + offsets[i][0];
        int new_y = y + offsets[i][1];
        if (field->is_in_bounds(new_x, new_y) && !field->get_organism(new_x, new_y)) {
            free_cells.push_back(i);
        }
    }
    if (!free_cells.empty()) {
        int idx = free_cells[std::uniform_int_distribution<size_t>(0, free_cells.size() - 1)(rng)];
        int new_x = x + offsets[idx][0];
        int new_y = y + offsets[idx][1];
        Organism* child = new Organism(*this, new_x, new_y);
        // Chance to mutate child to carnivorous if density is high
        if (type == OrganismType::Photosynthetic && get_density(field) >= 6.0f) {
            float mutation_chance = DENSITY_MUTATION_FACTOR * get_density(field);
            if (std::uniform_real_distribution<float>(0.0f, 1.0f)(rng) < mutation_chance) {
                child->type = OrganismType::Carnivorous;
                child->color = Color(255, 0, 0);
            }
        }
        field->add_organism(child);
        energy -= REPRODUCE_COST;
    }
}

float Organism::get_density(Field* field) const {
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            if (field->get_organism(x + dx, y + dy)) ++count;
        }
    }
    return static_cast<float>(count);
}

void Organism::mutate_type(Field* field) {
    if (type == OrganismType::Photosynthetic && age >= FERTILITY_DELAY) {
        float density = get_density(field);
        if (density >= 6.0f && energy < 0.5f * MAX_ENERGY) {
            float mutation_chance = DENSITY_MUTATION_FACTOR * density * (1.0f - energy / MAX_ENERGY);
            if (std::uniform_real_distribution<float>(0.0f, 1.0f)(rng) < mutation_chance) {
                type = OrganismType::Carnivorous;
                color = Color(255, 0, 0);
            }
        }
    }
}

void Organism::change_marker() {
    int idx = std::uniform_int_distribution<int>(0, MUTATION_MARKERS_COUNT - 1)(rng);
    mutation_markers[idx] = std::uniform_int_distribution<int>(0, 1000)(rng);
}

int Organism::find_kinship(const Organism* other) const {
    int matches = 0;
    for (int i = 0; i < MUTATION_MARKERS_COUNT; ++i) {
        if (mutation_markers[i] == other->mutation_markers[i]) ++matches;
    }
    return matches;
}

bool Organism::find_free_direction(Field* field, int& best_direction) const {
    static const int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    float min_density = 9.0f; // More than max possible density (8)
    best_direction = -1;
    for (int i = 0; i < 4; ++i) {
        int new_x = x + directions[i][0];
        int new_y = y + directions[i][1];
        if (field->is_in_bounds(new_x, new_y) && !field->get_organism(new_x, new_y)) {
            // Calculate density at new position
            int count = 0;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    if (field->get_organism(new_x + dx, new_y + dy)) ++count;
                }
            }
            float density = static_cast<float>(count);
            if (density < min_density) {
                min_density = density;
                best_direction = i;
            }
        }
    }
    return best_direction != -1;
}

void Organism::update(Field* field) {
    ++age;
    photosynthesis(field);
    mutate_type(field);

    if (type == OrganismType::Photosynthetic) {
        // Try to reproduce if enough energy
        if (energy >= REPRODUCE_COST) {
            reproduce(field);
        }
        // If no free space for reproduction, move to a less dense area
        else if (get_density(field) >= 6.0f) {
            int best_direction;
            if (find_free_direction(field, best_direction)) {
                direction = best_direction;
                move(field);
            }
        }
    } else {
        // Carnivorous: attack if possible, otherwise move randomly
        attack(field);
        if (std::uniform_real_distribution<float>(0.0f, 1.0f)(rng) < 0.2f) { // 20% chance to move
            direction = std::uniform_int_distribution<int>(0, 3)(rng);
            move(field);
        }
    }

    // Die if energy <= 0 or too old || age >= 100
    if (energy <= 0) {
        field->set_organism(x, y, nullptr);
        delete this;
    }
}

void Organism::draw(SDL_Renderer* renderer) const {
    SDL_Rect rect = { FIELD_X + x * CELL_SIZE, FIELD_Y + y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &rect);
}