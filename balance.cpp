#include <random>
#include "balance.hpp"
#include "prefix.hpp"
#include <iostream>

// performs the in-place Fisher-Yates scramble on the given list.
//
// uses c++ random facilities as `std::rand() % range` introduces statistical
// bias whereas `uniform_int_distribution` does not.
static void do_scramble(std::vector<int>& lst)
{
    // static so the RNG is instantiated only once upon first invocation.
#ifdef TESTING
    // use the default seed to get reproducible tests
    static std::mt19937 rd{};
#else
    // otherwise get actual entropy from the hardware
    static std::mt19937 rd{std::random_device{}()};
#endif

    for (size_t i = lst.size() - 1; i > 0; --i) {
        auto n = std::uniform_int_distribution<int>(0, i)(rd);
        std::swap(lst[i], lst[n]);
    }
}

// generates a size 2n list of n 1s followed by n -1s.
static std::vector<int> generate_unscrambled(size_t n)
{
    std::vector<int> ret;
    ret.reserve(2 * n);

    ret.insert(ret.begin(), n, 1);
    ret.insert(ret.end(), n, -1);
    return ret;
}

std::vector<std::vector<int>> generate_scrambles(size_t nscram, size_t n)
{
    std::vector<int> temp = generate_unscrambled(n);
    std::vector<std::vector<int>> scrams(nscram, temp);

    for (auto& s : scrams) {
        do_scramble(s);
    }
    return scrams;
}

bool is_balanced(const std::vector<int>& lst)
{
    return non_neg_prefix_sum(lst) || non_pos_prefix_sum(lst);
}

#ifdef TESTING
#include "doctest.h"

// the RNG seed is always the same during testing so these are replicable.

TEST_CASE("do_scramble")
{
    std::vector<int> data1{1, 1, 1, -1, -1, -1};
    std::vector<int> result1{-1, 1, -1, 1, 1, -1};

    do_scramble(data1);
    CHECK_EQ(data1, result1);
}

TEST_CASE("generate_unscrambled")
{
    std::vector<int> result1{1, -1};
    std::vector<int> result2{1, 1, 1, 1, -1, -1, -1, -1};

    auto data1 = generate_unscrambled(1);
    auto data2 = generate_unscrambled(4);

    CHECK_EQ(data1, result1);
    CHECK_EQ(data2, result2);
}

TEST_CASE("generate_scrambles")
{
    std::vector<int> result1{-1, 1, 1, -1, -1, -1, 1, 1};
    std::vector<int> result2{1, 1, -1, -1, -1, 1, 1, -1};
    auto data = generate_scrambles(2, 4);
    CHECK_EQ(data.size(), 2);
    CHECK_EQ(data[0], result1);
    CHECK_EQ(data[1], result2);
}

TEST_CASE("is_balanced")
{
    // using the examples from the assignment pdf
    std::vector<int> data1{1, -1};
    std::vector<int> data2{1, -1, 1, -1};
    std::vector<int> data3{1, 1, 1, 1, -1, -1, -1, -1};
    std::vector<int> data4{1, 1, -1, 1, 1, 1, -1, -1, -1, -1};

    std::vector<int> bdata1{1, -1, -1, 1};
    std::vector<int> bdata2{1, 1, 1, -1, -1, -1, -1, -1, 1, 1};

    CHECK(is_balanced(data1));
    CHECK(is_balanced(data2));
    CHECK(is_balanced(data3));
    CHECK(is_balanced(data4));
    CHECK_FALSE(is_balanced(bdata1));
    CHECK_FALSE(is_balanced(bdata2));
}

#endif
