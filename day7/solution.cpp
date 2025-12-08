#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>

using large = unsigned long long;

void insertAndSum(int pos, large count, std::unordered_map<int, large>& positions) {
    if (positions.find(pos) != positions.end()) {
        positions[pos] += count;
    } else {
        positions.insert({pos, count});
    }
}

int main() {

    std::ifstream infile("input.txt");
    std::string line;


    std::getline(infile, line);
    std::unordered_map<int, large> positions;
    positions.insert({line.find('S'), 1});
    std::getline(infile, line);
    

    int splits = 0;
    while (std::getline(infile, line)) {
       std::unordered_map<int, large> new_positions;
        for (const auto& [pos, count] : positions) {
            if (line[pos] == '^') {
                splits++;
                insertAndSum(pos - 1, count, new_positions);
                insertAndSum(pos + 1, count, new_positions);
            } else {
                insertAndSum(pos, count, new_positions);
            }
        }
        positions = new_positions;
        std::getline(infile, line);
    }

    large total_timelines = 0;
    for (const auto& [pos, count] : positions) {
        std::cout << "End position: " << pos << " reached in : " << count << " timelines." << std::endl;
        total_timelines += count;        
    }
    std::cout << "Number of possible end positions: " << positions.size() << std::endl;
    std::cout << "(PART1) Number of splits: " << splits << std::endl;
    std::cout << "(PART2) Total number of timelines: " << total_timelines << std::endl;
    return 0;
}