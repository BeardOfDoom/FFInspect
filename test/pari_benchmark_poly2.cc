//g++ -O2 pari_benchmark_poly2.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lpari -o pari_benchmark_poly2.o

#include <pari/pari.h>
#include <benchmark/benchmark.h>
#include <iostream>

using namespace std;

static void BM_PolynomField2_Addition(benchmark::State& state, initializer_list<int> firstPolIndexOfOnes, long firstPolDegree, initializer_list<int> secondPolIndexOfOnes, long secondPolDegree)
{
    GEN firstPol = monomial_F2x(firstPolDegree, 1);
    for(initializer_list<int>::iterator i = firstPolIndexOfOnes.begin(); i < firstPolIndexOfOnes.end(); i++)
    {
        F2x_set(firstPol, *i);
    }

    GEN secondPol = monomial_F2x(secondPolDegree, 1);
    for(initializer_list<int>::iterator i = secondPolIndexOfOnes.begin(); i < secondPolIndexOfOnes.end(); i++)
    {
        F2x_set(secondPol, *i);
    }
    
    for (auto _ : state) {
        F2x_add(firstPol, secondPol);
    }
}


static void BM_PolynomField2_Multiplication(benchmark::State& state, initializer_list<int> firstPolIndexOfOnes, long firstPolDegree, initializer_list<int> secondPolIndexOfOnes, long secondPolDegree, initializer_list<int> modPolIndexOfOnes, long modPolDegree)
{
    GEN firstPol = monomial_F2x(firstPolDegree, 1);
    for(initializer_list<int>::iterator i = firstPolIndexOfOnes.begin(); i < firstPolIndexOfOnes.end(); i++)
    {
        F2x_set(firstPol, *i);
    }

    GEN secondPol = monomial_F2x(secondPolDegree, 1);
    for(initializer_list<int>::iterator i = secondPolIndexOfOnes.begin(); i < secondPolIndexOfOnes.end(); i++)
    {
        F2x_set(secondPol, *i);
    }

    GEN modPol = monomial_F2x(modPolDegree, 1);
    for(initializer_list<int>::iterator i = modPolIndexOfOnes.begin(); i < modPolIndexOfOnes.end(); i++)
    {
        F2x_set(modPol, *i);
    }

    for (auto _ : state) {
        F2xq_mul(firstPol, secondPol, modPol);
    }
}

static void BM_PolynomField2_Exponentiation(benchmark::State& state, initializer_list<int> basePolIndexOfOnes, long basePolDegree, GEN power, initializer_list<int> modPolIndexOfOnes, long modPolDegree)
{
    GEN basePol = monomial_F2x(basePolDegree, 1);
    for(initializer_list<int>::iterator i = basePolIndexOfOnes.begin(); i < basePolIndexOfOnes.end(); i++)
    {
        F2x_set(basePol, *i);
    }

    GEN modPol = monomial_F2x(modPolDegree, 1);
    for(initializer_list<int>::iterator i = modPolIndexOfOnes.begin(); i < modPolIndexOfOnes.end(); i++)
    {
        F2x_set(modPol, *i);
    }

    for (auto _ : state) {
        F2xq_pow(basePol, power, modPol);
    }
}

static void BM_PolynomField2_MultiplicativeInversion(benchmark::State& state, initializer_list<int> basePolIndexOfOnes, long basePolDegree, initializer_list<int> modPolIndexOfOnes, long modPolDegree)
{
    GEN basePol = monomial_F2x(basePolDegree, 1);
    for(initializer_list<int>::iterator i = basePolIndexOfOnes.begin(); i < basePolIndexOfOnes.end(); i++)
    {
        F2x_set(basePol, *i);
    }

    GEN modPol = monomial_F2x(modPolDegree, 1);
    for(initializer_list<int>::iterator i = modPolIndexOfOnes.begin(); i < modPolIndexOfOnes.end(); i++)
    {
        F2x_set(modPol, *i);
    }
    
    for (auto _ : state) {
        F2xq_inv(basePol, modPol)
    }
}

BENCHMARK_CAPTURE(
    BM_PolynomField2_Addition,
    basic_addition,
    {200, 69, 42, 0},
    211,
    {104, 102, 69, 3, 1},
    200
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_Multiplication,
    basic_multiplication,
    {200, 69, 42, 0},
    211,
    {104, 102, 69, 3, 1},
    200,
    {7, 5, 3, 0},
    409
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent,
    {104, 102, 69, 3, 1},
    200,
    strtoi("114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653"),
    {7, 5, 3, 0},
    409
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_Exponentiation,
    modular_exponentiation_with_reduced_exponent,
    {104, 102, 69, 3, 1},
    200,
    strtoi("545657948216266356582546506665905129604114050233595263203599698308139509840881747025098010154254763608746475177976961773539"),
    {7, 5, 3, 0},
    409
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_MultiplicativeInversion,
    basic_inverse,
    {104, 102, 69, 3, 1},
    200,
    {7, 5, 3, 0},
    409
);

int main(int argc, char** argv)
{
   pari_init(2000000000,0);
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}