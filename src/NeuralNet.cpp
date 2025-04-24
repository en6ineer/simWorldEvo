#include "NeuralNet.h"
#include <cmath>
#include <random>

NeuralNet::NeuralNet() : rng(std::random_device{}()) {
    weights1.resize(INPUT_SIZE, std::vector<float>(HIDDEN_SIZE));
    weights2.resize(HIDDEN_SIZE, std::vector<float>(OUTPUT_SIZE));
    biases1.resize(HIDDEN_SIZE);
    biases2.resize(OUTPUT_SIZE);

    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (auto& row : weights1) for (auto& w : row) w = dist(rng);
    for (auto& row : weights2) for (auto& w : row) w = dist(rng);
    for (auto& b : biases1) b = dist(rng);
    for (auto& b : biases2) b = dist(rng);
}

BrainOutput NeuralNet::compute(const BrainInput& input) const {
    std::vector<float> inputs = {input.energy, input.density, input.sun_intensity};
    std::vector<float> hidden(HIDDEN_SIZE, 0.0f);

    // Input -> Hidden
    for (int i = 0; i < INPUT_SIZE; ++i) {
        for (int j = 0; j < HIDDEN_SIZE; ++j) {
            hidden[j] += inputs[i] * weights1[i][j];
        }
    }
    for (int j = 0; j < HIDDEN_SIZE; ++j) {
        hidden[j] = activate(hidden[j] + biases1[j]);
    }

    // Hidden -> Output
    std::vector<float> outputs(OUTPUT_SIZE, 0.0f);
    for (int i = 0; i < HIDDEN_SIZE; ++i) {
        for (int j = 0; j < OUTPUT_SIZE; ++j) {
            outputs[j] += hidden[i] * weights2[i][j];
        }
    }
    for (int j = 0; j < OUTPUT_SIZE; ++j) {
        outputs[j] = activate(outputs[j] + biases2[j]);
    }

    return {outputs[0], outputs[1], outputs[2], outputs[3], outputs[4], outputs[5]};
}

void NeuralNet::mutate(float mutation_rate) {
    std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
    std::bernoulli_distribution mutate_dist(mutation_rate);

    for (auto& row : weights1) for (auto& w : row) if (mutate_dist(rng)) w += dist(rng);
    for (auto& row : weights2) for (auto& w : row) if (mutate_dist(rng)) w += dist(rng);
    for (auto& b : biases1) if (mutate_dist(rng)) b += dist(rng);
    for (auto& b : biases2) if (mutate_dist(rng)) b += dist(rng);
}