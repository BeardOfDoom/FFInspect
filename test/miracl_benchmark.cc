/*
 * g++ -O2 miracl_benchmark.cc ../home/vagrant/miracl/polymod.o ../home/vagrant/miracl/poly.o ../home/vagrant/miracl/ecn.o ../home/vagrant/miracl/crt.o ../home/vagrant/miracl/zzn.o ../home/vagrant/miracl/big.o ../home/vagrant/miracl/miracl.a -I../home/vagrant/miracl -std=c++11 -isystem ../home/vagrant/benchmark/include/ -lbenchmark -lpthread -L../home/vagrant/benchmark/build/src -o mybench
 * 
 * Program to calculate factorials.
 */

#include <stdio.h>
extern "C"
{
    #include "miracl.h"
}
#include "polymod.h"
#include <benchmark/benchmark.h>

Variable x;
Miracl precision=100;
miracl *mip=&precision;

Big init(Big modulus)
{
  modulo(modulus);

  return modulus;
}

//PRIME FIELD TESTS

static void BM_PrimeField_Addition(benchmark::State& state) {

}

static void BM_PrimeField_Multiplication(benchmark::State& state) {
  
}

static void BM_PrimeField_Exponentiation(benchmark::State& state) {
  
}

static void BM_PrimeField_MultiplicativeInversion(benchmark::State& state) {
  
}

//GF(2^m) TESTS

static void BM_PolynomField2_Addition(benchmark::State& state) {

}

static void BM_PolynomField2_Multiplication(benchmark::State& state) {
  
}

static void BM_PolynomField2_Exponentiation(benchmark::State& state) {
  
}

static void BM_PolynomField2_MultiplicativeInversion(benchmark::State& state) {
  
}

//GF(p^m) TESTS

static void BM_PolynomField_Addition(benchmark::State& state, Poly firstPoly, Poly secondPoly, Big mod)
{
  for (auto _ : state) {
    firstPoly + secondPoly;
  }
}

static void BM_PolynomField_Scalar_Multiplication(benchmark::State& state, Poly basePoly, Big scalar, Big mod)
{
  for (auto _ : state) {
    scalar * basePoly;
  }
}

static void BM_PolynomField_Multiplication(benchmark::State& state, Poly modPoly, Poly firstPoly, Poly secondPoly, Big mod)
{
  setmod(modPoly);
  PolyMod firstPolyMod(firstPoly);
  PolyMod secondPolyMod(secondPoly);

  for (auto _ : state) {
    firstPolyMod * secondPolyMod;
  }
}

static void BM_PolynomField_Exponentiation(benchmark::State& state, Big power, Poly modPoly, Poly basePoly, Big mod)
{
  setmod(modPoly);
  PolyMod basePolyMod(basePoly);

  for (auto _ : state) {
    pow(basePolyMod, power);
  }

  //destroys?
}

static void BM_PolynomField_MultiplicativeInversion(benchmark::State& state, Poly modPoly, Poly base, Big mod)
{
  Poly result[3];
  for (auto _ : state) {
    egcd(result, base, modPoly);
  }
}

// Register the function as a benchmark

BENCHMARK_CAPTURE(BM_PolynomField_Addition, basic_addition, pow(x,211)+2*pow(x,200)+pow(x,69)+pow(x,42)+1, pow(x,200)+2*pow(x,104)+pow(x,102)+pow(x,69)+2*pow(x,3)+x+2, init(Big(3)));

BENCHMARK_CAPTURE(BM_PolynomField_Scalar_Multiplication, basic_scalar_multiplication, pow(x,200)+2*pow(x,104)+pow(x,102)+pow(x,69)+2*pow(x,3)+x+2, Big("114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653"), init(Big(3)));

BENCHMARK_CAPTURE(BM_PolynomField_Multiplication, basic_multiplication,  pow(x,263)+2*pow(x,3)+2*pow(x,2)+x+1, pow(x,211)+2*pow(x,200)+pow(x,69)+pow(x,42)+1, pow(x,200)+2*pow(x,104)+pow(x,102)+pow(x,69)+2*pow(x,3)+x+2, init(Big(3)));

BENCHMARK_CAPTURE(BM_PolynomField_Exponentiation, modular_exponentiation_with_not_reduced_exponent, Big("114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653"), pow(x,263)+2*pow(x,3)+2*pow(x,2)+x+1, pow(x,200)+2*pow(x,104)+pow(x,102)+pow(x,69)+2*pow(x,3)+x+2, init(Big(3)));

BENCHMARK_CAPTURE(BM_PolynomField_Exponentiation, modular_exponentiation_with_reduced_exponent, Big("267958267864893013072272255214324825478678748460938376995848570678440230157915342322621691247283190482984938504887672817472893"), pow(x,263)+2*pow(x,3)+2*pow(x,2)+x+1, pow(x,200)+2*pow(x,104)+pow(x,102)+pow(x,69)+2*pow(x,3)+x+2, init(Big(3)));

BENCHMARK_CAPTURE(BM_PolynomField_MultiplicativeInversion, basic_inverse, pow(x,263)+2*pow(x,3)+2*pow(x,2)+x+1, pow(x,200)+2*pow(x,104)+pow(x,102)+pow(x,69)+2*pow(x,3)+x+2, init(Big(3)));


// Run the benchmark
BENCHMARK_MAIN();