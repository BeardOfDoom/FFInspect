#include "generator.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        cout << "The arguments must be the next:" << endl;
        cout << "First: name of the output (poly_benchmark_data.txt OR poly2_benchmark_data.txt OR prime_benchmark_data.txt)." << endl;
        cout << "Second: a p prime (characteristic of the field)." << endl;
        cout << "Third: a q integer (order of the irreducible polynomial defining the field)." << endl;
        cout << "If q is 0, the program assumes that p is not a prime, but the bitlength of the prime that needs to be generated." << endl;

        return -1;
    }

    ofstream benchmarkDataFile;
    benchmarkDataFile.open(argv[1]);

    initGenerator();

    char* scalar = generateNumberOfSize(200);
    char* exponent = generateNumberOfSize(2000);
    char* reducedExponent;

    int q = atoi(argv[3]);
    if(q == 0)
    {
        char* p = generatePrimeOfSize(atoi(argv[2]));

        char* numberA = generateNumberInRange(strtoi(p));
        char* numberB = generateNumberInRange(strtoi(p));

        GEN pPari = strtoi(p);
        GEN exponentPari = strtoi(exponent);
        GEN reducedExponentPari = modii(exponentPari, subiu(pPari, 1));
        reducedExponent = itostr(reducedExponentPari);

        benchmarkDataFile << p << endl;

        benchmarkDataFile << numberA << endl;

        benchmarkDataFile << numberB << endl;
    }
    else
    {
        int p = atoi(argv[2]);

        vector<GeneralPolynomialElement> irreduciblePolynomial = generateIrreduciblePolynomial(p, q);
        vector<GeneralPolynomialElement> polynomialA = generatePolynomialInField(p, q);
        vector<GeneralPolynomialElement> polynomialB = generatePolynomialInField(p, q);

        GEN fieldOrder = powuu(p, q);
        GEN exponentPari = strtoi(exponent);
        GEN reducedExponentPari = modii(exponentPari, subiu(fieldOrder, 1));
        reducedExponent = itostr(reducedExponentPari);

        benchmarkDataFile << p << endl;

        benchmarkDataFile << q << endl;

        for(int i = 0; i < irreduciblePolynomial.size() - 1; i++)
        {
            benchmarkDataFile << irreduciblePolynomial[i].coefficient << " " << irreduciblePolynomial[i].exponent << " ";
        }
        benchmarkDataFile << irreduciblePolynomial[irreduciblePolynomial.size() - 1].coefficient << " " << irreduciblePolynomial[irreduciblePolynomial.size() - 1].exponent << endl;

        for(int i = 0; i < polynomialA.size() - 1; i++)
        {
            benchmarkDataFile << polynomialA[i].coefficient << " " << polynomialA[i].exponent << " ";
        }
        benchmarkDataFile << polynomialA[polynomialA.size() - 1].coefficient << " " << polynomialA[polynomialA.size() - 1].exponent << endl;

        for(int i = 0; i < polynomialB.size() - 1; i++)
        {
            benchmarkDataFile << polynomialB[i].coefficient << " " << polynomialB[i].exponent << " ";
        }
        benchmarkDataFile << polynomialB[polynomialB.size() - 1].coefficient << " " << polynomialB[polynomialB.size() - 1].exponent << endl;

    }

    benchmarkDataFile << scalar << endl;

    benchmarkDataFile << exponent << endl;

    benchmarkDataFile << reducedExponent << endl;

    benchmarkDataFile.close();

    return 0;
}