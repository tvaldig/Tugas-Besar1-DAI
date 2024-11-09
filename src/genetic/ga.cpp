#include "ga.hpp"
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

Individu::Individu(const std::vector<int>& kromosom) : kromosom(kromosom), fitness(0.0) {}

void Individu::hitungFitness(int N, int magicSum) {
    std::vector<std::vector<std::vector<int>>> kubus(N, std::vector<std::vector<int>>(N, std::vector<int>(N)));
    int indeks = 0;
    for (int layer = 0; layer < N; ++layer)
        for (int row = 0; row < N; ++row)
            for (int col = 0; col < N; ++col)
                kubus[layer][row][col] = kromosom[indeks++];

    double totalDifference = 0.0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            int sumRow = 0, sumCol = 0, sumPillar = 0;
            for (int k = 0; k < N; ++k) {
                sumRow += kubus[i][j][k];
                sumCol += kubus[i][k][j];
                sumPillar += kubus[k][i][j];
            }
            totalDifference += std::abs(magicSum - sumRow);
            totalDifference += std::abs(magicSum - sumCol);
            totalDifference += std::abs(magicSum - sumPillar);
        }
    
    int sumDiag1 = 0, sumDiag2 = 0, sumDiag3 = 0, sumDiag4 = 0;
    for (int i = 0; i < N; ++i) {
        sumDiag1 += kubus[i][i][i];
        sumDiag2 += kubus[i][i][N - i - 1];
        sumDiag3 += kubus[i][N - i - 1][i];
        sumDiag4 += kubus[N - i - 1][i][i];
    }
    totalDifference += std::abs(magicSum - sumDiag1);
    totalDifference += std::abs(magicSum - sumDiag2);
    totalDifference += std::abs(magicSum - sumDiag3);
    totalDifference += std::abs(magicSum - sumDiag4);
    
    fitness = 1.0 / (1.0 + totalDifference);
}

int hitungMagicSum(int N) {
    return (N * (N * N * N + 1)) / 2;
}

std::vector<Individu> inisialisasiPopulasi(int ukuranPopulasi, int N) {
    std::vector<Individu> populasi;
    std::vector<int> kromosomDasar(N * N * N);
    for (int i = 0; i < N * N * N; ++i)
        kromosomDasar[i] = i + 1;

    std::random_device rd;
    std::mt19937 g(rd());
    for (int i = 0; i < ukuranPopulasi; ++i) {
        std::vector<int> kromosom = kromosomDasar;
        std::shuffle(kromosom.begin(), kromosom.end(), g);
        populasi.emplace_back(kromosom);
    }
    return populasi;
}

Individu seleksi(const std::vector<Individu>& populasi) {
    double totalFitness = 0.0;
    for (const auto& individu : populasi)
        totalFitness += individu.fitness;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribusi(0.0, totalFitness);
    double nilaiRandom = distribusi(gen);
    double kumulatifFitness = 0.0;

    for (const auto& individu : populasi) {
        kumulatifFitness += individu.fitness;
        if (kumulatifFitness >= nilaiRandom)
            return individu;
    }
    return populasi.back();
}

std::pair<Individu, Individu> crossover(const Individu& parent1, const Individu& parent2) {
    int ukuran = parent1.kromosom.size();
    std::vector<int> kromosomAnak1(ukuran), kromosomAnak2(ukuran);
    std::vector<int> indeks(ukuran, -1);
    std::vector<bool> visited(ukuran, false);
    int siklus = 1;

    for (int i = 0; i < ukuran; ++i) {
        if (!visited[i]) {
            int saatIni = i;
            do {
                indeks[saatIni] = siklus;
                visited[saatIni] = true;
                int value = parent2.kromosom[saatIni];
                saatIni = std::distance(parent1.kromosom.begin(), std::find(parent1.kromosom.begin(), parent1.kromosom.end(), value));
            } while (saatIni != i);
            siklus++;
        }
    }

    for (int i = 0; i < ukuran; ++i) {
        if (indeks[i] % 2 == 1) {
            kromosomAnak1[i] = parent1.kromosom[i];
            kromosomAnak2[i] = parent2.kromosom[i];
        } else {
            kromosomAnak1[i] = parent2.kromosom[i];
            kromosomAnak2[i] = parent1.kromosom[i];
        }
    }
    return {Individu(kromosomAnak1), Individu(kromosomAnak2)};
}

void mutasi(Individu& individu, double tingkatMutasi) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribusi(0.0, 1.0);
    std::uniform_int_distribution<> distribusiIndeks(0, individu.kromosom.size() - 1);

    if (distribusi(gen) < tingkatMutasi) {
        int indeks1 = distribusiIndeks(gen);
        int indeks2 = distribusiIndeks(gen);
        std::swap(individu.kromosom[indeks1], individu.kromosom[indeks2]);
    }
}

Result algoritmaGenetik(int N, int ukuranPopulasi, int maxGenerasi, double tingkatCrossover, double tingkatMutasi) {
    Result hasil;
    int magicSum = hitungMagicSum(N);
    auto populasi = inisialisasiPopulasi(ukuranPopulasi, N);

    for (auto& individu : populasi)
        individu.hitungFitness(N, magicSum);

    Individu terbaik = populasi[0];
    auto waktuMulai = std::chrono::high_resolution_clock::now();

    for (int gen = 0; gen < maxGenerasi; ++gen) {
        std::vector<Individu> populasiBaru;
        populasiBaru.push_back(terbaik);

        while (populasiBaru.size() < ukuranPopulasi) {
            auto parent1 = seleksi(populasi);
            auto parent2 = seleksi(populasi);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distribusi(0.0, 1.0);

            auto anak = distribusi(gen) < tingkatCrossover ? crossover(parent1, parent2) : std::make_pair(parent1, parent2);
            mutasi(anak.first, tingkatMutasi);
            mutasi(anak.second, tingkatMutasi);
            anak.first.hitungFitness(N, magicSum);
            anak.second.hitungFitness(N, magicSum);
            populasiBaru.push_back(anak.first);
            if (populasiBaru.size() < ukuranPopulasi)
                populasiBaru.push_back(anak.second);
        }
        populasi = populasiBaru;

        for (const auto& individu : populasi) {
            if (individu.fitness > terbaik.fitness)
                terbaik = individu;
        }
        hasil.error = 1.0 / terbaik.fitness - 1.0;
        hasil.error_history.push_back(hasil.error);
        hasil.steps = gen + 1;
        if (terbaik.fitness >= 1.0)
            break;
    }
    return hasil;
}
