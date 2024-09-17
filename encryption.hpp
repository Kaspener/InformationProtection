#pragma once
#include "crypto_lib.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

std::unordered_map<std::string, long long> ShamirEncode(const std::string& input_file, const std::string& output_file);
void ShamirDecode(std::unordered_map<std::string, long long>& key, const std::string& input_file, const std::string& output_file);

std::unordered_map<std::string, long long> ElgamalEncode(const std::string& input_file, const std::string& output_file);
void ElgamalDecode(std::unordered_map<std::string, long long>& key, const std::string& input_file, const std::string& output_file);

std::vector<long long> VernamEncode(const std::string& input_file, const std::string& output_file);
void VernamDecode(std::vector<long long>& key, const std::string& input_file, const std::string& output_file);

std::unordered_map<std::string, long long> RSAEncode(const std::string& input_file, const std::string& output_file);
void RSADecode(std::unordered_map<std::string, long long>& key, const std::string& input_file, const std::string& output_file);