//g++ -O2 flint_benchmark_poly.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lgmp -lflint -lmpfr -o flint_benchmark_poly.o

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

using namespace std;

typedef struct MyPolyTypeElement
{
    string coefficient;
    int exponent;
} MyPolyTypeElement;

string generateFlintPolString(vector<MyPolyTypeElement> polyElements)
{
    string flintPolString;
    flintPolString = to_string(polyElements[0].exponent+1) + "  ";
    int lastExponent = polyElements[polyElements.size() - 1].exponent;

    if(lastExponent != 0)
    {
        for(int j = 0; j < lastExponent; j++)
        {
            flintPolString += "0 ";
        }
    }

    flintPolString += polyElements[polyElements.size() - 1].coefficient + " ";

    for(int i = polyElements.size() - 2; i >= 0; i--)
    {
        for(int j = lastExponent + 1; j < polyElements[i].exponent; j++)
        {
            flintPolString += "0 ";
        }
        lastExponent = polyElements[i].exponent;
        flintPolString += polyElements[i].coefficient + " ";
    }

    return flintPolString;
}

static void BM_FLINT_PolynomField_Addition(benchmark::State& state, const char* firstPolString, const char* secondPolString, const char* polString, const char* modString)
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

static void BM_FLINT_PolynomField_Scalar_Multiplication(benchmark::State& state, const char* basePolString, const char* scalarString, const char* polString, const char* modString)
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

static void BM_FLINT_PolynomField_Multiplication(benchmark::State& state, const char* firstPolString, const char* secondPolString, const char* polString, const char* modString)
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

static void BM_FLINT_PolynomField_Exponentiation(benchmark::State& state, const char* basePolString, const char* powerString, const char* polString, const char* modString)
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

static void BM_FLINT_PolynomField_MultiplicativeInversion(benchmark::State& state, const char* basePolString, const char* polString, const char* modString)
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
    BM_FLINT_PolynomField_Addition,
    basic_addition,
    generateFlintPolString({{"1", 211}, {"2", 200}, {"1", 69}, {"1", 42}, {"1", 0}}).c_str(),
    generateFlintPolString({{"1", 200}, {"2", 104}, {"1", 102}, {"1", 69}, {"2", 3}, {"1", 1}, {"2", 0}}).c_str(), 
    generateFlintPolString({{"1", 263}, {"2", 3}, {"2", 2}, {"1", 1}, {"1", 0}}).c_str(),
    "3"
);

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Scalar_Multiplication, 
    basic_scalar_multiplication,
    generateFlintPolString({{"1", 200}, {"2", 104}, {"1", 102}, {"1", 69}, {"2", 3}, {"1", 1}, {"2", 0}}).c_str(),
    "114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653",
    generateFlintPolString({{"1", 263}, {"2", 3}, {"2", 2}, {"1", 1}, {"1", 0}}).c_str(),
    "3"
);

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Multiplication,
    basic_multiplication,
    generateFlintPolString({{"1", 211}, {"2", 200}, {"1", 69}, {"1", 42}, {"1", 0}}).c_str(),
    generateFlintPolString({{"1", 200}, {"2", 104}, {"1", 102}, {"1", 69}, {"2", 3}, {"1", 1}, {"2", 0}}).c_str(),
    generateFlintPolString({{"1", 263}, {"2", 3}, {"2", 2}, {"1", 1}, {"1", 0}}).c_str(),
    "3"
);


BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent,
    generateFlintPolString({{"1", 200}, {"2", 104}, {"1", 102}, {"1", 69}, {"2", 3}, {"1", 1}, {"2", 0}}).c_str(),
    "114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653",
    generateFlintPolString({{"1", 263}, {"2", 3}, {"2", 2}, {"1", 1}, {"1", 0}}).c_str(),
    "3"
);

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Exponentiation,
    modular_exponentiation_with_reduced_exponent,
    generateFlintPolString({{"1", 200}, {"2", 104}, {"1", 102}, {"1", 69}, {"2", 3}, {"1", 1}, {"2", 0}}).c_str(),
    "267958267864893013072272255214324825478678748460938376995848570678440230157915342322621691247283190482984938504887672817472893",
    generateFlintPolString({{"1", 263}, {"2", 3}, {"2", 2}, {"1", 1}, {"1", 0}}).c_str(),
    "3"
);

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_MultiplicativeInversion,
    basic_inverse,
    generateFlintPolString({{"1", 200}, {"2", 104}, {"1", 102}, {"1", 69}, {"2", 3}, {"1", 1}, {"2", 0}}).c_str(),
    generateFlintPolString({{"1", 263}, {"2", 3}, {"2", 2}, {"1", 1}, {"1", 0}}).c_str(),
    "3"
);

//////////////////GF2^m

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Addition,
    basic_addition_GF2,
    generateFlintPolString({{"1", 211}, {"1", 200}, {"1", 69}, {"1", 42}, {"1", 0}}).c_str(),
    generateFlintPolString({{"1", 200}, {"1", 104}, {"1", 102}, {"1", 69}, {"1", 3}, {"1", 1}}).c_str(), 
    generateFlintPolString({{"1", 409}, {"1", 7}, {"1", 5}, {"1", 3}, {"1", 0}}).c_str(),
    "2"
);

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Multiplication,
    basic_multiplication_GF2,
    generateFlintPolString({{"1", 211}, {"1", 200}, {"1", 69}, {"1", 42}, {"1", 0}}).c_str(),
    generateFlintPolString({{"1", 200}, {"1", 104}, {"1", 102}, {"1", 69}, {"1", 3}, {"1", 1}}).c_str(),
    generateFlintPolString({{"1", 409}, {"1", 7}, {"1", 5}, {"1", 3}, {"1", 0}}).c_str(),
    "2"
);


BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent_GF2,
    generateFlintPolString({{"1", 200}, {"1", 104}, {"1", 102}, {"1", 69}, {"1", 3}, {"1", 1}}).c_str(),
    "114257861865478804407304804720106648610243131623383809889090591194231435391573141592655245498184889136485344775587546688876340744597107535063109170063376118857367666310205539214537693468661910927465064887750418137402612425763591725557531218816269330626202829315598574929746122951434029268750863832704892328653",
    generateFlintPolString({{"1", 409}, {"1", 7}, {"1", 5}, {"1", 3}, {"1", 0}}).c_str(),
    "2"
);

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Exponentiation,
    modular_exponentiation_with_reduced_exponent_GF2,
    generateFlintPolString({{"1", 200}, {"1", 104}, {"1", 102}, {"1", 69}, {"1", 3}, {"1", 1}}).c_str(),
    "545657948216266356582546506665905129604114050233595263203599698308139509840881747025098010154254763608746475177976961773539",
    generateFlintPolString({{"1", 409}, {"1", 7}, {"1", 5}, {"1", 3}, {"1", 0}}).c_str(),
    "2"
);

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_MultiplicativeInversion,
    basic_inverse_GF2,
    generateFlintPolString({{"1", 200}, {"1", 104}, {"1", 102}, {"1", 69}, {"1", 3}, {"1", 1}}).c_str(),
    generateFlintPolString({{"1", 409}, {"1", 7}, {"1", 5}, {"1", 3}, {"1", 0}}).c_str(),
    "2"
);

//////////////////prime field

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Addition,
    basic_addition_prime,
    generateFlintPolString({{"654634343526546343435235252356546343435235252351242543642626346265473554345345345346345235235352356234364262623654634343523525235436426262335252354364262623", 0}}).c_str(),
    generateFlintPolString({{"65463434352654634343431212312552346326546343426546343434312123343121232342346426262365463434352352526546343434312123235436426262335252354364262623", 0}}).c_str(), 
    generateFlintPolString({{"1", 1}}).c_str(),
    "127064667816153171453188458361121716837742586505814733181241470456786145066345547978484036504112599410542843490798748221028371638017831629596721969094482336338121230226570799909681840514956441130146955529333898991319999073734340328211327012424337328469145574215738612082413345484375774166766470122537302830033"
);

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Multiplication,
    basic_multiplication_prime,
    generateFlintPolString({{"654634343526546343435235252356546343435235252351242543642626346265473554345345345346345235235352356234364262623654634343523525235436426262335252354364262623", 0}}).c_str(),
    generateFlintPolString({{"65463434352654634343431212312552346326546343426546343434312123343121232342346426262365463434352352526546343434312123235436426262335252354364262623", 0}}).c_str(), 
    generateFlintPolString({{"1", 1}}).c_str(),
    "127064667816153171453188458361121716837742586505814733181241470456786145066345547978484036504112599410542843490798748221028371638017831629596721969094482336338121230226570799909681840514956441130146955529333898991319999073734340328211327012424337328469145574215738612082413345484375774166766470122537302830033"
);


BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Exponentiation,
    modular_exponentiation_with_not_reduced_exponent_prime,
    generateFlintPolString({{"65463434352654634343431212312552346326546343426546343434312123343121232342346426262365463434352352526546343434312123235436426262335252354364262623", 0}}).c_str(),
    "964208249802749964208249802749029642082498027490265927832937489237821721648750706842187461824690260746806592783293796420824980274902659278329374892378217216487507068421874618246902607468048996420824980274902659278329374892378217216487507068421874618246902607468023782172196420824980274902659278329374892378217216487507068421874618246902607468064879642082498027490265927832937489237821721648750706842187461824690260746805070684218746182496420824980274902659278329374892378217216487507068421874618246902607468069026074680026592783293748923782172964208249802749026592783293748923782172164875070684218746182469026074680164875070684218746182469642082498027490265927832937489237821721648750706842187461824690260746809026074680",
    generateFlintPolString({{"1", 1}}).c_str(),
    "127064667816153171453188458361121716837742586505814733181241470456786145066345547978484036504112599410542843490798748221028371638017831629596721969094482336338121230226570799909681840514956441130146955529333898991319999073734340328211327012424337328469145574215738612082413345484375774166766470122537302830033"
);

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_Exponentiation,
    modular_exponentiation_with_reduced_exponent_prime,
    generateFlintPolString({{"65463434352654634343431212312552346326546343426546343434312123343121232342346426262365463434352352526546343434312123235436426262335252354364262623", 0}}).c_str(),
    "29920919596070184232543104448733767801216164681815517738158399461869100719095892740285237041906414361045004203494212747763034362349849701968950328158893069677127144346126543948759469151772810256933700029271078416426709808675804407619702712540005353228326944885936488468999763784817403917659567907409539253480",
    generateFlintPolString({{"1", 1}}).c_str(),
    "127064667816153171453188458361121716837742586505814733181241470456786145066345547978484036504112599410542843490798748221028371638017831629596721969094482336338121230226570799909681840514956441130146955529333898991319999073734340328211327012424337328469145574215738612082413345484375774166766470122537302830033"
);

BENCHMARK_CAPTURE(
    BM_FLINT_PolynomField_MultiplicativeInversion,
    basic_inverse_prime,
    generateFlintPolString({{"654634343526546343435235252356546343435235252351242543642626346265473554345345345346345235235352356234364262623654634343523525235436426262335252354364262623", 0}}).c_str(),
    generateFlintPolString({{"1", 1}}).c_str(),
    "127064667816153171453188458361121716837742586505814733181241470456786145066345547978484036504112599410542843490798748221028371638017831629596721969094482336338121230226570799909681840514956441130146955529333898991319999073734340328211327012424337328469145574215738612082413345484375774166766470122537302830033"
);

BENCHMARK_MAIN();