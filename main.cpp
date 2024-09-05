#include <iostream>
#include "crypto_lib.hpp"

void test1(){
    long long basis = 7;
    long long indicator = 3;
    long long modul = 9;
    long long res = 0;
    try
    {
        res = modulPow(basis, indicator, modul);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    std::cout << res << std::endl;
}

void test2(){
    Euclid euclid(30, 12);
    euclid.printEquation();
}

void test3(){
    diffieHellman();
}

void test4(){
    std::cout << giantBabyStep(7, 9, 1) << std::endl;
}

int main(){
    std::cout << "----------1------------" << std::endl; 
    test1();
    std::cout << std::endl;
    std::cout << "----------2------------" << std::endl; 
    test2();
    std::cout << std::endl;
    std::cout << "----------3------------" << std::endl; 
    test3();
    std::cout << std::endl;
    std::cout << "----------4------------" << std::endl; 
    test4();
    std::cout << std::endl;
    return 0;
}