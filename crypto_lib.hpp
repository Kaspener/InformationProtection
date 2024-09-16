#ifndef CRYPTO_LIB_HPP
#define CRYPTO_LIB_HPP

class Euclid
{
private:
    long long m_a;
    long long m_b;
    long long m_x;
    long long m_y;
    long long m_gcd;

public:
    Euclid(long long a, long long b);

    long long a();
    long long b();
    long long x();
    long long y();
    long long gcd();

    void extendedGCD(long long a, long long b);
    void printEquation();
};

long long modulPow(long long basis, long long indicator, const long long& modul);

long long inversion(long long c, long long m);

long long diffieHellman();

long long giantBabyStep(const long long& a, const long long& p, const long long& y);

bool isPrime(const long long &number, const int &accuracy = 10);

long long genPrime(long long min, long long max);
long long genPrimePandQ();
long long genMutuallyPrime(const long long& P);

long long genPrimitiveRoot(const long long &P);


#endif //CRYPTO_LIB_HPP