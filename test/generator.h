#include <vector>
#include <pari/pari.h>

using namespace std;

//This only works with small numbers (p and q are both convertable to int)

typedef struct GeneralPolynomialElement
{
    unsigned int coefficient;
    unsigned int exponent;
} GeneralPolynomialElement;

vector<GeneralPolynomialElement> pariPolynomialToGeneralPolynomial(GEN Polynomial, unsigned int PolynomialDegree);

vector<GeneralPolynomialElement> generateIrreduciblePolynomial(unsigned int p, unsigned int q);

char* generatePrimeOfSize(unsigned int bitsize);

vector<GeneralPolynomialElement> generatePolynomialInField(unsigned int p, unsigned int q);

char* generateNumberOfSize(unsigned int bitsize);

void initGenerator();