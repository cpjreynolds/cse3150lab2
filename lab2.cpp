#include "balance.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <numeric>

/*
 * returns the standard deviation of the given list.
 *
 * used to determine convergence.
 */
static double stddev(const std::vector<double>& lst)
{
    double mean = std::accumulate(lst.begin(), lst.end(), 0.0) / lst.size();
    std::vector<double> sqdiffs{lst};
    std::for_each(sqdiffs.begin(), sqdiffs.end(),
                  [=](double& n) { n = std::pow(n - mean, 2); });
    double variance = std::accumulate(sqdiffs.begin(), sqdiffs.end(), 0.0) /
                      (sqdiffs.size() - 1);
    return std::sqrt(variance);
}

/*
 * runs the assignment algorithm to convergence.
 *
 * returns the proportion of well-balanced lists and the number of batches
 * required to converge.
 *
 * progress parameter enables printing a progress indicator to stdout so I could
 * see whether the program had hung or not.
 */
static std::tuple<double, size_t> do_thing(size_t n_samples, size_t scram_n,
                                           double target_eps,
                                           bool progress = false)
{
    std::vector<double> props;
    size_t n_iters{0};
    do {
        auto scrams = generate_scrambles(n_samples, scram_n);
        int nbal{0};
        for (const auto& s : scrams) {
            if (is_balanced(s)) {
                ++nbal;
            }
        }
        props.push_back(double(nbal) / double(n_samples));
        ++n_iters;
        if (progress) {
            if (n_iters % 100 == 0) {
                std::cout << '.';
                std::cout.flush();
            }
            if (n_iters % 1000 == 0) {
                std::cout << std::endl;
            }
        }
    } while (props.size() < 10 ||
             stddev(props) / std::sqrt(props.size()) > target_eps);
    if (progress) {
        std::cout << std::endl;
    }
    double proportion =
        std::accumulate(props.begin(), props.end(), 0.0) / props.size();
    return {proportion, n_iters};
}

#ifndef TESTING

// default number of samples per batch
static constexpr size_t N_SAMPLES = 1 << 11;
// default `n` of scrambled lists. actual size is 2n.
static constexpr size_t SCRAMBLE_N = 1 << 4;
// target epsilon (closeness to true probability) defined as (stddev / sqrt(n))
static constexpr double TARGET_EPS = 0.0001;

static constexpr std::string_view USAGE{
    "usage: ./lab2.out [n_samples=1024] [n=16] [epsilon=0.0001]"};

int main(int argc, char** argv)
{
    size_t n_samples = N_SAMPLES;
    size_t scram_n = SCRAMBLE_N;
    double target_eps = TARGET_EPS;

    try {
        if (argc > 1) {
            n_samples = std::stoul(argv[1]);
        }
        if (argc > 2) {
            scram_n = std::stoul(argv[2]);
        }
        if (argc > 3) {
            target_eps = std::stod(argv[3]);
        }
        if (argc > 4) {
            throw std::runtime_error("bad args");
        }
    }
    catch (const std::exception& e) {
        std::cerr << USAGE << std::endl;
        return 1;
    }

    auto [proportion, n_iters] = do_thing(n_samples, scram_n, target_eps, true);

    std::cout << "ratio (balanced/total): " << proportion << std::endl;
    std::cout << "iterations: " << n_iters << std::endl;
    std::cout << "total samples: " << n_iters * n_samples << std::endl;

    return 0;
}

#else

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("stddev")
{
    std::vector<double> data1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> data2{data1};
    std::reverse(data2.begin(), data2.end());
    std::vector<double> data3(100);
    std::iota(data3.begin(), data3.end(), 1);

    CHECK_EQ(stddev(data1), doctest::Approx(2.7386));
    CHECK_EQ(stddev(data2), doctest::Approx(2.7386));
    CHECK_EQ(stddev(data3), doctest::Approx(29.0115));
}

TEST_CASE("do_thing")
{
    auto [prop1, niter1] = do_thing(1 << 11, 1 << 4, 0.001);
    auto [prop2, niter2] = do_thing(1 << 11, 1 << 3, 0.001);
    auto [prop3, niter3] = do_thing(1 << 11, 1 << 2, 0.001);
    auto [prop4, niter4] = do_thing(1 << 11, 1 << 1, 0.001);

    CHECK_EQ(prop1, doctest::Approx(0.117606));
    CHECK_EQ(prop2, doctest::Approx(0.221376));
    CHECK_EQ(prop3, doctest::Approx(0.399425));
    CHECK_EQ(prop4, doctest::Approx(0.666472));
    CHECK_EQ(niter1, 49);
    CHECK_EQ(niter2, 103);
    CHECK_EQ(niter3, 92);
    CHECK_EQ(niter4, 106);
}

#endif
