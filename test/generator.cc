#include "generator.h"

//This only works with small numbers (p and q are both convertable to int)

vector<GeneralPolynomialElement> pariPolynomialToGeneralPolynomial(GEN Polynomial, unsigned int PolynomialDegree)
{
    vector<GeneralPolynomialElement> generalPolynomial;

    for(unsigned int i = PolynomialDegree + 2; i >= 2; i--)
    {
        generalPolynomial.push_back({itou(gel(Polynomial, i)), i - 2});
    }

    return generalPolynomial;
}

vector<GeneralPolynomialElement> generateIrreduciblePolynomial(unsigned int p, unsigned int q)
{
    GEN pPari = utoi(p);

    GEN irreduciblePolynomial = init_Fq(pPari, q, 0);

    return pariPolynomialToGeneralPolynomial(irreduciblePolynomial, q);
}

char* generatePrimeOfSize(unsigned int bitsize)
{
    GEN baseByBitsize = int2u(bitsize);
    GEN randomNumber;
    GEN randomNumberByBitsize = gen_0;

    int isPrime = 0;
    do
    {
        if(cmpii(randomNumberByBitsize, baseByBitsize) <= 0)
        {           
            randomNumber = randomi(baseByBitsize); 
            randomNumberByBitsize = addii(baseByBitsize, randomNumber);

            if(umodiu(randomNumberByBitsize, 2) == 0)
            {
                if(cmpiu(randomNumber, 0) > 0)
                {
                    randomNumberByBitsize = subiu(randomNumberByBitsize, 1);
                }
                else
                {
                    randomNumberByBitsize = addiu(randomNumberByBitsize, 1);
                }
            }
        }

        randomNumberByBitsize = subiu(randomNumberByBitsize, 2);
        
        isPrime = isprime(randomNumberByBitsize);
    }while(!isPrime);

    return itostr(randomNumberByBitsize);
}

vector<GeneralPolynomialElement> generatePolynomialInField(unsigned int p, unsigned int q)
{
    GEN pPari = utoi(p);
    GEN randomPolynomial = random_FpX(q, 0, pPari);
    return pariPolynomialToGeneralPolynomial(randomPolynomial, degpol(randomPolynomial));
}

char* generateNumberOfSize(unsigned int bitsize)
{
    GEN baseByBitsize = int2u(bitsize);
    GEN randomNumber = randomi(baseByBitsize);

    return itostr(addii(baseByBitsize, randomNumber));
}

char* generateNumberInRange(GEN p)
{
    return itostr(randomi(p));
}

void initGenerator()
{
    pari_init(2000000000,0);
}