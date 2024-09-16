#include "crypto_lib.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <random>
#include <unordered_map>
#include <algorithm>

static std::random_device rd;

static std::default_random_engine generator(rd());

Euclid::Euclid(long long a, long long b)
{
    if (a == 0 && b == 0)
        throw std::invalid_argument("Both numbers cannot be zero.");

    extendedGCD(a, b);
}

long long Euclid::a() { return m_a; }

long long Euclid::b() { return m_b; }

long long Euclid::x() { return m_x; }

long long Euclid::y() { return m_y; }

long long Euclid::gcd() { return m_gcd; }

void Euclid::extendedGCD(long long a, long long b)
{
    if (a < b)
        std::swap(a, b);

    m_a = a;
    m_b = b;

    std::vector<long long> u = {a, 1, 0};
    std::vector<long long> v = {b, 0, 1};
    std::vector<long long> t(3);
    while (v[0] != 0)
    {
        long long q = u[0] / v[0];

        t[0] = u[0] % v[0];
        t[1] = u[1] - v[1] * q;
        t[2] = u[2] - v[2] * q;

        std::copy(v.begin(), v.begin() + v.size(), u.begin());
        std::copy(t.begin(), t.begin() + t.size(), v.begin());
    }
    m_gcd = u[0];
    m_x = u[1];
    m_y = u[2];
}

void Euclid::printEquation()
{
    std::cout << m_a << "*" << m_x << " + " << m_b << "*" << m_y << " = " << m_gcd << std::endl;
}

long long modulPow(long long basis, long long indicator, const long long &modul)
{
    if (basis < 0)
        throw std::invalid_argument("Basis must be a natural number!");
    if (indicator < 0)
        throw std::invalid_argument("Indicator must be a natural number!");
    if (modul <= 0)
        throw std::invalid_argument("Modul must be greater than zero!");

    long long result = 1;

    basis %= modul;

    while (indicator > 0)
    {
        if (indicator & 1)
        {
            result = (result * basis) % modul;
        }
        indicator >>= 1;
        basis = (basis * basis) % modul;
    }

    return result;
}

long long inversion(long long c, long long m)
{
    if (c > m)
        c %= m;
    Euclid euclid(m, c);
    if (euclid.gcd() != 1)
    {
        throw std::runtime_error("Numbers must be mutually simple!");
    }
    long long res = euclid.y();
    if (res < 0)
        res += m;
    return res;
}

bool miillerTest(long long d, const long long &n)
{
    int a = 2 + rand() % (n - 4);

    int x = modulPow(a, d, n);

    if (x == 1 || x == n - 1)
        return true;

    while (d != n - 1)
    {
        x = (x * x) % n;
        d *= 2;

        if (x == 1)
            return false;
        if (x == n - 1)
            return true;
    }

    return false;
}

bool isPrime(const long long &number, const int &accuracy)
{
    if (number <= 1 || number == 4)
        return false;
    if (number <= 3)
        return true;

    if (number % 2 == 0)
        return false;

    int d = number - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < accuracy; i++)
        if (!miillerTest(d, number))
            return false;

    return true;
}

long long genPrimePandQ()
{
    std::uniform_int_distribution<long long unsigned> distribution(100000, 1000000000);

    while (1)
    {
        long long Q = distribution(generator);
        if (isPrime(Q))
        {
            long long P = 2 * Q + 1;
            if (isPrime(P))
                return P;
        }
    }
}

long long genPrime(long long min, long long max)
{
    std::uniform_int_distribution<long long unsigned> distribution(min, max);

    while (1)
    {
        long long Q = distribution(generator);
        if (isPrime(Q))
        {
            return Q;
        }
    }
}

long long genMutuallyPrime(const long long& P)
{
    std::uniform_int_distribution<long long unsigned> distribution(100000, P-1);
    while(true)
    {
        long long number = distribution(generator);
        Euclid euqlid(number, P);
        if (euqlid.gcd() == 1) return number;
    }
}

long long genPrimitiveRoot(const long long &P)
{
    std::uniform_int_distribution<long long> distribution(2, P - 1);

    while (1)
    {
        long long g = distribution(generator);
        if (modulPow(g, P / 2, P) == 1)
            return g;
    }
}

long long diffieHellman()
{
    long long P = genPrimePandQ();
    long long g = genPrimitiveRoot(P);
    std::cout << "P = " << P << " | g = " << g << std::endl;
    std::uniform_int_distribution<long long> distribution(1, P - 1);
    long long Xa = distribution(generator);
    long long Xb = distribution(generator);
    std::cout << "Xa = " << Xa << " | Xb = " << Xb << std::endl;
    long long Ya = modulPow(g, Xa, P);
    long long Yb = modulPow(g, Xb, P);
    std::cout << "Ya = " << Ya << " | Yb = " << Yb << std::endl;
    long long Zab = modulPow(Yb, Xa, P);
    long long Zba = modulPow(Ya, Xb, P);
    std::cout << "Zab = " << Zab << " | Zba = " << Zba << std::endl;
    return Zab;
}

long long giantBabyStep(const long long &a, const long long &p, const long long &y)
{
    long long m, k;
    m = k = sqrt(p) + 1;
    std::unordered_map<long long, long long> mp;
    long long num = y % p;
    mp[y] = 0;
    for (int i = 1; i < m; ++i)
    {
        num = (num * a) % p;
        mp[num] = i;
    }
    num = modulPow(a, m, p);
    if (mp.count(num))
    {
        return m - mp[num];
    }
    long long step = num;
    for (int i = 2; i <= k; ++i)
    {
        num = (num*step)%p;
        if (mp.count(num))
        {
            return m * i - mp[num];
        }
    }
    return -1;
}
