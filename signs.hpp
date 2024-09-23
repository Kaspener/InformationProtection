#pragma once
#include "crypto_lib.hpp"
#include <string>
#include <unordered_map>

std::string readFile(const std::string& filePath);

std::unordered_map<std::string, long long> RSA_sign(const std::string& filePath);
bool RSA_sign_check(const std::string& filePath, std::unordered_map<std::string, long long> mp);

std::unordered_map<std::string, long long> Elgamal_sign(const std::string& filePath);
bool Elgamal_sign_check(const std::string& filePath, std::unordered_map<std::string, long long> mp);

std::unordered_map<std::string, long long> GOST_sign(const std::string& filePath);
bool GOST_sign_check(const std::string& filePath, std::unordered_map<std::string, long long> mp);