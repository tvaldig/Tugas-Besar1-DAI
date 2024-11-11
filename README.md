# Tugas Besar 1 IF3070 Dasar Inteligensi Artifisial : Pencarian Solusi Diagonal Magic Cube dengan Local Search
Tugas ini bertujuan untuk mengimplementasikan search algorithm, yaitu Steepest Ascent Hill Climbing, Hill Climbing with Sideways Move, Stochastic Hill Climbing, Random Restart Hill Climbing, Simulated Annealing, dan Genetic Algorithm dalam memecahkan permasalahan magic cube 5x5x5.

Anggota Kelompok :
1. Taufiq Ramadhan Ahmad (18222060)
2. Muhammad Kevinza Faiz (18222072)
3. Viktor Arsindiantoro S (18222083)
4. Timotius Vivaldi Gunawan (18222091)

---

## Table of Contents

- [About the Project](#about-the-project)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation and Run](#installation)
- [File Structure](#file-structure)

---

## About the Project

Tugas **Pencarian Solusi Diagonal Magic Cube dengan Local Search** bertujuan untuk menyelesaikan masalah magic cube 5x5x5 di mana jumlah bilangan pada setiap baris, kolom, tiang, diagonal ruang, dan diagonal pada suatu potongan bidang kubus bernilai sama yang disebut dengan magic number. Implementasi ini mencakup beberapa varian **hill climbing**, **simulated annealing**, dan **genetic algorithm** untuk menjelajah ruang solusi yang mungkin:
- **Steepest Ascent Hill Climbing**: Hanya memilih neighbor dengan nilai error lebih kecil.
- **Stochastic Hill Climbing**: Memilih neighbor secara acak dan menerimanya jika error neighbor lebih kecil.
- **Hill Climbing with Sideways Moves**: Memungkinkan dilakukan pemindahan ke neighbor dengan eror yang sama dengan kondisi current.
- **Simulated Annealing**: Menggabungkan eksplorasi ruang pencarian solusi dengan mekanisme probabilistik yang memungkinkan penerimaan solusi yang lebih buruk pada tahap awal pencarian untuk menghindari keterjebakan pada local minimal.
- **Genetic Algorithm**: Menerapkan konsep seleksi alam dan evolusi genetik, yaitu selection, crossover, dan mutation.

Contoh Magic Cube 5x5x5
<p align="center">
  <img width="600" height="400" src="example.jpg" alt="simple example" />
</p>


Repository ini terbagi menjadi 2 branch : 
1. Master branch berisi program dengan visualisasi objective function terhadap tiap iterasi
2. Branch without-plot berisi program tanpa visualisasi objective function

---

## Getting Started

### Prerequisites

- Standard **g++ compiler library**
- **Python** versi 3.12 dengan library matplotlib **(Optional : hanya untuk master branch)**
  
### Installation and Run Program

1. **Clone Repository**
   ```bash
   git clone https://github.com/tvaldig/Tugas-Besar1-DAI.git
   
2. **Run the program**
   ```bash
   ./cube

---

## File Structure
    D:.
    ├───.vscode
    ├───build
    │   ├───.cmake
    │   │   └───api
    │   │       └───v1
    │   │           ├───query
    │   │           │   └───client-vscode
    │   │           └───reply
    │   └───CMakeFiles
    │       ├───3.20.21032501-MSVC_2
    │       │   ├───CompilerIdC
    │       │   └───CompilerIdCXX
    │       └───3.31.0-rc2
    │           ├───CompilerIdC
    │           └───CompilerIdCXX
    ├───include
    │   ├───glad
    │   ├───GLFW
    │   ├───glm
    │   │   ├───detail
    │   │   ├───ext
    │   │   ├───gtc
    │   │   ├───gtx
    │   │   └───simd
    │   └───KHR
    ├───lib
    └───src
        ├───genetic
        ├───objfunc
        ├───random_restart
        ├───sideway_move
        ├───simulated_annealing
        ├───steepest_ascent_hl
        └───stochastic_hc
