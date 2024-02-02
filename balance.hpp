#ifndef BALANCE_HPP
#define BALANCE_HPP

#include <vector>

// generates `nscram` size 2n Fisher-Yates scrambled lists of {1,-1}s.
std::vector<std::vector<int>> generate_scrambles(size_t nscram, size_t n);

// returns true if the given list has a non-negative OR non-positive prefix sum
bool is_balanced(const std::vector<int>& lst);

#endif
