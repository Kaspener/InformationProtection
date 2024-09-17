#include "encryption.hpp"
#include <iostream>
#include <random>
#include <unordered_map>
#include <sstream>

static std::random_device rd;

static std::default_random_engine generator(rd());

std::unordered_map<std::string, long long> ShamirEncode(const std::string& input_file, const std::string& output_file)
{
    std::ifstream file(input_file, std::ios::binary);
    if (!file.is_open()) {
        exit(1);
    }

    std::vector<int> m;
    char ch;
    while (file.get(ch)) {
        m.push_back(static_cast<unsigned char>(ch));
    }
    file.close();

    std::unordered_map<std::string, long long> mp;
    long long P = genPrime(100000, 1000000000);
    long long Ca = genMutuallyPrime(P-1);
    Euclid euclid(P, Ca);
    long long Da = inversion(Ca, P-1);
    long long Cb = genMutuallyPrime(P-1);
    long long Db = inversion(Cb, P-1);

    std::ofstream encode_file(output_file, std::ios::out);
    if (!encode_file.is_open()) {
        exit(1);
    }

    for(int part : m){
        long long x1 = modulPow(part, Ca, P);
        long long x2 = modulPow(x1, Cb, P);
        long long x3 = modulPow(x2, Da, P);
        encode_file << x3 << " ";
    }
    mp["P"] = P;
    mp["Ca"] = Ca;
    mp["Cb"] = Cb;
    mp["Da"] = Da;
    mp["Db"] = Db;
    return mp;
}

void ShamirDecode(std::unordered_map<std::string, long long>& key, const std::string& input_file, const std::string& output_file){
    std::ifstream file(input_file, std::ios::in);
    if (!file.is_open()) {
        exit(1);
    }

    std::vector<int> x3;
    int temp;
    while (file >> temp) {
        x3.push_back(temp);
    }
    file.close();

    int p = key["P"];
    int Db = key["Db"];

    std::ofstream decode_file(output_file, std::ios::binary);
    if (!decode_file.is_open()) {
        exit(1);
    }

    for (int part : x3) {
        int x4 = modulPow(part, Db, p);
        decode_file.put(static_cast<char>(x4));
    }

    decode_file.close();
}

std::unordered_map<std::string, long long> ElgamalEncode(const std::string& input_file, const std::string& output_file)
{
    std::ifstream file(input_file, std::ios::binary);
    if (!file.is_open()) {
        exit(1);
    }

    std::vector<int> m;
    char ch;
    while (file.get(ch)) {
        m.push_back(static_cast<unsigned char>(ch));
    }
    file.close();

    std::unordered_map<std::string, long long> mp;

    long long P = genPrimePandQ();
    long long g = genPrimitiveRoot(P);
    std::uniform_int_distribution<long long unsigned> distribution(100000, P-2);
    long long k = distribution(generator);
    long long r = modulPow(g, k, P);
    long long Ca = distribution(generator);
    long long Da = modulPow(g, Ca, P);
    long long Cb = distribution(generator);
    long long Db = modulPow(g, Cb, P);

    std::ofstream encode_file(output_file, std::ios::out);
    if (!encode_file.is_open()) {
        exit(1);
    }

    for(int part : m){
        long long e = ((part%P) * modulPow(Db, k, P))%P; 
        encode_file << e << " ";
    }
    mp["P"] = P;
    mp["r"] = r;
    mp["Cb"] = Cb;
    return mp;
}

void ElgamalDecode(std::unordered_map<std::string, long long> &key, const std::string &input_file, const std::string &output_file)
{
    std::ifstream file(input_file, std::ios::in);
    if (!file.is_open()) {
        exit(1);
    }

    std::vector<int> x3;
    int temp;
    while (file >> temp) {
        x3.push_back(temp);
    }
    file.close();

    int P = key["P"];
    int Cb = key["Cb"];
    int r = key["r"];

    std::ofstream decode_file(output_file, std::ios::binary);
    if (!decode_file.is_open()) {
        exit(1);
    }

    for (int part : x3) {
        int x4 = ((part%P) * modulPow(r, P-1-Cb, P))%P;
        decode_file.put(static_cast<char>(x4));
    }

    decode_file.close();
}

std::vector<long long> VernamEncode(const std::string& input_file, const std::string& output_file) {
    std::ifstream file(input_file, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + input_file);
    }

    std::vector<long long> content((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
    file.close();

    std::vector<long long> codes;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (size_t i = 0; i < content.size(); ++i) {
        codes.push_back(dis(gen));
    }
    
    std::vector<int> encoded;
    for (size_t i = 0; i < content.size(); ++i) {
        encoded.push_back(content[i] ^ codes[i]);
    }

    std::ofstream fileout(output_file, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot write to file: " + output_file);
    }
    
    for (long long byte : encoded) {
        fileout.put(static_cast<char>(byte));
    }

    fileout.close();
    
    return codes;
}

void VernamDecode(std::vector<long long>& key, const std::string& input_file, const std::string& output_file) {
    std::vector<long long> decoded;
    std::ifstream file(input_file, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + input_file);
    }

    std::vector<long long> content((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
    file.close();
    if (key.size() != content.size()) {
        throw std::runtime_error("Invalid key size for Vernam decryption.");
    }
    for (size_t i = 0; i < content.size(); ++i) {
        decoded.push_back(content[i] ^ key[i]);
    }
    
    std::ofstream fileout(output_file, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot write to file: " + output_file);
    }
    
    for (long long byte : decoded) {
        fileout.put(static_cast<char>(byte));
    }

    fileout.close();
}

std::unordered_map<std::string, long long> RSAEncode(const std::string &input_file, const std::string &output_file)
{
    std::ifstream file(input_file, std::ios::binary);
    if (!file.is_open()) {
        exit(1);
    }

    std::vector<int> message;
    char ch;
    while (file.get(ch)) {
        message.push_back(static_cast<unsigned char>(ch));
    }
    file.close();

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

    std::unordered_map<std::string, long long> mp;

    std::ofstream encode_file(output_file, std::ios::out);
    if (!encode_file.is_open()) {
        exit(1);
    }

    for(int m : message){
        long long e = modulPow(m, db, Nb);
        encode_file << e << " ";
    }
    mp["cb"] = cb;
    mp["Nb"] = Nb;
    return mp;
}

void RSADecode(std::unordered_map<std::string, long long> &key, const std::string &input_file, const std::string &output_file)
{
    std::ifstream file(input_file, std::ios::in);
    if (!file.is_open()) {
        exit(1);
    }

    std::vector<int> x3;
    int temp;
    while (file >> temp) {
        x3.push_back(temp);
    }
    file.close();

    long long cb = key["cb"];
    long long Nb = key["Nb"];

    std::ofstream decode_file(output_file, std::ios::binary);
    if (!decode_file.is_open()) {
        exit(1);
    }

    for (int part : x3) {
        int x4 = modulPow(part, cb, Nb);
        decode_file.put(static_cast<char>(x4));
    }

    decode_file.close();
}