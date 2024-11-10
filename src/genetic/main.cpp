#include "ga.hpp"
#include <iostream>

int main() {
    int N = 5;
    int populationSize = 100;
    int maxGenerations = 100000;
    double crossoverRate = 0.8;
    double mutationRate = 0.05;

    Result result = geneticAlgorithm(N, populationSize, maxGenerations, crossoverRate, mutationRate);

    std::cout << "Execution Time: " << result.time_taken << " seconds" << std::endl;
    std::cout << "Final Score: " << jumlahSkor(result.cube) << std::endl;
    std::cout << "Final Error: " << result.error << std::endl;

    return 0;
}
