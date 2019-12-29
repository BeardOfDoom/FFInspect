//g++ -O2 pari_benchmark_poly.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lpari -o pari_benchmark_poly.o

#include <pari/pari.h>
#include <benchmark/benchmark.h>
#include <vector>

using namespace std;

typedef struct MyPolyTypeElement
{
    GEN coefficient;
    int exponent;
} MyPolyTypeElement;

GEN mymkpoln(vector<MyPolyTypeElement> polyElements)
{
    GEN x, y;
    long degree = polyElements[0].exponent;
    long l = degree+3;
    x = cgetg(l, t_POL); y = x + 2;
    x[1] = evalvarn(0);

    int lastExponent = polyElements[0].exponent;
    gel(y,lastExponent) = polyElements[0].coefficient;
    for(int i = 1; i < polyElements.size(); i++)
    {
        for(int j = lastExponent - 1; j > polyElements[i].exponent; j--)
        {
            gel(y,j) = gen_0;
        }
        lastExponent = polyElements[i].exponent;
        gel(y,lastExponent) = polyElements[i].coefficient;
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
    basic_addition,
    mymkpoln({{gen_1, 211}, {gen_2, 200}, {gen_1, 69}, {gen_1, 42}, {gen_1, 0}}),
    mymkpoln({{gen_1, 200}, {gen_2, 104}, {gen_1, 102}, {gen_1, 69}, {gen_2, 3}, {gen_1, 1}, {gen_2, 0}}),
    strtoi("3")
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Scalar_Multiplication, 
    basic_scalar_multiplication,
    mymkpoln({{gen_1, 200}, {gen_2, 104}, {gen_1, 102}, {gen_1, 69}, {gen_2, 3}, {gen_1, 1}, {gen_2, 0}}),
    strtoi("114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653"),
    strtoi("3")
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Multiplication,
    basic_multiplication,
    mymkpoln({{gen_1, 211}, {gen_2, 200}, {gen_1, 69}, {gen_1, 42}, {gen_1, 0}}),
    mymkpoln({{gen_1, 200}, {gen_2, 104}, {gen_1, 102}, {gen_1, 69}, {gen_2, 3}, {gen_1, 1}, {gen_2, 0}}),
    mymkpoln({{gen_1, 263}, {gen_2, 3}, {gen_2, 2}, {gen_1, 1}, {gen_1, 0}}),
    strtoi("3")
);


BENCHMARK_CAPTURE(
    BM_PolynomField_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent,
    mymkpoln({{gen_1, 200}, {gen_2, 104}, {gen_1, 102}, {gen_1, 69}, {gen_2, 3}, {gen_1, 1}, {gen_2, 0}}),
    strtoi("114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653"),
    mymkpoln({{gen_1, 263}, {gen_2, 3}, {gen_2, 2}, {gen_1, 1}, {gen_1, 0}}),
    strtoi("3")
);

BENCHMARK_CAPTURE(
    BM_PolynomField_Exponentiation,
    modular_exponentiation_with_reduced_exponent,
    mymkpoln({{gen_1, 200}, {gen_2, 104}, {gen_1, 102}, {gen_1, 69}, {gen_2, 3}, {gen_1, 1}, {gen_2, 0}}),
    strtoi("267958267864893013072272255214324825478678748460938376995848570678440230157915342322621691247283190482984938504887672817472893"),
    mymkpoln({{gen_1, 263}, {gen_2, 3}, {gen_2, 2}, {gen_1, 1}, {gen_1, 0}}),
    strtoi("3")
);

BENCHMARK_CAPTURE(
    BM_PolynomField_MultiplicativeInversion,
    basic_inverse,
    mymkpoln({{gen_1, 200}, {gen_2, 104}, {gen_1, 102}, {gen_1, 69}, {gen_2, 3}, {gen_1, 1}, {gen_2, 0}}),
    mymkpoln({{gen_1, 263}, {gen_2, 3}, {gen_2, 2}, {gen_1, 1}, {gen_1, 0}}),
    strtoi("3")
);

int main(int argc, char** argv)
{
   pari_init(2000000000,0);
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}