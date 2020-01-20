//g++ -O2 miracl_benchmark_prime.cc /home/vagrant/miracl/zzn.o /home/vagrant/miracl/big.o /home/vagrant/miracl/miracl.a -I/home/vagrant/miracl -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -o miracl_benchmark_prime.o

extern "C"
{
    #include "miracl.h"
}
#include "zzn.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Miracl miraclPrecision=1000;
miracl *mip=&miraclPrecision;

string p;
string numberA;
string numberB;
string benchmarkExponent;
string reducedBenchmarkExponent;

//PRIME FIELD TESTS
static void BM_PrimeField_Addition(benchmark::State& state, ZZn firstNumber, ZZn secondNumber)
{
    for (auto _ : state) {
        firstNumber + secondNumber;
    }
}

static void BM_PrimeField_Multiplication(benchmark::State& state, ZZn firstNumber, ZZn secondNumber)
{
    for (auto _ : state) {
        firstNumber * secondNumber;
    }
}

static void BM_PrimeField_Exponentiation(benchmark::State& state, ZZn base, Big power)
{
    for (auto _ : state) {
        pow(base, power);
    }
}

static void BM_PrimeField_MultiplicativeInversion(benchmark::State& state, Big base, Big mod)
{
    for (auto _ : state) {
        inverse(base, mod);
    }
}

BENCHMARK_CAPTURE(
    BM_PrimeField_Addition,
    basic_addition_miracl,
    ZZn(Big((char*)numberA.c_str())),
    ZZn(Big((char*)numberB.c_str()))
);

BENCHMARK_CAPTURE(
    BM_PrimeField_Multiplication,
    basic_multiplication_miracl,
    ZZn(Big((char*)numberA.c_str())),
    ZZn(Big((char*)numberB.c_str()))
);

BENCHMARK_CAPTURE(
    BM_PrimeField_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent_miracl,
    ZZn(Big((char*)numberA.c_str())),
    Big((char*)benchmarkExponent.c_str())
);

BENCHMARK_CAPTURE(
    BM_PrimeField_Exponentiation,
    modular_exponentiation_with_reduced_exponent_miracl,
    ZZn(Big((char*)numberA.c_str())),
    Big((char*)reducedBenchmarkExponent.c_str())
);

BENCHMARK_CAPTURE(
    BM_PrimeField_MultiplicativeInversion,
    basic_inverse_miracl,
    Big((char*)numberA.c_str()),
    Big((char*)p.c_str())
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

  modulo(Big((char*)p.c_str()));
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
}