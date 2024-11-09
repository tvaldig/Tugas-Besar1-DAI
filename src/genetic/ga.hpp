#ifndef GA_HPP
#define GA_HPP

#include <vector>
#include <chrono>

struct Individu {
    std::vector<int> kromosom;
    double fitness;
    Individu(const std::vector<int>& kromosom);
    void hitungFitness(int N, int magicSum);
};

struct Hasil {
    double errorAkhir;
    int langkah;
    double waktuEksekusi;
    std::vector<std::vector<std::vector<int>>> kubus;
    std::vector<std::vector<std::vector<int>>> kubusAwal;
    std::vector<double> riwayatError;
};

int hitungMagicSum(int N);
std::vector<Individu> inisialisasiPopulasi(int ukuranPopulasi, int N);
Individu seleksi(const std::vector<Individu>& populasi);
std::pair<Individu, Individu> crossover(const Individu& parent1, const Individu& parent2);
void mutasi(Individu& individu, double tingkatMutasi);
Hasil algoritmaGenetik(int N, int ukuranPopulasi, int maxGenerasi, double tingkatCrossover, double tingkatMutasi);

#endif
