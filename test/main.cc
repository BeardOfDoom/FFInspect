#include "generator.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    initGenerator();
    int p = 11;
    int q = 100;
    vector<GeneralPolynomialElement> irreduciblePolynomial = generateIrreduciblePolynomial(p, q);
    vector<GeneralPolynomialElement> polynomialA = generatePolynomialInField(p, q);
    vector<GeneralPolynomialElement> polynomialB = generatePolynomialInField(p, q);
    char* scalar = generateNumberOfSize(200);
    char* exponent = generateNumberOfSize(2000);

    GEN fieldOrder = powuu(p, q);
    GEN exponentPari = strtoi(exponent);
    GEN reducedExponentPari = modii(exponentPari, subiu(fieldOrder, 1));

    char* reducedExponent = itostr(reducedExponentPari);

    ofstream benchmarkDataFile;
    benchmarkDataFile.open("benchmark_data.txt");

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

    benchmarkDataFile << scalar << endl;

    benchmarkDataFile << exponent << endl;

    benchmarkDataFile << reducedExponent << endl;

    benchmarkDataFile.close();

    return 0;
}