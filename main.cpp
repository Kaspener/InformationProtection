#include <iostream>
#include "encryption.hpp"
#include <vector>
#include <fstream>

void encodeAndDecodeFiles(){
    std::string filename = "file.txt";
    
    auto key = ShamirEncode(filename, "shamir_encode.txt");
    ShamirDecode(key, "shamir_encode.txt", "shamir_decode.txt");
    auto key2 = ElgamalEncode(filename, "elgamal_encode.txt");
    ElgamalDecode(key2, "elgamal_encode.txt", "elgamal_decode.txt");
    std::vector<long long> key3 = VernamEncode(filename, "vernam_encode.txt");
    VernamDecode(key3, "vernam_encode.txt", "vernam_decode.txt");
    auto key4 = RSAEncode(filename, "RSA_encode.txt");
    RSADecode(key4, "RSA_encode.txt", "RSA_decode.txt");
}

int main()
{
    encodeAndDecodeFiles();
    return 0;
}