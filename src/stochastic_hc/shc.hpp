#ifndef SHC_HPP
#define SHC_HPP

#include "../objfunc/obj.hpp"

std::vector<std::vector<std::vector<int>>> random_neighbor(
    const std::vector<std::vector<std::vector<int>>>& cube);

Result stochastic_hill_climbing(std::vector<std::vector<std::vector<int>>> cube, int max_iterations);

#endif
