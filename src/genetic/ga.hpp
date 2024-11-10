#ifndef GA_HPP
#define GA_HPP

#include <vector>
#include <chrono>

#include "../objfunc/obj.hpp"


struct Individual {
    std::vector<int> chromosome;
    double fitness;
    std::vector<std::vector<std::vector<int>>> cube;

    Individual(const std::vector<int>& chromosome, int N, int target_sum);
    void calculateFitness(int N, int target_sum);
};

int calculateMagicSum(int N);
int jumlahSkor(const std::vector<std::vector<std::vector<int>>>& cube);
double calculate_error(const std::vector<std::vector<std::vector<int>>>& cube);

std::vector<Individual> initializePopulation(int populationSize, int N, int target_sum);
Individual selectParent(const std::vector<Individual>& population);
std::pair<Individual, Individual> performCrossover(const Individual& parent1, const Individual& parent2);
void performMutation(Individual& individual, double mutationRate);

Result geneticAlgorithm(int N, int populationSize, int maxGenerations, double crossoverRate, double mutationRate);

#endif
