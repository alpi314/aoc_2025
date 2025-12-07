#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using large = unsigned long long;

int getNumber(std::vector<std::string> s, int i) {
    int num = -1;
    for (int j = 0; j < s.size() - 1; j++) {
        if (s[j][i] == ' ') continue;
        if (num == -1) num = s[j][i] - '0';
        else {
            num = num * 10 + (s[j][i] - '0');
        }
    }
    return num;
}

int main() {
    std::ifstream infile("input.txt");
    std::string line;

    int index = 0;
    std::vector<std::string> lines;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }

    large sum_of_results = 0;
    while (index < lines[0].size()) {
        char op = lines.back()[index];

        large result = op == '+' ? 0 : 1;
        while (index < lines[0].size()) {
            int number = getNumber(lines, index);
            if (number == -1) break;

            if (op == '+') {
                result += number;
            } else if (op == '*') {
                result *= number;
            }
            index += 1;
        }
        
        index += 1; // skip space
        sum_of_results += result;
    }

    std::cout << "Grand total is: " << sum_of_results << std::endl;
    return 0;
}