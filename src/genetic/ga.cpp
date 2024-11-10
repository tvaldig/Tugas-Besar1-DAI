#include "ga.hpp"
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>

Individual::Individual(const std::vector<int>& chromosome, int N, int target_sum) : chromosome(chromosome), fitness(0.0) {
    cube = std::vector<std::vector<std::vector<int>>>(N, std::vector<std::vector<int>>(N, std::vector<int>(N)));
    int index = 0;
    for (int layer = 0; layer < N; ++layer)
        for (int row = 0; row < N; ++row)
            for (int col = 0; col < N; ++col)
                cube[layer][row][col] = chromosome[index++];
    calculateFitness(N, target_sum);
}

void Individual::calculateFitness(int N, int target_sum) {
    int score = jumlahSkor(cube);
    fitness = score / static_cast<double>(N * N * 3 + N * 2 + 4);
}

int calculateMagicSum(int N) {
    return (N * (N * N * N + 1)) / 2;
}

int jumlahSkor(const std::vector<std::vector<std::vector<int>>>& cube) {
    int sum = 0;
    int target_sum = calculateMagicSum(cube.size());
    int N = cube.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int row_sum = 0, col_sum = 0, pillar_sum = 0;
            for (int k = 0; k < N; ++k) {
                row_sum += cube[i][j][k];
                col_sum += cube[i][k][j];
                pillar_sum += cube[k][i][j];
            }
            if (row_sum == target_sum) sum++;
            if (col_sum == target_sum) sum++;
            if (pillar_sum == target_sum) sum++;
        }
    }

    for (int k = 0; k < N; ++k) {
        int diag1 = 0, diag2 = 0;
        for (int i = 0; i < N; ++i) {
            diag1 += cube[i][i][k];
            diag2 += cube[i][N - 1 - i][k];
        }
        if (diag1 == target_sum) sum++;
        if (diag2 == target_sum) sum++;
    }

    for (int j = 0; j < N; ++j) {
        int diag1 = 0, diag2 = 0;
        for (int i = 0; i < N; ++i) {
            diag1 += cube[i][j][i];
            diag2 += cube[i][j][N - 1 - i];
        }
        if (diag1 == target_sum) sum++;
        if (diag2 == target_sum) sum++;
    }

    for (int i = 0; i < N; ++i) {
        int diag1 = 0, diag2 = 0;
        for (int j = 0; j < N; ++j) {
            diag1 += cube[i][j][j];
            diag2 += cube[i][j][N - 1 - j];
        }
        if (diag1 == target_sum) sum++;
        if (diag2 == target_sum) sum++;
    }

    int diag1 = 0, diag2 = 0, diag3 = 0, diag4 = 0;
    for (int i = 0; i < N; ++i) {
        diag1 += cube[i][i][i];
        diag2 += cube[i][i][N - 1 - i];
        diag3 += cube[i][N - 1 - i][i];
        diag4 += cube[i][N - 1 - i][N - 1 - i];
    }
    if (diag1 == target_sum) sum++;
    if (diag2 == target_sum) sum++;
    if (diag3 == target_sum) sum++;
    if (diag4 == target_sum) sum++;

    return sum;
}

double calculate_error(const std::vector<std::vector<std::vector<int>>>& cube) {
    int N = cube.size();
    double M = (N * (pow(N, 3) + 1)) / 2;
    double error_total = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double row_sum = 0, col_sum = 0, pillar_sum = 0;
            for (int k = 0; k < N; ++k) {
                row_sum += cube[i][j][k];
                col_sum += cube[i][k][j];
                pillar_sum += cube[k][i][j];
            }
            error_total += std::abs(M - row_sum) + std::abs(M - col_sum) + std::abs(M - pillar_sum);
        }
    }
    return error_total;
}

std::vector<Individual> initializePopulation(int populationSize, int N, int target_sum) {
    std::vector<Individual> population;
    std::vector<int> baseChromosome(N * N * N);
    for (int i = 0; i < N * N * N; ++i)
        baseChromosome[i] = i + 1;

    std::random_device rd;
    std::mt19937 g(rd());
    for (int i = 0; i < populationSize; ++i) {
        std::vector<int> chromosome = baseChromosome;
        std::shuffle(chromosome.begin(), chromosome.end(), g);
        population.emplace_back(chromosome, N, target_sum);
    }
    return population;
}

Individual selectParent(const std::vector<Individual>& population) {
    double totalFitness = 0.0;
    for (const auto& individual : population)
        totalFitness += individual.fitness;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(0.0, totalFitness);
    double randomValue = distribution(gen);
    double cumulativeFitness = 0.0;

    for (const auto& individual : population) {
        cumulativeFitness += individual.fitness;
        if (cumulativeFitness >= randomValue)
            return individual;
    }
    return population.back();
}

std::pair<Individual, Individual> performCrossover(const Individual& parent1, const Individual& parent2) {
    int size = parent1.chromosome.size();
    std::vector<int> childChromosome1(size), childChromosome2(size);
    std::vector<int> indices(size, -1);
    std::vector<bool> visited(size, false);
    int cycle = 1;

    for (int i = 0; i < size; ++i) {
        if (!visited[i]) {
            int currentIndex = i;
            do {
                indices[currentIndex] = cycle;
                visited[currentIndex] = true;
                int value = parent2.chromosome[currentIndex];
                currentIndex = std::distance(parent1.chromosome.begin(), std::find(parent1.chromosome.begin(), parent1.chromosome.end(), value));
            } while (currentIndex != i);
            cycle++;
        }
    }

    for (int i = 0; i < size; ++i) {
        if (indices[i] % 2 == 1) {
            childChromosome1[i] = parent1.chromosome[i];
            childChromosome2[i] = parent2.chromosome[i];
        } else {
            childChromosome1[i] = parent2.chromosome[i];
            childChromosome2[i] = parent1.chromosome[i];
        }
    }
    return {Individual(childChromosome1, parent1.cube.size(), calculateMagicSum(parent1.cube.size())), 
            Individual(childChromosome2, parent1.cube.size(), calculateMagicSum(parent1.cube.size()))};
}

void performMutation(Individual& individual, double mutationRate) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribution(0.0, 1.0);

    if (distribution(gen) < mutationRate) {
        std::uniform_int_distribution<> indexDist(0, individual.chromosome.size() - 1);
        int index1 = indexDist(gen), index2 = indexDist(gen);
        std::swap(individual.chromosome[index1], individual.chromosome[index2]);
    }
}

Result geneticAlgorithm(int N, int populationSize, int maxGenerations, double crossoverRate, double mutationRate) {
    Result result;
    int target_sum = calculateMagicSum(N);
    auto population = initializePopulation(populationSize, N, target_sum);

    Individual bestIndividual = population[0];
    auto startTime = std::chrono::high_resolution_clock::now();

    for (int generation = 0; generation < maxGenerations; ++generation) {
        std::vector<Individual> newPopulation;
        newPopulation.push_back(bestIndividual);

        while (newPopulation.size() < populationSize) {
            auto parent1 = selectParent(population);
            auto parent2 = selectParent(population);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distribution(0.0, 1.0);

            auto children = distribution(gen) < crossoverRate ? performCrossover(parent1, parent2) : std::make_pair(parent1, parent2);
            performMutation(children.first, mutationRate);
            performMutation(children.second, mutationRate);
            children.first.calculateFitness(N, target_sum);
            children.second.calculateFitness(N, target_sum);
            newPopulation.push_back(children.first);
            if (newPopulation.size() < populationSize)
                newPopulation.push_back(children.second);
        }
        population = newPopulation;

        for (const auto& individual : population) {
            if (individual.fitness > bestIndividual.fitness)
                bestIndividual = individual;
        }
        result.error = calculate_error(bestIndividual.cube);
        result.error_history.push_back(result.error);
        result.steps = generation + 1;

        int currentScore = jumlahSkor(bestIndividual.cube);
        std::cout << "Generation: " << generation + 1 
                  << ", Score: " << currentScore 
                  << ", Error: " << result.error 
                  << std::endl;

        if (currentScore == N * N * 3 + N * 2 + 4)
            break;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.time_taken = std::chrono::duration<double>(endTime - startTime).count();
    result.cube = bestIndividual.cube;

    return result;
}
