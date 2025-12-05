#include <fstream>
#include <string>
#include <cstdint>
#include <iostream>
#include <vector>

bool PART1 = false;
 
// 1227775554
// 52316131093

// 4174379265
// 69564213293

bool isInvalid(const std::string& s, int offset) {
    int num_offsets = s.length() / offset;
    for (int i = 0; i < offset; i++) {
        char left = s[i];
        for (int j = 1; j < num_offsets; j++) {
            if (s[i + j * offset] != left) {
                return false;
            }
        }
    }
    return true;
}

std::vector<int> lengthDivisors(const std::string& s) {
    if (PART1) {
        if (s.length() % 2 == 0) {
            int half = s.length() / 2;
            return {half};
        } else {
            return {};
        }
    }
    std::vector<int> divisors;
    int len = s.length();
    for (int i = len / 2; i >= 1; i--) {
        if (len % i == 0) {
            divisors.push_back(i);
        }
    }
    return divisors;
}


int64_t invalidSum(std::string start, std::string end) {
    if (start.length() < end.length()) {
        std::string nend = std::string(start.length(), '9');
        std::string nstart = "1" + std::string(end.length() - 1, '0');
        return invalidSum(start, nend) + invalidSum(nstart, end);
    }

    int64_t invalid_sum = 0;
    int64_t start_int = std::stoll(start);
    int64_t end_int = std::stoll(end);
    for (int64_t i = start_int; i <= end_int; i++) {
        std::string s = std::to_string(i);
        for (int offset : lengthDivisors(s)) {
            if (isInvalid(s, offset)) {
                std::cout << "Found invalid: " << s << " with offset " << offset << std::endl;
                invalid_sum += i;
                break;
            }
        }
    }
    return invalid_sum;
}
int main() {
    std::ifstream file("input.txt");
    std::string str;
    std::getline(file, str);

    int64_t r = 0;
    
    while (!str.empty()) {
        size_t comma = str.find(',');
        std::string token;

        if (comma != std::string::npos) {
            token = str.substr(0, comma);
            str = str.substr(comma + 1);
        } else {
            token = str;
            str = "";
        }

        size_t dash = token.find('-');
        if (dash != std::string::npos) {
            std::string start_str = token.substr(0, dash);
            std::string end_str = token.substr(dash + 1);
            
            std::cout << "Processing range: " << start_str << "-" << end_str << std::endl;
            
            r += invalidSum(start_str, end_str);
        }
    }
    
    printf("Final result: %lld\n", r);
    return 0;
}