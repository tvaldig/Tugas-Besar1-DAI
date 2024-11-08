#include "sahl.hpp"

Result steepest_ascent_hill_climbing(std::vector<std::vector<std::vector<int>>> cube) {
    Result result;
    result.cube = cube;
    result.error = calculate_error(result.cube);
    result.steps = 0;

    bool improvement = true;
    auto start_time = std::chrono::high_resolution_clock::now();

    while (improvement) {
        improvement = false;
        auto best_cube = result.cube;
        double best_error = result.error;

        for (int i1 = 0; i1 < N * N * N - 1; ++i1) {
            int x1 = i1 / (N * N), y1 = (i1 / N) % N, z1 = i1 % N;
            for (int i2 = i1 + 1; i2 < N * N * N; ++i2) {
                int x2 = i2 / (N * N), y2 = (i2 / N) % N, z2 = i2 % N;

                auto neighbor = swap_elements(result.cube, x1, y1, z1, x2, y2, z2);
                double neighbor_error = calculate_error(neighbor);

                if (neighbor_error < best_error) {
                    std::cout<< "steepest ascent hill climbing" << std::endl;
                    std::cout << "Error: " << neighbor_error << std::endl;
                    std::cout << "Jumlah h: " << jumlahSkor(neighbor) << std::endl;
                    best_error = neighbor_error;
                    best_cube = neighbor;
                }
            }
        }

        if (best_error < result.error) {
            result.cube = best_cube;
            result.error = best_error;
            result.steps++;
            improvement = true;
        }

        if (result.error == 0) break;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
    return result;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));
    auto cube = initialize_random_cube();
    Result rsult = steepest_ascent_hill_climbing(cube);
    
    return 0;
}