//g++ -O2 pari_benchmark_poly.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lpari -o pari_benchmark_poly.o

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
    GEN x, y;
    long degree = generalPolynomial[0].exponent;
    long l = degree+3;
    x = cgetg(l, t_POL); y = x + 2;
    x[1] = evalvarn(0);

    int lastExponent = generalPolynomial[0].exponent;
    gel(y,lastExponent) = utoi(generalPolynomial[0].coefficient);
    for(int i = 1; i < generalPolynomial.size(); i++)
    {
        for(int j = lastExponent - 1; j > generalPolynomial[i].exponent; j--)
        {
            gel(y,j) = gen_0;
        }
        lastExponent = generalPolynomial[i].exponent;
        gel(y,lastExponent) = utoi(generalPolynomial[i].coefficient);
    }
    if(lastExponent != 0)
    {
        for(int j = lastExponent; j >= 0; j--)
        {
            gel(y,j) = gen_0;
        }
    }
    return normalizepol_lg(x, l);
}

static void BM_PolynomField_Addition(benchmark::State& state, GEN firstPol, GEN secondPol, GEN mod)
{
    for (auto _ : state)
    {
        FpX_add(firstPol, secondPol, mod);
    }
}

static void BM_PolynomField_Scalar_Multiplication(benchmark::State& state, GEN pol, GEN scalar, GEN mod)
{
    for (auto _ : state)
    {
        FpX_Fp_mul(pol, scalar, mod);
    }
}

static void BM_PolynomField_Multiplication(benchmark::State& state, GEN firstPol, GEN secondPol, GEN polMod, GEN mod)
{
    for (auto _ : state)
    {
        FpXQ_mul(firstPol, secondPol, polMod, mod);
    }
}

static void BM_PolynomField_Exponentiation(benchmark::State& state, GEN basePol, GEN power, GEN polMod, GEN mod)
{
    for (auto _ : state)
    {
        FpXQ_pow(basePol, power, polMod, mod);
    }
}

static void BM_PolynomField_MultiplicativeInversion(benchmark::State& state, GEN basePol, GEN polMod, GEN mod)
{
    for (auto _ : state)
    {
        FpXQ_inv(basePol, polMod, mod);
    }
}

BENCHMARK_CAPTURE(
    BM_PolynomField_Addition,
    basic_addition_pari,
    generalPolynomialToPariPolynomial(polynomialA),
    generalPolynomialToPariPolynomial(polynomialB),
    utoi(p)
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Scalar_Multiplication, 
    basic_scalar_multiplication_pari,
    generalPolynomialToPariPolynomial(polynomialA),
    strtoi(scalar.c_str()),
    utoi(p)
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Multiplication,
    basic_multiplication_pari,
    generalPolynomialToPariPolynomial(polynomialA),
    generalPolynomialToPariPolynomial(polynomialB),
    generalPolynomialToPariPolynomial(irreduciblePolynomial),
    utoi(p)
);


BENCHMARK_CAPTURE(
    BM_PolynomField_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent_pari,
    generalPolynomialToPariPolynomial(polynomialA),
    strtoi(benchmarkExponent.c_str()),
    generalPolynomialToPariPolynomial(irreduciblePolynomial),
    utoi(p)
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Exponentiation,
    modular_exponentiation_with_reduced_exponent_pari,
    generalPolynomialToPariPolynomial(polynomialA),
    strtoi(reducedBenchmarkExponent.c_str()),
    generalPolynomialToPariPolynomial(irreduciblePolynomial),
    utoi(p)
);

BENCHMARK_CAPTURE(
    BM_PolynomField_MultiplicativeInversion,
    basic_inverse_pari,
    generalPolynomialToPariPolynomial(polynomialA),
    generalPolynomialToPariPolynomial(irreduciblePolynomial),
    utoi(p)
);

int main(int argc, char** argv)
{
    int coefficient, exponent;
    string tmp, polynomialString;
    stringstream polynomialStringStream;

    ifstream benchmarkDataFile;
    benchmarkDataFile.open("poly_benchmark_data.txt");

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