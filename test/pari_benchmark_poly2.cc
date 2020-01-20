//g++ -O2 pari_benchmark_poly2.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lpari -o pari_benchmark_poly2.o

#include <pari/pari.h>
#include <benchmark/benchmark.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

typedef struct GeneralPolynomialElement
{
    unsigned int coefficient;
    unsigned int exponent;
} GeneralPolynomialElement;

int p;
int q;
vector<GeneralPolynomialElement> irreduciblePolynomial;
vector<GeneralPolynomialElement> polynomialA;
vector<GeneralPolynomialElement> polynomialB;
string scalar;
string benchmarkExponent;
string reducedBenchmarkExponent;

GEN generalPolynomialToPariPolynomial(vector<GeneralPolynomialElement> generalPolynomial)
{
    GEN pariPolynomial = monomial_F2x(generalPolynomial.size() - 1, 0);
    for(int i = 1; i < generalPolynomial.size(); i++)
    {
        F2x_set(pariPolynomial, generalPolynomial[i].exponent);
    }
    
    return pariPolynomial;
}

static void BM_PolynomField2_Addition(benchmark::State& state, GEN firstPol, GEN secondPol)
{
    for (auto _ : state) {
        F2x_add(firstPol, secondPol);
    }
}


static void BM_PolynomField2_Multiplication(benchmark::State& state, GEN firstPol, GEN secondPol, GEN modPol)
{
    for (auto _ : state) {
        F2xq_mul(firstPol, secondPol, modPol);
    }
}

static void BM_PolynomField2_Exponentiation(benchmark::State& state,GEN basePol, GEN power, GEN modPol)
{
    for (auto _ : state) {
        F2xq_pow(basePol, power, modPol);
    }
}

static void BM_PolynomField2_MultiplicativeInversion(benchmark::State& state, GEN basePol, GEN modPol)
{
    for (auto _ : state) {
        F2xq_inv(basePol, modPol);
    }
}

BENCHMARK_CAPTURE(
    BM_PolynomField2_Addition,
    basic_addition,
    generalPolynomialToPariPolynomial(polynomialA),
    generalPolynomialToPariPolynomial(polynomialB)
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_Multiplication,
    basic_multiplication,
    generalPolynomialToPariPolynomial(polynomialA),
    generalPolynomialToPariPolynomial(polynomialB),
    generalPolynomialToPariPolynomial(irreduciblePolynomial)
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent,
    generalPolynomialToPariPolynomial(polynomialA),
    strtoi(benchmarkExponent.c_str()),
    generalPolynomialToPariPolynomial(irreduciblePolynomial)
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_Exponentiation,
    modular_exponentiation_with_reduced_exponent,
    generalPolynomialToPariPolynomial(polynomialA),
    strtoi(reducedBenchmarkExponent.c_str()),
    generalPolynomialToPariPolynomial(irreduciblePolynomial)
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_MultiplicativeInversion,
    basic_inverse,
    generalPolynomialToPariPolynomial(polynomialA),
    generalPolynomialToPariPolynomial(irreduciblePolynomial)
);

int main(int argc, char** argv)
{
    int coefficient, exponent;
    string tmp, polynomialString;
    stringstream polynomialStringStream;

    ifstream benchmarkDataFile;
    benchmarkDataFile.open("poly2_benchmark_data.txt");

    benchmarkDataFile >> p;

    benchmarkDataFile >> q;

    getline(benchmarkDataFile, tmp);

    polynomialString = "";
    getline(benchmarkDataFile, polynomialString);
    polynomialStringStream << polynomialString;
    while(!polynomialStringStream.eof())
    {
        polynomialStringStream >> coefficient;
        polynomialStringStream >> exponent;
        irreduciblePolynomial.push_back({coefficient, exponent});
    }
    polynomialStringStream.clear();

    polynomialString = "";
    getline(benchmarkDataFile, polynomialString);
    polynomialStringStream << polynomialString;
    while(!polynomialStringStream.eof())
    {
        polynomialStringStream >> coefficient;
        polynomialStringStream >> exponent;
        polynomialA.push_back({coefficient, exponent});
    }
    polynomialStringStream.clear();

    polynomialString = "";
    getline(benchmarkDataFile, polynomialString);
    polynomialStringStream << polynomialString;
    while(!polynomialStringStream.eof())
    {
        polynomialStringStream >> coefficient;
        polynomialStringStream >> exponent;
        polynomialB.push_back({coefficient, exponent});
    }
    polynomialStringStream.clear();
    
    getline(benchmarkDataFile, scalar);

    getline(benchmarkDataFile, benchmarkExponent);

    getline(benchmarkDataFile, reducedBenchmarkExponent);

    benchmarkDataFile.close();

   pari_init(2000000000,0);
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}