#include "obj.hpp"
#include <iostream>
#include <numeric>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

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
