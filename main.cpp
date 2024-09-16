#include <iostream>
#include "encryption.hpp"

int main()
{
    std::cout << "----------SHAMIR---------" << std::endl;
    ShamirEncode();
    std::cout << std::endl
              << std::endl;

    std::cout << "----------ELGAMAL--------" << std::endl;
    Elgamal();
    std::cout << std::endl
              << std::endl;

    std::cout << "----------VERNAM---------" << std::endl;
    Vernam("Nu privet :-)");
    std::cout << std::endl
              << std::endl;
    std::cout << "------------RSA----------" << std::endl;
    RSA();
    std::cout << std::endl
              << std::endl;
    return 0;
}