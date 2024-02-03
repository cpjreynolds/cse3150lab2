# CSE3150 - Lab 2

`make all` to compile
`make check` to compile and run tests.
`make run` to compile and run algorithm to convergence with default parameters.

## Usage

`> ./lab2.out 1024 16 0.0001`

runs with 1024 samples per batch, n=16, and stops execution when
`(stddev/sqrt(n_batches)) < 0.0001`. This is the default.

## Structure

`lab2.cpp` contains the `main` routine for running the algorithm to convergence
`balance.cpp/.hpp` contains the algorithm implementation and interface
`prefix.cpp/.hpp` contains the prefix sum implementation and interface
