#include "balance.hpp"
#include <iostream>
#include <vector>
#include <random>

int main(int argc, char** argv)
{
    auto scrams = generate_scrambles(10, 6);
    for (auto& s : scrams) {
        std::cout << is_balanced(s) << std::endl;
    }
}
