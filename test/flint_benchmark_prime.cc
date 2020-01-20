//g++ -O2 flint_benchmark_prime.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lgmp -lflint -lmpfr -o flint_benchmark_prime.o

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <gmp.h>

#include <flint/flint.h>
#include <flint/fmpz.h>
#include <flint/fmpz_poly.h>
#include <flint/fmpz_poly_q.h>
#include <flint/fq.h>
#include <flint/fq_poly.h>
#include <flint/fq_poly_templates.h>

#include <benchmark/benchmark.h>

#include <fstream>
#include <sstream>

using namespace std;

typedef struct GeneralPolynomialElement
{
    string coefficient;
    unsigned int exponent;
} GeneralPolynomialElement;

string p;
string numberA;
string numberB;
string benchmarkExponent;
string reducedBenchmarkExponent;

string generalPolynomialToFlintPolynomialString(vector<GeneralPolynomialElement> generalPolynomial)
{
    string flintPolString;
    flintPolString = to_string(generalPolynomial[0].exponent+1) + "  ";
    int lastExponent = generalPolynomial[generalPolynomial.size() - 1].exponent;

    if(lastExponent != 0)
    {
        for(int j = 0; j < lastExponent; j++)
        {
            flintPolString += "0 ";
        }
    }

    flintPolString += generalPolynomial[generalPolynomial.size() - 1].coefficient + " ";

    for(int i = generalPolynomial.size() - 2; i >= 0; i--)
    {
        for(int j = lastExponent + 1; j < generalPolynomial[i].exponent; j++)
        {
            flintPolString += "0 ";
        }
        lastExponent = generalPolynomial[i].exponent;
        flintPolString += generalPolynomial[i].coefficient + " ";
    }

    return flintPolString;
}

static void BM_PolynomField_Addition(benchmark::State& state, const char* firstPolString, const char* secondPolString, const char* polString, const char* modString)
{
    fmpz_poly_t firstPol;
    fmpz_poly_init(firstPol);
    fmpz_poly_set_str(firstPol, firstPolString);

    fmpz_poly_t secondPol;
    fmpz_poly_init(secondPol);
    fmpz_poly_set_str(secondPol, secondPolString);

    fmpz_t mod;
    fmpz_init(mod);
    fmpz_set_str(mod, modString, 10);

    fmpz_poly_t pol;
    fmpz_poly_init(pol);
    fmpz_poly_set_str(pol, polString);

    fmpz_mod_poly_t polMod;
    fmpz_mod_poly_init(polMod, mod);
    fmpz_mod_poly_set_fmpz_poly(polMod, pol);

    fq_ctx_t ctx;
    fq_ctx_init_modulus(ctx, polMod, "X");

    fmpz_poly_t result;
    fmpz_poly_init(result);
    for (auto _ : state)
    {
        fq_add(result, firstPol, secondPol, ctx);
    }

    //clears
}

static void BM_PolynomField_Scalar_Multiplication(benchmark::State& state, const char* basePolString, const char* scalarString, const char* polString, const char* modString)
{
    fmpz_poly_t basePol;
    fmpz_poly_init(basePol);
    fmpz_poly_set_str(basePol, basePolString);

    fmpz_t scalar;
    fmpz_init(scalar);
    fmpz_set_str(scalar, scalarString, 10);

    fmpz_t mod;
    fmpz_init(mod);
    fmpz_set_str(mod, modString, 10);

    fmpz_poly_t pol;
    fmpz_poly_init(pol);
    fmpz_poly_set_str(pol, polString);

    fmpz_mod_poly_t polMod;
    fmpz_mod_poly_init(polMod, mod);
    fmpz_mod_poly_set_fmpz_poly(polMod, pol);

    fq_ctx_t ctx;
    fq_ctx_init_modulus(ctx, polMod, "X");

    fmpz_poly_t result;
    fmpz_poly_init(result);
    for (auto _ : state)
    {
        fq_mul_fmpz(result, basePol, scalar, ctx);
    }
    
    //clears
}

static void BM_PolynomField_Multiplication(benchmark::State& state, const char* firstPolString, const char* secondPolString, const char* polString, const char* modString)
{
    fmpz_poly_t firstPol;
    fmpz_poly_init(firstPol);
    fmpz_poly_set_str(firstPol, firstPolString);

    fmpz_poly_t secondPol;
    fmpz_poly_init(secondPol);
    fmpz_poly_set_str(secondPol, secondPolString);

    fmpz_t mod;
    fmpz_init(mod);
    fmpz_set_str(mod, modString, 10);

    fmpz_poly_t pol;
    fmpz_poly_init(pol);
    fmpz_poly_set_str(pol, polString);

    fmpz_mod_poly_t polMod;
    fmpz_mod_poly_init(polMod, mod);
    fmpz_mod_poly_set_fmpz_poly(polMod, pol);

    fq_ctx_t ctx;
    fq_ctx_init_modulus(ctx, polMod, "X");

    fmpz_poly_t result;
    fmpz_poly_init(result);
    for (auto _ : state)
    {
        fq_mul(result, firstPol, secondPol, ctx);
    }
    
    //clears
}

static void BM_PolynomField_Exponentiation(benchmark::State& state, const char* basePolString, const char* powerString, const char* polString, const char* modString)
{
    fmpz_poly_t basePol;
    fmpz_poly_init(basePol);
    fmpz_poly_set_str(basePol, basePolString);

    fmpz_t power;
    fmpz_init(power);
    fmpz_set_str(power, powerString, 10);

    fmpz_t mod;
    fmpz_init(mod);
    fmpz_set_str(mod, modString, 10);

    fmpz_poly_t pol;
    fmpz_poly_init(pol);
    fmpz_poly_set_str(pol, polString);

    fmpz_mod_poly_t polMod;
    fmpz_mod_poly_init(polMod, mod);
    fmpz_mod_poly_set_fmpz_poly(polMod, pol);

    fq_ctx_t ctx;
    fq_ctx_init_modulus(ctx, polMod, "X");

    fmpz_poly_t result;
    fmpz_poly_init(result);
    for (auto _ : state)
    {
        fq_pow(result, basePol, power, ctx);
    }
    
    //clears
}

static void BM_PolynomField_MultiplicativeInversion(benchmark::State& state, const char* basePolString, const char* polString, const char* modString)
{
    fmpz_poly_t basePol;
    fmpz_poly_init(basePol);
    fmpz_poly_set_str(basePol, basePolString);

    fmpz_t mod;
    fmpz_init(mod);
    fmpz_set_str(mod, modString, 10);

    fmpz_poly_t pol;
    fmpz_poly_init(pol);
    fmpz_poly_set_str(pol, polString);
    
    fmpz_mod_poly_t polMod;
    fmpz_mod_poly_init(polMod, mod);
    fmpz_mod_poly_set_fmpz_poly(polMod, pol);

    fq_ctx_t ctx;
    fq_ctx_init_modulus(ctx, polMod, "X");

    fmpz_poly_t result;
    fmpz_poly_init(result);
    for (auto _ : state)
    {
        fq_inv(result, basePol, ctx);
    }
    
    //clears
}

BENCHMARK_CAPTURE(
    BM_PolynomField_Addition,
    basic_addition_prime_flint,
    generalPolynomialToFlintPolynomialString({{numberA, 0}}).c_str(),
    generalPolynomialToFlintPolynomialString({{numberB, 0}}).c_str(), 
    generalPolynomialToFlintPolynomialString({{"1", 1}}).c_str(),
    p.c_str()
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Multiplication,
    basic_multiplication_prime_flint,
    generalPolynomialToFlintPolynomialString({{numberA, 0}}).c_str(),
    generalPolynomialToFlintPolynomialString({{numberB, 0}}).c_str(), 
    generalPolynomialToFlintPolynomialString({{"1", 1}}).c_str(),
    p.c_str()
);


BENCHMARK_CAPTURE(
    BM_PolynomField_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent_prime_flint,
    generalPolynomialToFlintPolynomialString({{numberA, 0}}).c_str(),
    benchmarkExponent.c_str(),
    generalPolynomialToFlintPolynomialString({{"1", 1}}).c_str(),
    p.c_str()
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Exponentiation,
    modular_exponentiation_with_reduced_exponent_prime_flint,
    generalPolynomialToFlintPolynomialString({{numberA, 0}}).c_str(),
    reducedBenchmarkExponent.c_str(),
    generalPolynomialToFlintPolynomialString({{"1", 1}}).c_str(),
    p.c_str()
);

BENCHMARK_CAPTURE(
    BM_PolynomField_MultiplicativeInversion,
    basic_inverse_prime_flint,
    generalPolynomialToFlintPolynomialString({{numberA, 0}}).c_str(),
    generalPolynomialToFlintPolynomialString({{"1", 1}}).c_str(),
    p.c_str()
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

    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}