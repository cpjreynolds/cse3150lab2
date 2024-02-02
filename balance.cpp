#include <random>
#include "balance.hpp"
#include "prefix.hpp"

// performs the in-place Fisher-Yates scramble on the given list.
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
