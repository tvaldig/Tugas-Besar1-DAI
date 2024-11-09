#include "ga.hpp"
#include <iostream>

int main() {
    int N = 5; 
    int ukuranPopulasi = 100;
    int maxGenerasi = 10000; 
    double tingkatCrossover = 0.8;
    double tingkatMutasi = 0.05;   

    bool solusiDitemukan = false;
    int iterasi = 0;

    while (!solusiDitemukan) {
        std::cout << "Iterasi: " << iterasi + 1 << std::endl;

        Hasil hasil = algoritmaGenetik(N, ukuranPopulasi, maxGenerasi, tingkatCrossover, tingkatMutasi);

        if (hasil.errorAkhir == 0.0) {
            solusiDitemukan = true;
            std::cout << "Magic Cube berhasil ditemukan pada iterasi ke-" << iterasi + 1 << "!" << std::endl;
            std::cout << "Waktu Eksekusi: " << hasil.waktuEksekusi << " detik" << std::endl;
            std::cout << "Langkah yang dibutuhkan: " << hasil.langkah << std::endl;
            break;
        } else {
            std::cout << "Magic Cube belum ditemukan. Error akhir pada iterasi ini: " << hasil.errorAkhir << std::endl;
            ++iterasi;
        }
    }

    return 0;
}
