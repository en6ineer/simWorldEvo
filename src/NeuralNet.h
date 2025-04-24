#pragma once

#include <vector>
#include <random>

struct BrainInput {
    float energy;        // Energy / max_energy
    float density;      // Number of neighbors / 8
    float sun_intensity;// Field::sun_intensity
};

struct BrainOutput {
    float move_up;
    float move_right;
    float move_down;
    float move_left;
    float attack;
    float reproduce;
};

class NeuralNet {
private:
    std::vector<std::vector<float>> weights1; // Input -> Hidden
    std::vector<std::vector<float>> weights2; // Hidden -> Output
    std::vector<float> biases1;               // Hidden layer biases
    std::vector<float> biases2;               // Output layer biases
    static constexpr int INPUT_SIZE = 3;       // energy, density, sun_intensity
    static constexpr int HIDDEN_SIZE = 6;     // 6 hidden neurons
    static constexpr int OUTPUT_SIZE = 6;      // 4 moves, attack, reproduce
    std::mt19937 rng;

    float activate(float x) const { return std::tanh(x); }

public:
    NeuralNet();
    BrainOutput compute(const BrainInput& input) const;
    void mutate(float mutation_rate = 0.05f);
};