//g++ -O2 miracl_benchmark_poly2.cc /home/vagrant/miracl/poly2mod.o /home/vagrant/miracl/poly2.o /home/vagrant/miracl/ec2.o /home/vagrant/miracl/gf2m.o /home/vagrant/miracl/ecn.o /home/vagrant/miracl/crt.o /home/vagrant/miracl/zzn.o /home/vagrant/miracl/big.o /home/vagrant/miracl/miracl.a -I/home/vagrant/miracl -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -o miracl_benchmark_poly2.o

extern "C"
{
    #include "miracl.h"
}
#include "poly2mod.h"
#include <benchmark/benchmark.h>

Variable x;
Miracl precision=100;
miracl *mip=&precision;

//GF(2^m) TESTS
static void BM_PolynomField2_Addition(benchmark::State& state, Poly2 firstPoly2, Poly2 secondPoly2, BOOL check)
{
    for (auto _ : state) {
        firstPoly2 + secondPoly2;
    }
}

static void BM_PolynomField2_Multiplication(benchmark::State& state, Poly2 modPoly2, Poly2 firstPoly2, Poly2 secondPoly2, BOOL check)
{
    setmod(modPoly2);
    Poly2Mod firstPoly2Mod(firstPoly2);
    Poly2Mod secondPoly2Mod(secondPoly2);

    for (auto _ : state) {
        firstPoly2Mod * secondPoly2Mod;
    }
}

static void BM_PolynomField2_Exponentiation(benchmark::State& state, Big power, Poly2 modPoly2, Poly2 basePoly2, BOOL check)
{
    for (auto _ : state) {
        pow(basePoly2, power, modPoly2);
    }
}

static void BM_PolynomField2_MultiplicativeInversion(benchmark::State& state, Poly2 modPoly2, Poly2 basePoly2, BOOL check)
{
    Poly2 result[3];
    for (auto _ : state) {
        egcd(result, basePoly2, modPoly2);
    }
}

// Register the PolynomField2 functions as benchmarks

BENCHMARK_CAPTURE(BM_PolynomField2_Addition, basic_addition, pow2(x,211)+pow2(x,200)+pow2(x,69)+pow2(x,42)+1, pow2(x,200)+pow2(x,104)+pow2(x,102)+pow2(x,69)+pow2(x,3)+x, modulo(409, 7, 5, 3, TRUE));

BENCHMARK_CAPTURE(BM_PolynomField2_Multiplication, basic_multiplication, pow2(x,409)+pow2(x,7)+pow2(x,5)+pow2(x,3)+1, pow2(x,211)+pow2(x,200)+pow2(x,69)+pow2(x,42)+1, pow2(x,200)+pow2(x,104)+pow2(x,102)+pow2(x,69)+pow2(x,3)+x, modulo(409, 7, 5, 3, TRUE));

BENCHMARK_CAPTURE(BM_PolynomField2_Exponentiation, modular_exponentiation_with_not_reduced_exponent, Big("114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653"), pow2(x,409)+pow2(x,7)+pow2(x,5)+pow2(x,3)+1, pow2(x,200)+pow2(x,104)+pow2(x,102)+pow2(x,69)+pow2(x,3)+x, modulo(409, 7, 5, 3, TRUE));

BENCHMARK_CAPTURE(BM_PolynomField2_Exponentiation, modular_exponentiation_with_reduced_exponent, Big("545657948216266356582546506665905129604114050233595263203599698308139509840881747025098010154254763608746475177976961773539"), pow2(x,409)+pow2(x,7)+pow2(x,5)+pow2(x,3)+1, pow2(x,200)+pow2(x,104)+pow2(x,102)+pow2(x,69)+pow2(x,3)+x, modulo(409, 7, 5, 3, TRUE));

BENCHMARK_CAPTURE(BM_PolynomField2_MultiplicativeInversion, basic_inverse, pow2(x,409)+pow2(x,7)+pow2(x,5)+pow2(x,3)+1, pow2(x,200)+pow2(x,104)+pow2(x,102)+pow2(x,69)+pow2(x,3)+x, modulo(409, 7, 5, 3, TRUE));

// Run the benchmark
BENCHMARK_MAIN();