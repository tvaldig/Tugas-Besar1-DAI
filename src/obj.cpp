#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <random> 
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>

const int N = 5;
const int VAL = 125;
const int target_sum = 315;


double magicCubeLoss(const std::vector<std::vector<std::vector<int>>>& cube, int n) {
    double M = (n * (pow(n, 3) + 1)) / 2;

    double error_baris = 0, error_kolom = 0, error_tiang = 0;
    double error_diagonal_ruang = 0, error_diagonal_bidang = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            double row_sum = std::accumulate(cube[i][j].begin(), cube[i][j].end(), 0);
            error_baris += std::abs(M - row_sum);
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int k = 0; k < 5; k++) {
            double col_sum = 0;
            for (int j = 0; j < 5; j++) {
                col_sum += cube[i][j][k];
            }
            error_kolom += std::abs(M - col_sum);
        }
    }

    for (int j = 0; j < 5; j++) {
        for (int k = 0; k < 5; k++) {
            double pillar_sum = 0;
            for (int i = 0; i < 5; i++) {
                pillar_sum += cube[i][j][k];
            }
            error_tiang += std::abs(M - pillar_sum);
        }
    }

    error_diagonal_ruang += std::abs(M - (cube[0][0][0] + cube[1][1][1] + cube[2][2][2] + cube[3][3][3] + cube[4][4][4]));
    error_diagonal_ruang += std::abs(M - (cube[0][0][4] + cube[1][1][3] + cube[2][2][2] + cube[3][3][1] + cube[4][4][0]));
    error_diagonal_ruang += std::abs(M - (cube[0][4][0] + cube[1][3][1] + cube[2][2][2] + cube[3][1][3] + cube[4][0][4]));
    error_diagonal_ruang += std::abs(M - (cube[0][4][4] + cube[1][3][3] + cube[2][2][2] + cube[3][1][1] + cube[4][0][0]));

    for (int k = 0; k < 5; k++) {
        double diag1 = 0, diag2 = 0;
        for (int i = 0; i < 5; i++) {
            diag1 += cube[i][i][k];
            diag2 += cube[i][4 - i][k];
        }
        error_diagonal_bidang += std::abs(M - diag1);
        error_diagonal_bidang += std::abs(M - diag2);
    }

    for (int j = 0; j < 5; j++) {
        double diag1 = 0, diag2 = 0;
        for (int i = 0; i < 5; i++) {
            diag1 += cube[i][j][i];
            diag2 += cube[i][j][4 - i];
        }
        error_diagonal_bidang += std::abs(M - diag1);
        error_diagonal_bidang += std::abs(M - diag2);
    }

    for (int i = 0; i < 5; i++) {
        double diag1 = 0, diag2 = 0;
        for (int j = 0; j < 5; j++) {
            diag1 += cube[i][j][j];
            diag2 += cube[i][j][4 - j];
        }
        error_diagonal_bidang += std::abs(M - diag1);
        error_diagonal_bidang += std::abs(M - diag2);
    }

    double obj_func = error_baris + error_kolom + error_tiang + error_diagonal_ruang + error_diagonal_bidang;

    return obj_func;
}

#include <vector>
#include <algorithm>

std::vector<std::vector<std::vector<std::vector<int>>>> generate_neighbors(const std::vector<std::vector<std::vector<int>>>& cube) {
    std::vector<std::vector<std::vector<std::vector<int>>>> neighbors;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                for (int l = i; l < 5; l++) {
                    for (int m = (l == i ? j : 0); m < 5; m++) {
                        for (int n = (l == i && m == j ? k + 1 : 0); n < 5; n++) {
                            if (i == l && j == m && k == n) continue;
                            auto new_cube = cube;
                            std::swap(new_cube[i][j][k], new_cube[l][m][n]);
                            neighbors.push_back(new_cube);
                        }
                    }
                }
            }
        }
    }

    return neighbors;
}

std::vector<std::vector<std::vector<int>>> initialize_random_cube() {
    std::vector<int> numbers(N * N * N);
    std::iota(numbers.begin(), numbers.end(), 1);

    std::random_device rd; 
    std::default_random_engine rng(rd());

    std::shuffle(numbers.begin(), numbers.end(), rng);
    std::vector<std::vector<std::vector<int>>> cube(N, std::vector<std::vector<int>>(N, std::vector<int>(N)));
    int idx = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                cube[i][j][k] = numbers[idx++];

    return cube;
}

int jumlahSkor(const std::vector<std::vector<std::vector<int>>>& cube) {
    int sum = 0;

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
    double M = (N * (pow(N, 3) + 1)) / 2;
    double error_baris = 0, error_kolom = 0, error_tiang = 0;
    double error_diagonal_ruang = 0, error_diagonal_bidang = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double row_sum = 0, col_sum = 0, pillar_sum = 0;
            for (int k = 0; k < N; ++k) {
                row_sum += cube[i][j][k];
                col_sum += cube[i][k][j];
                pillar_sum += cube[k][i][j];
            }
            error_baris += std::abs(M - row_sum);
            error_kolom += std::abs(M - col_sum);
            error_tiang += std::abs(M - pillar_sum);
        }
    }

    for (int k = 0; k < N; ++k) {
        double diag1 = 0, diag2 = 0;
        for (int i = 0; i < N; ++i) {
            diag1 += cube[i][i][k];
            diag2 += cube[i][N - 1 - i][k];
        }
        error_diagonal_bidang += std::abs(M - diag1) + std::abs(M - diag2);
    }

    for (int j = 0; j < N; ++j) {
        double diag1 = 0, diag2 = 0;
        for (int i = 0; i < N; ++i) {
            diag1 += cube[i][j][i];
            diag2 += cube[i][j][N - 1 - i];
        }
        error_diagonal_bidang += std::abs(M - diag1) + std::abs(M - diag2);
    }

    for (int i = 0; i < N; ++i) {
        double diag1 = 0, diag2 = 0;
        for (int j = 0; j < N; ++j) {
            diag1 += cube[i][j][j];
            diag2 += cube[i][j][N - 1 - j];
        }
        error_diagonal_bidang += std::abs(M - diag1) + std::abs(M - diag2);
    }

    double diag1 = 0, diag2 = 0, diag3 = 0, diag4 = 0;
    for (int i = 0; i < N; ++i) {
        diag1 += cube[i][i][i];
        diag2 += cube[i][i][N - 1 - i];
        diag3 += cube[i][N - 1 - i][i];
        diag4 += cube[i][N - 1 - i][N - 1 - i];
    }
    error_diagonal_ruang = std::abs(M - diag1) + std::abs(M - diag2) + std::abs(M - diag3) + std::abs(M - diag4);

    return error_baris + error_kolom + error_tiang + error_diagonal_ruang + error_diagonal_bidang;
}

std::vector<std::vector<std::vector<int>>> swap_elements(
    const std::vector<std::vector<std::vector<int>>>& cube, int x1, int y1, int z1, int x2, int y2, int z2) {
    auto new_cube = cube;
    std::swap(new_cube[x1][y1][z1], new_cube[x2][y2][z2]);
    return new_cube;
}

struct Result {
    double error;
    int steps;
    double time_taken;
    std::vector<std::vector<std::vector<int>>> cube;
};

Result steepest_ascent_hill_climbing() {
    Result result;
    result.cube = initialize_random_cube();
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
    double loss = magicCubeLoss(cube, N);
    Result rsult = simulated_annealing();
    
    return 0;
}