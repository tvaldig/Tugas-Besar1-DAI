#include "ga.hpp"
#include <iostream>

int main() {
    int N = 5;
    int ukuranPopulasi = 100;
    int maxGenerasi = 10000;
    double tingkatCrossover = 0.8;
    double tingkatMutasi = 0.05;

    Hasil hasil = algoritmaGenetik(N, ukuranPopulasi, maxGenerasi, tingkatCrossover, tingkatMutasi);

    std::cout << "Waktu Eksekusi: " << hasil.waktuEksekusi << " detik" << std::endl;
    std::cout << "Error Akhir: " << hasil.errorAkhir << std::endl;

    return 0;
}
