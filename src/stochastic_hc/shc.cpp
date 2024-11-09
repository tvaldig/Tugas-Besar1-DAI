#include "shc.hpp"
#include <random>
#include <chrono>
#include <iostream>

std::vector<std::vector<std::vector<int>>> random_neighbor(
    const std::vector<std::vector<std::vector<int>>>& cube) {
    int x1, y1, z1, x2, y2, z2;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N - 1);

    x1 = dis(gen); y1 = dis(gen); z1 = dis(gen);
    x2 = dis(gen); y2 = dis(gen); z2 = dis(gen);

    while (x1 == x2 && y1 == y2 && z1 == z2) {
        x2 = dis(gen); y2 = dis(gen); z2 = dis(gen);
    }

    return swap_elements(cube, x1, y1, z1, x2, y2, z2);
}

Result stochastic_hill_climbing(std::vector<std::vector<std::vector<int>>> cube, int max_iterations) {
    Result result;
    result.cube = cube;
    result.error = calculate_error(result.cube);
    result.steps = 0;

    std::vector<double> error_history;
    error_history.push_back(result.error);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int iter = 0; iter < max_iterations; ++iter) {
        auto neighbor = random_neighbor(result.cube);
        double neighbor_error = calculate_error(neighbor);

        if (neighbor_error < result.error) {
            result.cube = neighbor;
            result.error = neighbor_error;
            result.steps++;
            error_history.push_back(result.error);
            std::cout << "Stochastic Hill Climbing" << std::endl;
            std::cout << "Jumlah h: " << jumlahSkor(result.cube) << std::endl;
            std::cout << "Jumlah Error: " << result.error << std::endl;

            if (result.error == 0) {
                break;
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
    result.error_history = error_history;
    
    return result;
}

// int main() {
//     std::srand(static_cast<unsigned int>(std::time(0)));
//     auto cube = initialize_random_cube();
//     Result rsult = stochastic_hill_climbing(cube, 100000);
    
//     return 0;
// }
