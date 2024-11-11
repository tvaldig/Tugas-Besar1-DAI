#ifndef OBJ_HPP
#define OBJ_HPP
#define MAX_CONSTRAINTS 13 

#include <vector>
#include <cmath>
#include <chrono>
#include <iostream>
#include <Windows.h>
#include "matplotlibcpp.h"


const int N = 5;
const int VAL = 125;
const int target_sum = 315;

struct Result {
    double error;
    int steps;
    double time_taken;
    std::vector<std::vector<std::vector<int>>> cube;
    std::vector<double> error_history;
    std::vector<double> iterasi;  
    std::vector<double> objfunc; 
    std::vector<double> probability;
    std::vector<double> probabilityindex;
    int frekuensi_stuck;
};

std::vector<std::vector<std::vector<std::vector<int>>>> generate_neighbors(const std::vector<std::vector<std::vector<int>>>& cube);

std::vector<std::vector<std::vector<int>>> initialize_random_cube();

int jumlahSkor(const std::vector<std::vector<std::vector<int>>>& cube);

double calculate_error(const std::vector<std::vector<std::vector<int>>>& cube);

std::vector<std::vector<std::vector<int>>> swap_elements(
    const std::vector<std::vector<std::vector<int>>>& cube, int x1, int y1, int z1, int x2, int y2, int z2);

bool isMagicRow(const std::vector<std::vector<std::vector<int>>> &cube, int level, int row);
bool isMagicColumn(const std::vector<std::vector<std::vector<int>>> &cube, int level, int column);
bool isMagicDiagonal(const std::vector<std::vector<std::vector<int>>> &cube, int level);
bool isMagicPillar(const std::vector<std::vector<std::vector<int>>> &cube, int row, int column);
bool isMagic3DDiagonal(const std::vector<std::vector<std::vector<int>>> &cube);
int countConstraints(const std::vector<std::vector<std::vector<int>>> &cube, int x, int y, int z);

void displayGraph(Result result, std::string namaalgoritma, bool isSimulated);
void destroyGraph();
#endif
