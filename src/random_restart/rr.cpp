#include "rr.hpp"

Result random_restart_hill_climbing(std::vector<std::vector<std::vector<int>>> cube, int max_restarts) {
    Result best_result;
    best_result.error = std::numeric_limits<double>::infinity();
    std::vector<Result> result_history;
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

        

        best_result.error_history.push_back(current_result.error);
        result_history.push_back(current_result);
        if (best_result.error == 0) {
            break;
        }
    }
    std::cout << "========================RESTART LOG=============================" << std::endl;
    for(int j = 0; j < max_restarts; j++ ){
        std::cout << "RESTART KE- " << j + 1 << std::endl;
        std::cout << "STEPS TAKEN   : " << result_history[j].steps << std::endl;
        std::cout << "CURRENT ERROR : " << result_history[j].error << std::endl;
        std::cout << "================================================================" << std::endl;
    }
   

    auto end_time = std::chrono::high_resolution_clock::now();
    best_result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
    return best_result;
}
