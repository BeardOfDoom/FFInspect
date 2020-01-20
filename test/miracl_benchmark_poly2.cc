//g++ -O2 miracl_benchmark_poly2.cc /home/vagrant/miracl/poly2mod.o /home/vagrant/miracl/poly2.o /home/vagrant/miracl/ec2.o /home/vagrant/miracl/gf2m.o /home/vagrant/miracl/ecn.o /home/vagrant/miracl/crt.o /home/vagrant/miracl/zzn.o /home/vagrant/miracl/big.o /home/vagrant/miracl/miracl.a -I/home/vagrant/miracl -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -o miracl_benchmark_poly2.o

extern "C"
{
    #include "miracl.h"
}
#include "poly2mod.h"
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

//GF(2^m) TESTS

Poly2 generalPolynomialToMiraclPolynomial(vector<GeneralPolynomialElement> generalPolynomial)
{
    Poly2 miraclPolynomial;
    for(int i = 0; i < generalPolynomial.size(); i++)
    {
        miraclPolynomial += pow2(x, generalPolynomial[i].exponent);
    }

    return miraclPolynomial;
}

static void BM_PolynomField2_Addition(benchmark::State& state, Poly2 firstPoly2, Poly2 secondPoly2)
{
    for (auto _ : state) {
        firstPoly2 + secondPoly2;
    }
}

static void BM_PolynomField2_Multiplication(benchmark::State& state, Poly2 firstPoly2, Poly2 secondPoly2, Poly2 modPoly2)
{
    setmod(modPoly2);
    Poly2Mod firstPoly2Mod(firstPoly2);
    Poly2Mod secondPoly2Mod(secondPoly2);

    for (auto _ : state) {
        firstPoly2Mod * secondPoly2Mod;
    }
}

static void BM_PolynomField2_Exponentiation(benchmark::State& state, Poly2 basePoly2, Big power, Poly2 modPoly2)
{
    for (auto _ : state) {
        pow(basePoly2, power, modPoly2);
    }
}

static void BM_PolynomField2_MultiplicativeInversion(benchmark::State& state, Poly2 basePoly2, Poly2 modPoly2)
{
    Poly2 result[3];
    for (auto _ : state) {
        egcd(result, basePoly2, modPoly2);
    }
}

BENCHMARK_CAPTURE(
    BM_PolynomField2_Addition,
    basic_addition_miracl,
    generalPolynomialToMiraclPolynomial(polynomialA),
    generalPolynomialToMiraclPolynomial(irreduciblePolynomial)
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_Multiplication,
    basic_multiplication_miracl,
    generalPolynomialToMiraclPolynomial(polynomialA),
    generalPolynomialToMiraclPolynomial(polynomialB),
    generalPolynomialToMiraclPolynomial(irreduciblePolynomial)
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent_miracl,
    generalPolynomialToMiraclPolynomial(polynomialA),
    Big((char*)benchmarkExponent.c_str()),
    generalPolynomialToMiraclPolynomial(irreduciblePolynomial)
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_Exponentiation,
    modular_exponentiation_with_reduced_exponent_miracl,
    generalPolynomialToMiraclPolynomial(polynomialA),
    Big((char*)reducedBenchmarkExponent.c_str()),
    generalPolynomialToMiraclPolynomial(irreduciblePolynomial)
);

BENCHMARK_CAPTURE(
    BM_PolynomField2_MultiplicativeInversion,
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

  //this is stupid, but necessary...
  modulo(q, 3, 2, 1, FALSE);
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
}