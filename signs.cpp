#include "signs.hpp"
#include "MD5.h"
#include <fstream>
#include <sstream>
#include <random>

long long mod(long long a, long long b)
{
    if (b < 0)
        return -mod(-a, -b);
    int ret = a % b;
    if (ret < 0)
        ret += b;
    return ret;
}

static std::random_device rd;

static std::default_random_engine generator(rd());

std::string readFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

void writeSign(const std::string &filePath, const std::vector<long long> &data)
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    for (const auto &item : data)
    {
        file << item << " ";
    }
}

std::vector<long long> readSign(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::vector<long long> data;
    long long value;

    while (file >> value)
    {
        data.push_back(value);
    }

    return data;
}

std::unordered_map<std::string, long long> RSA_sign(const std::string &filePath)
{
    long long P = genPrime(1000, 50000);
    long long Q = genPrime(1000, 50000);
    long long N = P * Q;
    long long fi = (P - 1) * (Q - 1);
    long long d = genMutuallyPrime(fi);
    long long c = inversion(d, fi);
    std::string message = readFile(filePath);
    std::string hash = md5(message);
    std::vector<long long> hash_vt = HashToVector(hash);

    std::vector<long long> s;
    for (auto &it : hash_vt)
    {
        s.push_back(modulPow(it, c, N));
    }
    writeSign("RSA_sign.txt", s);

    std::unordered_map<std::string, long long> mp;
    mp["d"] = d;
    mp["N"] = N;

    return mp;
}

bool RSA_sign_check(const std::string &filePath, std::unordered_map<std::string, long long> mp)
{
    long long d = mp["d"];
    long long N = mp["N"];
    std::string message = readFile(filePath);
    std::vector<long long> s = readSign("RSA_sign.txt");
    std::vector<long long> w;
    for (auto &it : s)
    {
        w.push_back(modulPow(it, d, N));
    }
    std::string hash = md5(message);
    std::vector<long long> hash_vt = HashToVector(hash);
    return w == hash_vt;
}

std::unordered_map<std::string, long long> Elgamal_sign(const std::string &filePath)
{
    std::unordered_map<std::string, long long> mp;
    long long P = genPrimePandQ();
    long long g = genPrimitiveRoot(P);
    std::uniform_int_distribution<long long unsigned> distribution(100000, P - 2);
    long long x = distribution(generator);
    long long y = modulPow(g, x, P);
    long long k = genMutuallyPrime(P - 1);
    long long r = modulPow(g, k, P);
    long long negK = inversion(k, P - 1);

    std::string message = readFile(filePath);
    std::string hash = md5(message);
    std::vector<long long> hash_vt = HashToVector(hash);

    std::vector<long long> s;
    for (auto &it : hash_vt)
    {
        long long u = mod(it - x * r, P - 1);
        s.push_back(((negK % (P - 1)) * (u % (P - 1))) % (P - 1));
    }
    writeSign("Elgamal_sign.txt", s);
    mp["P"] = P;
    mp["g"] = g;
    mp["y"] = y;
    mp["r"] = r;
    return mp;
}

bool Elgamal_sign_check(const std::string &filePath, std::unordered_map<std::string, long long> mp)
{
    long long P = mp["P"];
    long long g = mp["g"];
    long long y = mp["y"];
    long long r = mp["r"];

    std::string message = readFile(filePath);
    std::string hash = md5(message);
    std::vector<long long> hash_vt = HashToVector(hash);
    std::vector<long long> s = readSign("Elgamal_sign.txt");
    std::vector<long long> left;
    std::vector<long long> right;
    for (auto &it : hash_vt)
    {
        right.push_back(modulPow(g, it, P));
    }
    for (auto &it : s)
    {
        left.push_back(((modulPow(y, r, P) % P) * (modulPow(r, it, P) % P) % P));
    }
    return left == right;
}

std::unordered_map<std::string, long long> GOST_sign(const std::string &filePath)
{
    std::unordered_map<std::string, long long> mp;
    std::uniform_int_distribution<long long> dist(2, (1LL << 16) - 1);
    std::uniform_int_distribution<long long> dist_qb(2, (1LL << 16) - 1);
    long long a;
    long long q;
    long long p;
    q = genPrime(2, (1LL << 16) - 1);
    long long b = dist(generator);
    do
    {
        p = q * b + 1;
        b = dist(generator);
    } while (!isPrime(p));
    do
    {
        a = dist_qb(generator);
    } while (modulPow(a, q, p) != 1);
    std::uniform_int_distribution<long long> randX(1, q - 1);
    long long x = randX(generator);
    long long y = modulPow(a, x, p);

    std::string message = readFile(filePath);
    std::string hash = md5(message);
    std::vector<long long> hash_vt = HashToVector(hash);

    long long r = 0;
    std::vector<long long> s;
    long long k;
    bool flag = true;
    while (flag)
    {
        flag = false;
        s.clear();
        while (r == 0)
        {
            k = randX(generator);
            r = mod(modulPow(a, k, p), q);
        }
        for (auto &it : hash_vt)
        {
            long long temp = mod(k * it + x * r, q);
            if (temp == 0)
            {
                flag = true;
                break;
            }
            s.push_back(temp);
        }
    }
    writeSign("GOST_sign.txt", s);
    mp["p"] = p;
    mp["q"] = q;
    mp["a"] = a;
    mp["r"] = r;
    mp["y"] = y;
    return mp;
}

bool GOST_sign_check(const std::string &filePath, std::unordered_map<std::string, long long> mp)
{
    long long p = mp["p"];
    long long a = mp["a"];
    long long r = mp["r"];
    long long y = mp["y"];
    long long q = mp["q"];

    std::string message = readFile(filePath);
    std::string hash = md5(message);
    std::vector<long long> hash_vt = HashToVector(hash);
    std::vector<long long> s = readSign("GOST_sign.txt");
    std::vector<long long> negH;
    std::vector<long long> v;
    for (auto &it : hash_vt)
    {
        negH.push_back(inversion(it, q));
    }
    for (int i = 0; i < s.size(); ++i)
    {
        long long u1 = mod(s[i] * negH[i], q);
        long long u2 = mod(-r * negH[i], q);
        v.push_back(mod(mod(modulPow(a, u1, p) * modulPow(y, u2, p), p), q));
    }
    for (auto &it : v)
    {
        if (it != r)
            return false;
    }
    return true;
}
