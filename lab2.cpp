#include "balance.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

// default number of samples per batch
static constexpr size_t N_SAMPLES = 1 << 11;
// default `n` of scrambled lists. actual size is 2n.
static constexpr size_t SCRAMBLE_N = 1 << 4;
// target epsilon (closeness to true probability) defined as (stddev / sqrt(n))
static constexpr double TARGET_EPS = 0.0001;

static constexpr std::string_view USAGE{
    "usage: ./lab2.out [n_samples=1024] [n=16] [epsilon=0.0001]"};

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
    double variance =
        std::accumulate(sqdiffs.begin(), sqdiffs.end(), 0.0) / sqdiffs.size();
    return std::sqrt(variance);
}

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
        if (n_iters % 100 == 0) {
            std::cout << '.';
            std::cout.flush();
        }
        if (n_iters % 1000 == 0) {
            std::cout << std::endl;
        }
    } while (props.size() < 10 ||
             stddev(props) / std::sqrt(props.size()) > target_eps);
    std::cout << std::endl;
    double proportion =
        std::accumulate(props.begin(), props.end(), 0.0) / props.size();
    std::cout << "ratio (balanced/total): " << proportion << std::endl;
    std::cout << "iterations: " << n_iters << std::endl;

    return 0;
}
