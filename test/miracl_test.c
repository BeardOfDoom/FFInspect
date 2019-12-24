/*
 * gcc miracl_test.c ../home/vagrant/miracl/miracl.a -I../home/vagrant/miracl
 * Program to calculate factorials.
 */

#include <stdio.h>
#include "miracl.h"   /* include MIRACL system */

int main()
{
    /* calculate factorial of number */
    big nf;        /* declare "big" variable nf */
    int n;
    miracl *mip=mirsys(5000,10);

    /* base 10, 5000 digits per big  */
    nf=mirvar(1);   /* initialise big variable nf=1  */
    printf("factorial program\n");
    printf("input number n= \n");
    scanf("%d",&n);
    getchar();
    while (n>1)
        premult(nf,n--,nf); /* nf=n!=n*(n-1)*...2*1  */

    printf("n!= \n");
    otnum(nf,stdout); /* output result */

    return 0;
}