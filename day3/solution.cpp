#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

bool PART1 = false;
int N = PART1 ? 2 : 12;

// 17263
// 170731717900423 

unsigned long long maxJoltage(std::string bank, int n, int start) {
    if (n == 0) return 0;
    n -= 1;

    int maxDigit = 0;
    int maxDigitIndex = -1;
    for (int i = start; i < bank.size() - n; i++) {
        int digit = bank[i] - '0';
        if (digit > maxDigit) {
            maxDigit = digit;
            maxDigitIndex = i;
        }
    }
    return maxDigit * pow(10.0, n) + maxJoltage(bank, n, maxDigitIndex + 1);
}

int main() {
    std::ifstream file("input.txt");
    std::string bank; 

    unsigned long long totalJoltage = 0;
    while (std::getline(file, bank))
    {   
        unsigned long long batteryJoltage = maxJoltage(bank, N, 0);
        totalJoltage += batteryJoltage;
    }
    std::cout << "Total joltage: " << totalJoltage << std::endl;
    return 0;
}