//
// Created by arso on 24.3.23.
//

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <Eigen/Dense>
#include <fstream>

#define noAct 0
#define ReLU 1
#define Sigmoid 2
const int num_activation_functions = 3;

using namespace std;

class MultiLayerPerceptron {
public:
    MultiLayerPerceptron(const std::vector<int>& architecture,
                         const std::vector<int>& activation_functions)
            : weights(architecture.size() - 1),
              biases(architecture.size() - 1),
              activation_functions(activation_functions),
              architecture(architecture)
    {
        // Initialize biases
        for (int i = 1; i < architecture.size(); i++) {
            biases[i-1] = Eigen::VectorXd::Zero(architecture[i]);
        }
        // Initialize weights
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 1.0);
        for (int i = 0; i < architecture.size() - 1; i++) {
            weights[i] = Eigen::MatrixXd::Random(architecture[i+1], architecture[i]);
            weights[i] = weights[i].array() * dist(gen);
        }
    }


    void save(const std::string& filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Error: Could not open file '" << filename << "' for writing\n";
            return;
        }
        // Save architecture
        const int num_layers = architecture.size();
        file.write(reinterpret_cast<const char*>(&num_layers), sizeof(num_layers));
        for (int i = 0; i < num_layers; i++) {
            const int layer_count = architecture[i];
            file.write(reinterpret_cast<const char*>(&layer_count), sizeof(layer_count));
        }
        // Save activation functions
        for (int i = 0; i < activation_functions.size(); i++) {
            const int af = activation_functions[i];
            file.write(reinterpret_cast<const char*>(&af), sizeof(af));
        }
        // Save biases
        for (int i = 0; i < biases.size(); i++) {
            const int num_neurons = biases[i].size();
            file.write(reinterpret_cast<const char*>(biases[i].data()), num_neurons * sizeof(double));
        }
        // Save weights
        for (int i = 0; i < weights.size(); i++) {
            const int num_rows = weights[i].rows();
            const int num_cols = weights[i].cols();
            file.write(reinterpret_cast<const char*>(weights[i].data()), num_rows * num_cols * sizeof(double));
        }
    }

    void load(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Error: Could not open file '" << filename << "' for reading\n";
            return;
        }
        // Load architecture
        int num_layers;
        file.read(reinterpret_cast<char*>(&num_layers), sizeof(num_layers));
        std::vector<int> architecture(num_layers);
        for (int i = 0; i < num_layers; i++) {
            int numLayers;
            file.read(reinterpret_cast<char*>(&numLayers), sizeof(numLayers));
            architecture[i] = numLayers;
        }
        // Load activation functions
        std::vector<int> activation_functions(num_layers-1);
        for (int i = 0; i < activation_functions.size(); i++) {
            int af;
            file.read(reinterpret_cast<char*>(&af), sizeof(af));
            activation_functions[i] = af;
        }
        // Reconstruct the MLP with the loaded architecture and activation functions
        *this = MultiLayerPerceptron(architecture, activation_functions);
        // Load biases
        for (int i = 0; i < biases.size(); i++) {
            int num_neurons = biases[i].size();
            file.read(reinterpret_cast<char*>(biases[i].data()), num_neurons * sizeof(double));
        }
        // Load weights
        for (int i = 0; i < weights.size(); i++) {
            int num_rows = weights[i].rows();
            int num_cols = weights[i].cols();
            file.read(reinterpret_cast<char*>(weights[i].data()), num_rows * num_cols * sizeof(double));
        }
    }

    Eigen::VectorXd forward(const Eigen::VectorXd& input) const {
        Eigen::VectorXd output = input;
        for (int i = 0; i < weights.size(); i++) {
            output = (weights[i] * output).array() + biases[i].array();
            if (activation_functions[i] == ReLU) { // ReLU activation
                output = output.cwiseMax(Eigen::VectorXd::Zero(output.rows()));
            } else if (activation_functions[i] == Sigmoid) { // Sigmoid activation
                output = 1.0 / (1.0 + (-output).array().exp());
            }
        }
        return output;
    }
    void mutate(double mutation_factor, int radiation) {
        // Adjust weights and biases
        for (int i = 0; i < weights.size(); i++) {
            weights[i] += mutation_factor * Eigen::MatrixXd::Random(weights[i].rows(), weights[i].cols());
            biases[i] += mutation_factor * Eigen::VectorXd::Random(biases[i].size());
        }

        // Adjust architecture (if radiation is non-zero)
        if (radiation > 0) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dist(0.0, 1.0);
//            std::bernoulli_distribution drop_neuron(0.1 * mutation_factor);
//            std::bernoulli_distribution add_neuron(0.1 * mutation_factor);
            std::bernoulli_distribution change_activation(0.1);
            for (int i = 0; i < architecture.size() - 1; i++) {
                // Drop neuron
                // Change activation function
                if (dist(gen) < change_activation(gen) ) {
                    const int current_af = activation_functions[i];
                    const int new_af = (current_af + 1) % num_activation_functions;
                    activation_functions[i] = new_af;
                }
            }
        }
    }

private:
    std::vector<Eigen::MatrixXd> weights;
    std::vector<Eigen::VectorXd> biases;
    std::vector<int> activation_functions;
    std::vector<int> architecture;
};



/**
* std::vector<int> architecture = {2, 4, 1};
std::vector<int> activations = {1, 2}; // 1 for ReLU, 2 for sigmoid
MultiLayerPerceptron mlp(architecture, activations);
*/