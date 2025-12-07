#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using large = unsigned long long;

int getNumber(std::string s, int* i) {
    int num = 0;
    while ((*i) < s.size() && s[*i] != ' ') {
        num = num * 10 + (s[*i] - '0');
        (*i)++;
    }
    return num;
}

void skipSpaces(std::string s, int* i) {
    while ((*i) < s.size() && s[*i] == ' ') {
        (*i)++;
    }
}

int main() {
    std::ifstream infile("input.txt");
    std::string line;

    std::vector<int*> indices;
    std::vector<std::string> lines;
    while (std::getline(infile, line)) {
        lines.push_back(line);
        indices.push_back(new int(0));
        skipSpaces(line, indices.back());
    }

    large sum_of_results = 0;
    while ((*indices.back()) < lines.back().size()) {
        char op = lines.back()[*indices.back()];
        (*indices.back())++;
        skipSpaces(lines.back(), indices.back());

        large result = op == '+' ? 0 : 1;
        for (int i = 0; i < lines.size() - 1; i++) {
            int num = getNumber(lines[i], indices[i]);
            skipSpaces(lines[i], indices[i]);
            if (op == '+') {
                result += num;
            } else {
                result *= num;
            }
        }
        
        sum_of_results += result;
    }

    std::cout << "Grand total is: " << sum_of_results << std::endl;
    return 0;
}