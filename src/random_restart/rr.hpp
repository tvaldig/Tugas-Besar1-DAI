#ifndef RR_HPP
#define RR_HPP

#include "../objfunc/obj.hpp"
#include "../steepest_ascent_hl/sahl.hpp"

Result random_restart_hill_climbing(std::vector<std::vector<std::vector<int>>> cube, int max_restarts);

#endif
