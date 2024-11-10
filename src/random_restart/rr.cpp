#include "rr.hpp"

Result random_restart_hill_climbing(std::vector<std::vector<std::vector<int>>> cube, int max_restarts) {
    Result best_result;
    best_result.error = std::numeric_limits<double>::infinity();
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < max_restarts; i++) {
        Result current_result;

        if (i == 0) {
            current_result = steepest_ascent_hill_climbing(cube);
        } else {
            std::vector<std::vector<std::vector<int>>> new_state = initialize_random_cube();
            current_result = steepest_ascent_hill_climbing(new_state);
        }
        if (current_result.error < best_result.error) {
            best_result = current_result;
        }

        std::cout << "RESTART KE- " << i + 1 << std::endl;
        std::cout << "STEPS TAKEN   : " << current_result.steps << std::endl;
        std::cout << "CURRENT ERROR : " << current_result.error << std::endl;

        best_result.error_history.push_back(current_result.error);

        if (best_result.error == 0) {
            break;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    best_result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
    return best_result;
}
