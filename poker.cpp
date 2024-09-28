#include "poker.hpp"
#include "crypto_lib.hpp"
#include <random>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

static std::random_device rd;

static std::default_random_engine generator(rd());

std::unordered_map<long long, std::string> deckGeneration()
{
    std::vector<std::string> suit = {SPADE, CLUB, HEART, DIAMOND};
    std::vector<std::string> number = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    std::unordered_map<long long, std::string> deck;
    for(int i = 0; i < suit.size(); ++i){
        for(int j = 0; j < number.size(); ++j){
            deck[number.size()*i + j + 2] = number[j] + suit[i];
        }
    }

    for(auto& [key, value] : deck)
    {
        std::cout << value << "(" << key << ") ";
    }
    std::cout << std::endl;
    return deck;
}

void poker(int playersCount)
{
    if (playersCount > 23){
        std::cout << "TO MANY PLAYERS" << std::endl;
        return;
    }
    long long P = genPrimePandQ();
    std::cout << "P = " << P << std::endl;
    std::vector<long long> c(playersCount);
    std::vector<long long> d(playersCount);
    for(int i = 0; i < playersCount; ++i){
        c[i] = genMutuallyPrime(P-1);
        d[i] = inversion(c[i], P-1);
    }
    std::cout << "DECK:" << std::endl;
    std::unordered_map<long long, std::string> deck = deckGeneration();
    std::cout << std::endl;
    
    std::vector<long long> numbers;
    numbers.reserve(52);
    for(auto& [key, value] : deck) numbers.push_back(key);

    for(int i = 0; i < playersCount; ++i){
        for(auto& key : numbers){
            key = modulPow(key, c[i], P);
        }
        std::random_shuffle(numbers.begin(), numbers.end());
    }


    std::vector<std::vector<long long>> hands(playersCount, std::vector<long long>(2));
    for(int i = 0; i < playersCount; ++i){
        hands[i][0] = numbers[i*2];
        hands[i][1] = numbers[i*2+1];
    } 

    std::vector<long long> table(5);
    for(int i = 0; i < 5; ++i){
        table[i] = numbers[playersCount*2+i];
    }
    for(int i = 0; i < playersCount; ++i){
        for(auto& key : table){
            key = modulPow(key, d[i], P);
        }
    }
    
    std::cout << "CARDS ON TABLE:" << std::endl;
    for(auto& key : table){
        std::cout << deck[key] << "(" << key << ") " << std::endl;
    }

    for(int i = 0; i < playersCount; ++i){
        for(int j = 0; j < playersCount; ++j){
            if (i!=j){
                hands[i][0] = modulPow(hands[i][0], d[j], P);
                hands[i][1] = modulPow(hands[i][1], d[j], P);
            }
        }
        hands[i][0] = modulPow(hands[i][0], d[i], P);
        hands[i][1] = modulPow(hands[i][1], d[i], P);
    }

    std::cout << std::endl << "CARDS ON PLAYERS:" << std::endl;
    for(int i = 0; i < playersCount; ++i){
        std::cout << "PLAYER " << i+1 << std::endl;
        std::cout << deck[hands[i][0]] << "(" << hands[i][0] << ") "; 
        std::cout << deck[hands[i][1]] << "(" << hands[i][1] << ")" << std::endl << std::endl; 
    }

    std::cout << "KEYS:" << std::endl;
    for(int i = 0; i < playersCount; ++i){
        std::cout << "C(" << i+1 << ") = " << c[i] << "\t\tD(" << i+1 << ") = " << d[i] << std::endl;  
    }
}
