#ifndef OBJ_HPP
#define OBJ_HPP

#include <vector>
#include <cmath>
#include <chrono>
#include <iostream>

const int N = 5;
const int VAL = 125;
const int target_sum = 315;

struct Result {
    double error;
    int steps;
    double time_taken;
    std::vector<std::vector<std::vector<int>>> cube;
    std::vector<std::vector<std::vector<int>>> initial_cube;
    std::vector<double> error_history;
};

std::vector<std::vector<std::vector<std::vector<int>>>> generate_neighbors(const std::vector<std::vector<std::vector<int>>>& cube);

std::vector<std::vector<std::vector<int>>> initialize_random_cube();

int jumlahSkor(const std::vector<std::vector<std::vector<int>>>& cube);

double calculate_error(const std::vector<std::vector<std::vector<int>>>& cube);

std::vector<std::vector<std::vector<int>>> swap_elements(
    const std::vector<std::vector<std::vector<int>>>& cube, int x1, int y1, int z1, int x2, int y2, int z2);



#endif
