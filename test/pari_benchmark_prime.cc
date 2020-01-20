//g++ -O2 pari_benchmark_prime.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lpari -o pari_benchmark_prime.o

#include <pari/pari.h>
#include <benchmark/benchmark.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string p;
string numberA;
string numberB;
string benchmarkExponent;
string reducedBenchmarkExponent;

static void BM_PrimeField_Addition(benchmark::State& state, GEN firstNumer, GEN secondNumber, GEN mod)
{
    for (auto _ : state) {
        Fp_add(firstNumer, secondNumber, mod);
    }
}

static void BM_PrimeField_Multiplication(benchmark::State& state, GEN firstNumer, GEN secondNumber, GEN mod)
{
    for (auto _ : state) {
        Fp_mul(firstNumer, secondNumber, mod);
    }
}

static void BM_PrimeField_Exponentiation(benchmark::State& state, GEN base, GEN power, GEN mod)
{
    for (auto _ : state) {
        Fp_pow(base, power, mod);
    }
}

static void BM_PrimeField_MultiplicativeInversion(benchmark::State& state, GEN base, GEN mod)
{
    for (auto _ : state) {
        Fp_inv(base, mod);
    }
}

BENCHMARK_CAPTURE(
    BM_PrimeField_Addition,
    basic_addition_pari,
    Fp_to_mod(strtoi(numberA.c_str()), strtoi(p.c_str())),
    Fp_to_mod(strtoi(numberB.c_str()), strtoi(p.c_str())),
    strtoi(p.c_str())
);

BENCHMARK_CAPTURE(
    BM_PrimeField_Multiplication,
    basic_multiplication_pari,
    Fp_to_mod(strtoi(numberA.c_str()), strtoi(p.c_str())),
    Fp_to_mod(strtoi(numberB.c_str()), strtoi(p.c_str())),
    strtoi(p.c_str())
);

BENCHMARK_CAPTURE(
    BM_PrimeField_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent_pari,
    Fp_to_mod(strtoi(numberA.c_str()), strtoi(p.c_str())),
    strtoi(benchmarkExponent.c_str()),
    strtoi(p.c_str())
);

BENCHMARK_CAPTURE(
    BM_PrimeField_Exponentiation,
    modular_exponentiation_with_reduced_exponent_pari,
    Fp_to_mod(strtoi(numberA.c_str()), strtoi(p.c_str())),
    strtoi(reducedBenchmarkExponent.c_str()),
    strtoi(p.c_str())
);

BENCHMARK_CAPTURE(
    BM_PrimeField_MultiplicativeInversion,
    basic_inverse_pari,
    strtoi(numberA.c_str()),
    strtoi(p.c_str())
);

int main(int argc, char** argv)
{
    ifstream benchmarkDataFile;
    benchmarkDataFile.open("prime_benchmark_data.txt");

    getline(benchmarkDataFile, p);

    getline(benchmarkDataFile, numberA);

    getline(benchmarkDataFile, numberB);

    getline(benchmarkDataFile, benchmarkExponent);

    getline(benchmarkDataFile, reducedBenchmarkExponent);

    benchmarkDataFile.close();

    pari_init(2000000000,0);
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}