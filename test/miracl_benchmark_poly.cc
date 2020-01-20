//g++ -O2 miracl_benchmark_poly.cc /home/vagrant/miracl/polymod.o /home/vagrant/miracl/poly.o /home/vagrant/miracl/ecn.o /home/vagrant/miracl/crt.o /home/vagrant/miracl/zzn.o /home/vagrant/miracl/big.o /home/vagrant/miracl/miracl.a -I/home/vagrant/miracl -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -o miracl_benchmark_poly.o

extern "C"
{
    #include "miracl.h"
}
#include "polymod.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

typedef struct GeneralPolynomialElement
{
    unsigned int coefficient;
    unsigned int exponent;
} GeneralPolynomialElement;


Variable x;
Miracl miraclPrecision=1000;
miracl *mip=&miraclPrecision;

int p;
int q;
vector<GeneralPolynomialElement> irreduciblePolynomial;
vector<GeneralPolynomialElement> polynomialA;
vector<GeneralPolynomialElement> polynomialB;
string scalar;
string benchmarkExponent;
string reducedBenchmarkExponent;

//GF(p^q) TESTS

//TODO test
Poly generalPolynomialToMiraclPolynomial(vector<GeneralPolynomialElement> generalPolynomial)
{
    Poly miraclPolynomial;
    for(int i = 0; i < generalPolynomial.size(); i++)
    {
        miraclPolynomial += generalPolynomial[i].coefficient * pow(x, generalPolynomial[i].exponent);
    }

    return miraclPolynomial;
}

static void BM_PolynomField_Addition(benchmark::State& state, Poly firstPoly, Poly secondPoly)
{
  for (auto _ : state) {
    firstPoly + secondPoly;
  }
}

static void BM_PolynomField_Scalar_Multiplication(benchmark::State& state, Poly basePoly, Big scalar)
{
  for (auto _ : state) {
    scalar * basePoly;
  }
}

static void BM_PolynomField_Multiplication(benchmark::State& state, Poly firstPoly, Poly secondPoly, Poly modPoly)
{
  setmod(modPoly);
  PolyMod firstPolyMod(firstPoly);
  PolyMod secondPolyMod(secondPoly);

  for (auto _ : state) {
    firstPolyMod * secondPolyMod;
  }
}

static void BM_PolynomField_Exponentiation(benchmark::State& state, Poly basePoly, Big power, Poly modPoly)
{
  setmod(modPoly);
  PolyMod basePolyMod(basePoly);

  for (auto _ : state) {
    pow(basePolyMod, power);
  }
}

static void BM_PolynomField_MultiplicativeInversion(benchmark::State& state, Poly basePoly, Poly modPoly)
{
  Poly result[3];
  for (auto _ : state) {
    egcd(result, basePoly, modPoly);
  }
}

BENCHMARK_CAPTURE(
    BM_PolynomField_Addition,
    basic_addition_miracl,
    generalPolynomialToMiraclPolynomial(polynomialA),
    generalPolynomialToMiraclPolynomial(polynomialB)
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Scalar_Multiplication,
    basic_scalar_multiplication,
    generalPolynomialToMiraclPolynomial(polynomialA),
    Big((char*)scalar.c_str())
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Multiplication,
    basic_multiplication_miracl,
    generalPolynomialToMiraclPolynomial(polynomialA),
    generalPolynomialToMiraclPolynomial(polynomialB),
    generalPolynomialToMiraclPolynomial(irreduciblePolynomial)
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent_miracl,
    generalPolynomialToMiraclPolynomial(polynomialA),
    Big((char*)benchmarkExponent.c_str()),
    generalPolynomialToMiraclPolynomial(irreduciblePolynomial)
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Exponentiation,
    modular_exponentiation_with_reduced_exponent_miracl,
    generalPolynomialToMiraclPolynomial(polynomialA),
    Big((char*)reducedBenchmarkExponent.c_str()),
    generalPolynomialToMiraclPolynomial(irreduciblePolynomial)
);

BENCHMARK_CAPTURE(
    BM_PolynomField_MultiplicativeInversion,
    basic_inverse_miracl,
    generalPolynomialToMiraclPolynomial(polynomialA),
    generalPolynomialToMiraclPolynomial(irreduciblePolynomial)
);

int main(int argc, char** argv)
{
    int coefficient, exponent;
    string tmp, polynomialString;
    stringstream polynomialStringStream;

    ifstream benchmarkDataFile;
    benchmarkDataFile.open("benchmark_data.txt");

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

    modulo(Big(p));
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}