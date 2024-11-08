#include "sa.hpp"

double scheduling(double t0, double cooling_rate, int iteration) {
    return t0 * pow(cooling_rate, iteration);
}
Result simulated_annealing() {
    Result result;
    result.cube = initialize_random_cube();
    result.error = calculate_error(result.cube);
    result.steps = 0;


    double cooling_rate =  0.99999;
    double temp = 1000.0;
    int max_iterate = 10000000;


    std::vector<std::vector<std::vector<int>>> best_cube = result.cube;
    double best_error = result.error;


    for (int i = 1; i < max_iterate; ++i) {
        temp = scheduling(temp, cooling_rate, i);


        int x1 = rand() % N;
        int y1 = rand() % N;
        int z1 = rand() % N;
        int x2 = rand() % N;
        int y2 = rand() % N;
        int z2 = rand() % N;
        while (x1==x2 && y1==y2 && z1==z2) 
        {
            x2 = rand() % N;
            y2 = rand() % N;
            z2 = rand() % N;
        }
        auto neighbor = swap_elements(result.cube, x1, y1, z1, x2, y2, z2);
        double neighbor_error = calculate_error(neighbor);
        double selisih = neighbor_error - result.error;


        if (selisih < 0) {
            result.cube = neighbor;
            result.error = neighbor_error;
            result.steps += 1;
        } else {
            if (static_cast<double>(rand()) / RAND_MAX < std::exp(-selisih / temp)) {
                result.cube = neighbor;
                result.error = neighbor_error;
                result.steps += 1;
            }
        }

        std::cout << "Jumlah h: " << jumlahSkor(result.cube) << std::endl;
        std::cout << "Jumlah Error: " << calculate_error(result.cube) << std::endl;


        if (result.error == 0) {
            break;
        }
    }


    return result;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));
    auto cube = initialize_random_cube();
    Result rsult = simulated_annealing();
    
    return 0;
}