#include <iostream>
#include "signs.hpp"

const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string RESET = "\033[0m";

int main()
{
    std::string filename = "file.txt";
    std::string fileBad = "fileBad.txt";
    std::unordered_map<std::string, long long> mp = RSA_sign(filename);
    if (RSA_sign_check(filename, mp)){
        std::cout << GREEN << "Correct RSA sign" << RESET << std::endl;
    }
    if (!RSA_sign_check(fileBad, mp)){
        std::cout << RED << "ERROR RSA sign" << RESET << std::endl;
    }
    std::unordered_map<std::string, long long> mp2 = Elgamal_sign(filename);
    if (Elgamal_sign_check(filename, mp2)){
        std::cout << GREEN << "Correct Elgamal sign" << RESET << std::endl;
    }
    if (!Elgamal_sign_check(fileBad, mp2)){
        std::cout << RED << "ERROR Elgamal sign" << RESET << std::endl;
    }
    std::unordered_map<std::string, long long> mp3 = GOST_sign(filename);
    if (GOST_sign_check(filename, mp3)){
        std::cout << GREEN << "Correct GOST sign" << RESET << std::endl;
    }
    if (!GOST_sign_check(fileBad, mp3)){
        std::cout << RED << "ERROR GOST sign" << RESET << std::endl;
    }
    return 0;
}