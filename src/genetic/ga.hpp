#ifndef GA_HPP
#define GA_HPP

#include <vector>
#include <chrono>
#include "../objfunc/obj.hpp"

struct Individu {
    std::vector<int> kromosom;
    double fitness;
    Individu(const std::vector<int>& kromosom);
    void hitungFitness(int N, int magicSum);
};


int hitungMagicSum(int N);
std::vector<Individu> inisialisasiPopulasi(int ukuranPopulasi, int N);
Individu seleksi(const std::vector<Individu>& populasi);
std::pair<Individu, Individu> crossover(const Individu& parent1, const Individu& parent2);
void mutasi(Individu& individu, double tingkatMutasi);
Result algoritmaGenetik(int N, int ukuranPopulasi, int maxGenerasi, double tingkatCrossover, double tingkatMutasi);

#endif
