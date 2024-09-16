#include "encryption.hpp"
#include <iostream>
#include <random>

static std::random_device rd;

static std::default_random_engine generator(rd());

void ShamirEncode()
{
    long long P = genPrime(100000, 1000000000);
    long long Ca = genMutuallyPrime(P-1);
    Euclid euclid(P, Ca);
    long long Da = inversion(Ca, P-1);
    long long Cb = genMutuallyPrime(P-1);
    long long Db = inversion(Cb, P-1);
    std::uniform_int_distribution<long long unsigned> distribution(100000, P-1);
    long long message = distribution(generator);
    long long x1 = modulPow(message, Ca, P);
    long long x2 = modulPow(x1, Cb, P);
    long long x3 = modulPow(x2, Da, P);
    long long x4 = modulPow(x3, Db, P);
    std::cout << "P = " << P << std::endl;
    std::cout << "Ca = " << Ca << "\t\tCb = " << Cb << std::endl;
    std::cout << "Da = " << Da << "\t\tDb = " << Db << std::endl;
    std::cout << "Message = " << message << std::endl;
    std::cout << "A calculate x1 = " << x1 << std::endl;
    std::cout << "B calculate x2 = " << x2 << std::endl;
    std::cout << "A calculate x3 = " << x3 << std::endl;
    std::cout << "B calculate x4 = " << x4 << std::endl;
}

void Elgamal()
{
    long long P = genPrimePandQ();
    long long g = genPrimitiveRoot(P);
    std::uniform_int_distribution<long long unsigned> distribution(100000, P-2);
    long long k = distribution(generator);
    long long r = modulPow(g, k, P);
    long long message = distribution(generator);
    long long Ca = distribution(generator);
    long long Da = modulPow(g, Ca, P);
    long long Cb = distribution(generator);
    long long Db = modulPow(g, Cb, P);
    long long e = ((message%P) * modulPow(Db, k, P))%P; 
    long long newMessage = ((e%P) * modulPow(r, P-1-Cb, P))%P;
    std::cout << "P = " << P << std::endl;
    std::cout << "Ca = " << Ca << "\t\tCb = " << Cb << std::endl;
    std::cout << "Da = " << Da << "\t\tDb = " << Db << std::endl;
    std::cout << "Message = " << message << std::endl;
    std::cout << "k = " << k << std::endl;
    std::cout << "r = " << r << std::endl;
    std::cout << "e = " << e << std::endl;
    std::cout << "New message = " << newMessage << std::endl;
}

void Vernam(const std::string& message)
{
    srand(time(0));
    std::string code;
    std::string key;
    code.resize(message.length());
    key.resize(message.length());
    for(int i = 0; i < message.length(); ++i){
        key[i] = rand()%256;
        code[i] = key[i] ^ message[i];
    }
    std::cout << "Code: " << code << std::endl;
    for(int i = 0; i < message.length(); ++i){
        code[i] ^= key[i];
    }
    std::cout << "Decode: " << code << std::endl;
}

void RSA()
{
    long long Pa = genPrime(1000, 50000);
    long long Qa = genPrime(1000, 50000);
    long long Na = Pa*Qa;
    long long fia = (Pa-1)*(Qa-1);
    long long da = genMutuallyPrime(fia);
    long long ca = inversion(da, fia);
    long long Pb = genPrime(1000, 50000);
    long long Qb = genPrime(1000, 50000);
    long long Nb = Pb*Qb;
    long long fib = (Pb-1)*(Qb-1);
    long long db = genMutuallyPrime(fib);
    long long cb = inversion(db, fib);
    std::uniform_int_distribution<long long unsigned> distribution(1000, Nb-1);
    long long message = distribution(generator);

    std::cout << "Pa = " << Pa << "\t\tPb = " << Pb << std::endl;
    std::cout << "Qa = " << Qa << "\t\tQb = " << Qb << std::endl;
    std::cout << "Na = " << Na << "\t\tNb = " << Nb << std::endl;
    std::cout << "fia = " << fia << "\t\tfib = " << fib << std::endl;
    std::cout << "da = " << da << "\t\tdb = " << db << std::endl;
    std::cout << "ca = " << ca << "\t\tcb = " << cb << std::endl;
    std::cout << "Message = " << message << std::endl;
    long long e = modulPow(message, db, Nb);
    std::cout << "E = " << e << std::endl;
    long long newMessage = modulPow(e, cb, Nb);
    
    std::cout << "New message = " << message << std::endl;
}
