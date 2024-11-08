#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>

// Konstanta
const int N = 5; // Ukuran kubus (5x5x5)
const int POPULATION_SIZE = 100;
const int GENERATIONS = 100000;
const double CROSSOVER_RATE = 0.8;
const double MUTATION_RATE = 0.05;

// Struktur Result
struct Result {
    double error;
    int steps;
    double time_taken;
    std::vector<std::vector<std::vector<int>>> cube;
    std::vector<std::vector<std::vector<int>>> initial_cube;
    std::vector<double> error_history;
};

// Kelas Individu
struct Individual {
    std::vector<int> chromosome;
    double fitness;

    Individual(const std::vector<int>& chromosome)
        : chromosome(chromosome), fitness(0.0) {}

    void calculate_fitness(int magic_constant);
};

// Fungsi untuk menghitung magic constant
int calculate_magic_constant(int N) {
    return (N * (N * N * N + 1)) / 2;
}

// Fungsi untuk menginisialisasi kubus acak
std::vector<std::vector<std::vector<int>>> initialize_random_cube() {
    std::vector<int> numbers(N * N * N);
    for (int i = 0; i < N * N * N; ++i) {
        numbers[i] = i + 1;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);

    std::vector<std::vector<std::vector<int>>> cube(
        N, std::vector<std::vector<int>>(N, std::vector<int>(N)));
    int index = 0;
    for (int layer = 0; layer < N; ++layer) {
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                cube[layer][row][col] = numbers[index++];
            }
        }
    }
    return cube;
}

// Fungsi untuk menghitung error
double calculate_error(const std::vector<std::vector<std::vector<int>>>& cube) {
    int magic_constant = calculate_magic_constant(N);
    double total_difference = 0.0;

    // Evaluasi baris, kolom, pilar
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int sum_row = 0;
            int sum_col = 0;
            int sum_pillar = 0;
            for (int k = 0; k < N; ++k) {
                sum_row += cube[i][j][k];
                sum_col += cube[i][k][j];
                sum_pillar += cube[k][i][j];
            }
            total_difference += std::abs(magic_constant - sum_row);
            total_difference += std::abs(magic_constant - sum_col);
            total_difference += std::abs(magic_constant - sum_pillar);
        }
    }

    // Evaluasi diagonal ruang utama
    int sum_diag1 = 0;
    int sum_diag2 = 0;
    int sum_diag3 = 0;
    int sum_diag4 = 0;
    for (int i = 0; i < N; ++i) {
        sum_diag1 += cube[i][i][i];
        sum_diag2 += cube[i][i][N - i - 1];
        sum_diag3 += cube[i][N - i - 1][i];
        sum_diag4 += cube[N - i - 1][i][i];
    }
    total_difference += std::abs(magic_constant - sum_diag1);
    total_difference += std::abs(magic_constant - sum_diag2);
    total_difference += std::abs(magic_constant - sum_diag3);
    total_difference += std::abs(magic_constant - sum_diag4);

    return total_difference;
}

// Implementasi fungsi calculate_fitness
void Individual::calculate_fitness(int magic_constant) {
    // Mapping kromosom ke kubus 3D
    std::vector<std::vector<std::vector<int>>> cube(
        N, std::vector<std::vector<int>>(N, std::vector<int>(N)));
    int index = 0;
    for (int layer = 0; layer < N; ++layer) {
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                cube[layer][row][col] = chromosome[index++];
            }
        }
    }

    double total_difference = calculate_error(cube);
    fitness = 1.0 / (1.0 + total_difference);
}

// Fungsi untuk menghasilkan populasi awal
std::vector<Individual> initialize_population() {
    std::vector<Individual> population;
    std::vector<int> base_chromosome(N * N * N);
    for (int i = 0; i < N * N * N; ++i) {
        base_chromosome[i] = i + 1;
    }

    std::random_device rd;
    std::mt19937 g(rd());

    for (int i = 0; i < POPULATION_SIZE; ++i) {
        std::vector<int> chromosome = base_chromosome;
        std::shuffle(chromosome.begin(), chromosome.end(), g);
        population.emplace_back(chromosome);
    }

    return population;
}

// Fungsi seleksi (Roulette Wheel Selection)
Individual selection(const std::vector<Individual>& population) {
    double fitness_sum = 0.0;
    for (size_t i = 0; i < population.size(); ++i) {
        fitness_sum += population[i].fitness;
    }

    std::uniform_real_distribution<double> distribution(0.0, fitness_sum);
    std::random_device rd;
    std::mt19937 gen(rd());
    double rand_value = distribution(gen);

    double cumulative_fitness = 0.0;
    for (size_t i = 0; i < population.size(); ++i) {
        cumulative_fitness += population[i].fitness;
        if (cumulative_fitness >= rand_value) {
            return population[i];
        }
    }

    return population.back();
}

// Fungsi crossover (Order Crossover)
std::pair<Individual, Individual> crossover(
    const Individual& parent1, const Individual& parent2) {
    int size = parent1.chromosome.size();
    std::vector<int> offspring1_chromosome(size, -1);
    std::vector<int> offspring2_chromosome(size, -1);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, size - 1);

    int crossover_point1 = dist(gen);
    int crossover_point2 = dist(gen);

    if (crossover_point1 > crossover_point2) {
        std::swap(crossover_point1, crossover_point2);
    }

    // Copy segmen dari parent1 ke offspring1
    for (int i = crossover_point1; i <= crossover_point2; ++i) {
        offspring1_chromosome[i] = parent1.chromosome[i];
    }

    // Copy gen-gen yang tersisa dari parent2 ke offspring1
    int current_pos = (crossover_point2 + 1) % size;
    int parent2_pos = (crossover_point2 + 1) % size;
    while (current_pos != crossover_point1) {
        while (std::find(offspring1_chromosome.begin(),
               offspring1_chromosome.end(),
               parent2.chromosome[parent2_pos]) != offspring1_chromosome.end()) {
            parent2_pos = (parent2_pos + 1) % size;
        }
        offspring1_chromosome[current_pos] = parent2.chromosome[parent2_pos];
        current_pos = (current_pos + 1) % size;
        parent2_pos = (parent2_pos + 1) % size;
    }

    // Proses yang sama untuk offspring2
    for (int i = crossover_point1; i <= crossover_point2; ++i) {
        offspring2_chromosome[i] = parent2.chromosome[i];
    }

    current_pos = (crossover_point2 + 1) % size;
    int parent1_pos = (crossover_point2 + 1) % size;
    while (current_pos != crossover_point1) {
        while (std::find(offspring2_chromosome.begin(),
               offspring2_chromosome.end(),
               parent1.chromosome[parent1_pos]) != offspring2_chromosome.end()) {
            parent1_pos = (parent1_pos + 1) % size;
        }
        offspring2_chromosome[current_pos] = parent1.chromosome[parent1_pos];
        current_pos = (current_pos + 1) % size;
        parent1_pos = (parent1_pos + 1) % size;
    }

    Individual offspring1(offspring1_chromosome);
    Individual offspring2(offspring2_chromosome);

    return std::make_pair(offspring1, offspring2);
}

// Fungsi mutasi (Swap Mutation)
void mutation(Individual& individual) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    std::uniform_int_distribution<> index_dist(0, individual.chromosome.size() - 1);

    if (dist(gen) < MUTATION_RATE) {
        int index1 = index_dist(gen);
        int index2 = index_dist(gen);
        std::swap(individual.chromosome[index1], individual.chromosome[index2]);
    }
}

// Fungsi utama algoritma genetik
Result genetic_algorithm() {
    Result result;
    int magic_constant = calculate_magic_constant(N);
    std::vector<Individual> population = initialize_population();

    // Simpan kubus awal
    result.initial_cube = initialize_random_cube();
    result.cube = result.initial_cube;
    result.error = calculate_error(result.cube);
    result.steps = 0;

    // Hitung fitness awal
    for (size_t i = 0; i < population.size(); ++i) {
        population[i].calculate_fitness(magic_constant);
    }

    Individual best_individual = population[0];
    auto start_time = std::chrono::high_resolution_clock::now();

    // **Memulai iterasi generasi**
    for (int gen = 0; gen < GENERATIONS; ++gen) {
        std::vector<Individual> new_population;

        // Elitisme
        new_population.push_back(best_individual);

        while (new_population.size() < POPULATION_SIZE) {
            // Seleksi
            Individual parent1 = selection(population);
            Individual parent2 = selection(population);

            std::random_device rd;
            std::mt19937 gen_rd(rd());
            std::uniform_real_distribution<> dist(0.0, 1.0);

            // Crossover
            std::pair<Individual, Individual> offspring_pair = {parent1, parent2};
            if (dist(gen_rd) < CROSSOVER_RATE) {
                offspring_pair = crossover(parent1, parent2);
            }

            // Mutasi
            mutation(offspring_pair.first);
            mutation(offspring_pair.second);

            // Hitung fitness
            offspring_pair.first.calculate_fitness(magic_constant);
            offspring_pair.second.calculate_fitness(magic_constant);

            // Tambahkan ke populasi baru
            new_population.push_back(offspring_pair.first);
            if (new_population.size() < POPULATION_SIZE) {
                new_population.push_back(offspring_pair.second);
            }
        }

        // Update populasi
        population = new_population;

        // Update individu terbaik
        for (size_t i = 0; i < population.size(); ++i) {
            if (population[i].fitness > best_individual.fitness) {
                best_individual = population[i];
            }
        }

        // Simpan error history
        double best_error = 1.0 / best_individual.fitness - 1.0;
        result.error_history.push_back(best_error);
        result.steps = gen + 1;

        // **Menampilkan output pada setiap iterasi**
        std::cout << "Generasi: " << gen + 1
                  << ", Error Terbaik: " << best_error << std::endl;

        // Kriteria berhenti
        if (best_individual.fitness >= 1.0) {
            std::cout << "Solusi optimal ditemukan pada generasi ke-"
                      << gen + 1 << std::endl;
            break;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    result.time_taken = std::chrono::duration<double>(
        end_time - start_time).count();

    // Update hasil akhir
    result.cube = std::vector<std::vector<std::vector<int>>>(
        N, std::vector<std::vector<int>>(N, std::vector<int>(N)));
    int index = 0;
    for (int layer = 0; layer < N; ++layer) {
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                result.cube[layer][row][col] = best_individual.chromosome[index++];
            }
        }
    }
    result.error = 1.0 / best_individual.fitness - 1.0;

    return result;
}

int main() {
    Result result = genetic_algorithm();

    // Tampilkan hasil akhir
    std::cout << "Waktu Eksekusi: " << result.time_taken << " detik" << std::endl;
    std::cout << "Jumlah Langkah: " << result.steps << std::endl;
    std::cout << "Error Akhir: " << result.error << std::endl;

    // Jika Anda ingin menampilkan kubus akhir, Anda dapat melakukannya di sini
    // Contoh:
    /*
    for (int layer = 0; layer < N; ++layer) {
        std::cout << "Lapisan " << layer + 1 << ":\n";
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                std::cout << result.cube[layer][row][col] << "\t";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    */

    return 0;
}
